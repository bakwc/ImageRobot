#include <QThreadPool>
#include <QDebug>
#include "application.h"

TApplication::TApplication(int argc, char *argv[])
    : QCoreApplication(argc, argv)
    , Storage(new TStorage)
    , AppState(AS_Init)
{
}

void TApplication::Init() {
    AppState = AS_Loading;
    Settings.Load("options.txt");
    Storage->Load("image-urls.txt");
}

void TApplication::Start() {
    qDebug() << "Started";
    AppState = AS_Started;
    QThreadPool* pool = QThreadPool::globalInstance();
    pool->setMaxThreadCount(Settings.GetParameter("threads"));
    size_t thumbSize = Settings.GetParameter("thumb_size");
    this->startTimer(1000);

    while (Storage->HasTasks()) {
        QSharedPointer<TProcess> process(new TProcess(Storage->GetNextTask(), thumbSize));
        Processes.push_back(process);
        while (!pool->tryStart(process.data())) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }
    pool->waitForDone();

    AppState = AS_Storing;

    for (int i = 0; i < Processes.size(); i++) {
        const QString& signature = Processes[i]->GetImageInfo().Signature;
        if (Duplicates.find(signature) == Duplicates.end()) {
            Duplicates[signature] = 0;
        } else {
            Duplicates[signature]++;
        }
    }

    for (int i = 0; i < Processes.size(); i++) {
        const QString& signature = Processes[i]->GetImageInfo().Signature;
        Processes[i]->GetImageInfo().Duplicates = Duplicates[signature];
        Storage->PutResult(Processes[i]->GetImageInfo());
    }

}

void TApplication::timerEvent(QTimerEvent *) {
    qDebug() << "Urls left: " << Storage->TasksLeft();
    if (Storage->TasksLeft() == 0) {
        Storage->Flush();
        quit();
    }
}

QString TApplication::GetAppState() {
    switch (AppState) {
    case AS_Init:
        return "init";
    case AS_Loading:
        return "loading";
    case AS_Started:
        return "started";
    case AS_Storing:
        return "storing";
    }
    return "unknown";
}
