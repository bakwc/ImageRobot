#pragma once

#include <QCoreApplication>
#include <QSharedPointer>
#include <QVector>
#include <QPointer>
#include <QHash>

#include "utils/usettings.h"

#include "process.h"
#include "storage.h"

enum EApplicationState {
    AS_Init,
    AS_Loading,
    AS_Started,
    AS_Storing
};

class TApplication : public QCoreApplication
{
    Q_OBJECT
public:
    explicit TApplication(int argc, char *argv[]);
    void Init();
    void Start();
private:
    void timerEvent(QTimerEvent *);
    QString GetAppState();
private:
    QVector<QSharedPointer<TProcess> > Processes;
    QPointer<TStorage> Storage;
    QHash<QString, size_t> Duplicates;
    EApplicationState AppState;
    USettings Settings;
};
