#include "imageprovider.h"
#include<QDebug>
ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
//代码中未使用Pixmap来绘制图像，若要使用，只需要将Image替换为Pixmap
{
}
QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug()<<"requestImage";
    return this->img;
}
QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug()<<"requestPixmap";
    return QPixmap::fromImage(this->img);
}
