#include "previewwidget.h"

previewWidget::previewWidget(QWidget *parent)
    : QWidget{parent}
{

}

void previewWidget::paintEvent(QPaintEvent *event)
{
    if(img.width() != 0 && img.height() != 0)
    {
        QPainter p(this);
        p.drawImage(0, 0, img);
    }

    event->accept();
}

void previewWidget::setImg(const QImage &newImg)
{
    img = newImg;
}
