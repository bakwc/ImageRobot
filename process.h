#pragma once

#include <QRunnable>
#include <QSize>
#include <QString>

struct TImageInfo {
    QString ImageUrl;
    QSize Size;
    QString Signature;
    bool FetchedOk;
    bool ParsedOk;
    size_t Duplicates;
};

class TProcess : public QRunnable
{
public:
    TProcess(const QString& imageUrl, size_t thumbSize);
    void run();
    TImageInfo& GetImageInfo();
private:
    TImageInfo ImageInfo;
    size_t ThumbSize;
};
