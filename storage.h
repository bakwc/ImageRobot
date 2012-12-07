#pragma once

#include <QObject>
#include <QVector>
#include <QQueue>
#include <QFile>
#include <QTextStream>
#include "process.h"

class TStorage : public QObject
{
    Q_OBJECT
public:
    explicit TStorage(QObject *parent = 0);
    void Load(const QString& fileName);
    bool HasTasks();
    QString GetNextTask();
    size_t TasksLeft();
    void PutResult(TImageInfo& imageInfo);
    void Flush();
private:
    bool Writing;
    QFile File;
    QTextStream FileOut;
    QQueue<QString> Urls;
};
