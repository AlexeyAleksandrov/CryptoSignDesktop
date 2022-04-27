#ifndef MY_TABLEWIDGET_H
#define MY_TABLEWIDGET_H

#include <QTableWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QDragEnterEvent>
#include <QMimeData>

class my_tableWidget : public QTableWidget
{
    Q_OBJECT
public:
    my_tableWidget(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *m_event);
    void mouseReleaseEvent(QMouseEvent *m_event);
    void mouseDoubleClickEvent(QMouseEvent * m_event);

public slots:
    void setSelectedForAllItems(bool selected); // функция установки выделния для всех элементов

signals:
    void mouseRightClick(QTableWidgetItem *item); // сигнал нажатия правой кнопколй мыши по таблице
    void mouseDoubleClick(QTableWidgetItem *item); // сигнал нажатия правой кнопколй мыши по таблице
    void dropFiles(QStringList files); // сигнал добавления файлов перетаскиванием

private:
    struct pressedPosition
    {
        int x = 0;
        int y = 0;
        void setPosByEvent(QMouseEvent *m_event);
        void clear();
    };

    pressedPosition m_press_position;

private:
    void check(int &one, int &two); // функция проверки на обратный порядок
    void setSelectedState(int row, int col, bool selected); // функция установки выделения
    void setSelectedByPos(int x, int y, bool selected); // функция установки выделения
    void setSelectedItems(QTableWidgetItem *itemPressed, QTableWidgetItem *itemReleased, bool selected); // выделить диапазон ячеек

private:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
};

#endif // MY_TABLEWIDGET_H
