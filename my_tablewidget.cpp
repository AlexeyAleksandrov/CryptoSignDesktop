#include "my_tablewidget.h"

#include <QMenu>

my_tableWidget::my_tableWidget(QWidget *parent) : QTableWidget(parent)
{
    setAcceptDrops(true);
}

void my_tableWidget::mousePressEvent(QMouseEvent *m_event)
{
    m_press_position.setPosByEvent(m_event);
}

void my_tableWidget::mouseReleaseEvent(QMouseEvent *m_event)
{
    int x = m_event->x();
    int y = m_event->y();
//    qDebug() << "table: " << x << y;
    if(m_event->button() == Qt::RightButton)
    {
        QTableWidgetItem *itemReleased = itemAt(x, y);
        emit mouseRightClick(itemReleased);
    }
    else if (m_event->button() == Qt::LeftButton)
    {
        if(rowCount() == 0) // если нет строк, значит и нажимать некуда
        {
            return;
        }
        QTableWidgetItem *itemPressed = itemAt(m_press_position.x, m_press_position.y);
        QTableWidgetItem *itemReleased = itemAt(x, y);

        // если происходит тык в пустоту
        if(itemPressed == nullptr && itemReleased == nullptr && m_press_position.x == x && m_press_position.y == y)
        {
            setSelectedForAllItems(false);
        }
        else if(itemPressed == itemReleased && itemPressed != nullptr)
        {
//            qDebug() << "Ожин и тот же";
            bool selected = itemPressed->isSelected();
            setSelectedForAllItems(false);
            itemPressed->setSelected(!selected);
        }
        else // если не в пустоту
        {
            setSelectedForAllItems(false); // снимаем выдлеление со всех

            if(itemPressed && itemReleased) // если оба элемента действительные
            {
                setSelectedItems(itemPressed, itemReleased, true);
            }
            else
            {
                // получаем прямоугольник, выделенный курсором
                int start_x = m_press_position.x;
                int end_x = x;
                int start_y = m_press_position.y;
                int end_y = y;

                check(start_x, end_x); // проверяем обратный порядок следования координат и разворачиваем их, при необходимости
                check(start_y, end_y);

//                qDebug() << "X = " << start_x << end_x;
//                qDebug() << "Y = "  << start_y << end_y;

                for (int i=start_x; i<=end_x; i++)
                {
                    for (int j=start_y; j<=end_y; j++)
                    {
                        setSelectedByPos(i, j, true); // ставим значение true всем ячейкам, которые задевает курсор
                    }
                }
            }
        }
    }
}

void my_tableWidget::mouseDoubleClickEvent(QMouseEvent *m_event)
{
    if (m_event->button() == Qt::LeftButton)
    {
        int x = m_event->x();
        int y = m_event->y();
        QTableWidgetItem *itemPressed = itemAt(x, y); // получаем элемент, по которому произведено нажатие
        emit mouseDoubleClick(itemPressed); // отправляем сигнал о нажатии
    }
}

void my_tableWidget::check(int &one, int &two)
{
    if(one > two)
    {
        int temp = one;
        one = two;
        two = temp;
    }
}

void my_tableWidget::setSelectedState(int row, int col, bool selected)
{
    // получаем ячейку
    QTableWidgetItem *it = item(row, col);
    if(it == nullptr)
    {
        it = new QTableWidgetItem;
        setItem(row, col, it);
    }
    // делаем её выделенной
    it->setSelected(selected);
}

void my_tableWidget::setSelectedByPos(int x, int y, bool selected)
{
    QTableWidgetItem *it = itemAt(x, y); // получаем ячейку
    if(it != nullptr)
    {
        it->setSelected(selected); // делаем её выделенной
    }
}

void my_tableWidget::setSelectedItems(QTableWidgetItem *itemPressed, QTableWidgetItem *itemReleased, bool selected)
{
    if(itemPressed && itemReleased) // если оба элемента действительные
    {
        int startRow = itemPressed->row();
        int endRow = itemReleased->row();
        int startCol = itemPressed->column();
        int endCol = itemReleased->column();

        check(startRow, endRow);
        check(startCol, endCol);

        for (int i=startRow; i<=endRow; i++)
        {
            for (int j=startCol; j<=endCol; j++)
            {
                setSelectedState(i, j, selected);
            }
        }
    }
}

void my_tableWidget::setSelectedForAllItems(bool selected)
{
    for (int i=0; i<rowCount(); i++)
    {
        for (int j=0; j<columnCount(); j++)
        {
            setSelectedState(i, j, selected);
        }
    }
}


void my_tableWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void my_tableWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void my_tableWidget::dropEvent(QDropEvent *event)
{
    auto data = event->mimeData();
    if(data->hasUrls())
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();

        auto links = data->urls();
        QStringList files; // список файлов
        for (auto &&link : links)
        {
            QString link_str = link.toString(); // переводим в строку
            if(link_str.contains("file:///")) // если содержит файл
            {
                QString file_name = link_str.remove("file:///"); // очищаем от мусора
                files.append(file_name); // добавляем в список
            }
        }
        if(files.size() > 0)
        {
            emit dropFiles(files);
        }
    }
    else
    {
        event->ignore();
    }
}

void my_tableWidget::pressedPosition::setPosByEvent(QMouseEvent *m_event)
{
    if(m_event != nullptr)
    {
        x = m_event->x();
        y = m_event->y();
    }
}

void my_tableWidget::pressedPosition::clear()
{
    x = 0;
    y = 0;
}
