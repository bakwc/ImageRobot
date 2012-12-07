#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QTime>
#include <QEventLoop>
#include <QImage>
#include <QImageReader>
#include <QBuffer>
#include <QCryptographicHash>
#include "process.h"

QByteArray GetPixelData(const QImage& img) {
    QByteArray result;
    for (int i = 0; i < img.width(); i++) {
        for (int j = 0; j < img.height(); j++) {
            QRgb c = img.pixel(i, j);
            result.push_back(qRed(c));
            result.push_back(qGreen(c));
            result.push_back(qBlue(c));
        }
    }
    return result;
}

TProcess::TProcess(const QString &imageUrl, size_t thumbSize)
    : ThumbSize(thumbSize)
{
    ImageInfo.ImageUrl = imageUrl;
    ImageInfo.FetchedOk = false;
    ImageInfo.ParsedOk = false;
    ImageInfo.Duplicates = 0;
    this->setAutoDelete(false);
}

void TProcess::run() {
       // QDataStream::setVersion(QDataStream::Qt_4_6);
    QNetworkAccessManager qnam;
    QNetworkRequest request = QNetworkRequest(QUrl(ImageInfo.ImageUrl));
    request.setRawHeader("User-Agent", "Mozilla/6.0 (Windows NT 6.2; WOW64; rv:16.0.1) Gecko/20121011 Firefox/16.0.1");
    QNetworkReply *reply = qnam.get(request);
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        ImageInfo.FetchedOk = true;
        QImageReader imageReader(reply);
        QImage pic = imageReader.read();
        QSize imgSize = pic.size();
        if (!imgSize.isNull()) {
            ImageInfo.ParsedOk = true;
            QImage thumb = pic.scaled(ThumbSize, ThumbSize, Qt::KeepAspectRatio);
            thumb = thumb.convertToFormat(QImage::Format_Indexed8);
            QByteArray imgData = GetPixelData(thumb);

            QCryptographicHash md5(QCryptographicHash::Md5);
            md5.addData(imgData);
            ImageInfo.Signature = md5.result().toHex().constData();
            ImageInfo.Size = imgSize;
        }
    }
}

TImageInfo& TProcess::GetImageInfo() {
    return ImageInfo;
}
