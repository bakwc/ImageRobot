#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "storage.h"

TStorage::TStorage(QObject *parent)
    : QObject(parent)
    , Writing(false)
{
}

void TStorage::Load(const QString& fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error reading file!\n";
        return;
    }
    QTextStream in(&file);
    QString line = "";
    while (!line.isNull()) {
        line = in.readLine();
        if (!line.isEmpty() && line[0] != '#') {
            Urls.enqueue(line);
        }
    }
}

void TStorage::Flush() {
    File.flush();
}

bool TStorage::HasTasks() {
    return !Urls.empty();
}

QString TStorage::GetNextTask() {
  return Urls.dequeue();
}

size_t TStorage::TasksLeft() {
    return Urls.size();
}

void TStorage::PutResult(TImageInfo& imageInfo) {
    if (!Writing) {
        File.setFileName("results.txt");
        File.open(QFile::WriteOnly);
        FileOut.setDevice(&File);
        Writing = true;
    }
    QString line = QString("%1\t%2\t%3\t%4\t%5\t%6\t%7\n")
            .arg(imageInfo.ImageUrl)
            .arg(imageInfo.Size.width())
            .arg(imageInfo.Size.height())
            .arg(imageInfo.Duplicates)
            .arg(imageInfo.FetchedOk)
            .arg(imageInfo.ParsedOk)
            .arg(imageInfo.Signature);
    File.write(line.toUtf8());
}
