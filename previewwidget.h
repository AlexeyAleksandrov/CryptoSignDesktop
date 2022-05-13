#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QWidget>
#include <QPaintEvent>
#include <QImage>
#include <QPainter>

class previewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit previewWidget(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent *event);

    void setImg(const QImage &newImg);

signals:

private:
    QImage img;

};

#endif // PREVIEWWIDGET_H
