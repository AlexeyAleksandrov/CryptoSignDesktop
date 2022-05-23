#include "logger.h"
#include <QDebug>
#include <QDate>
#include <QDirIterator>

#define START_TEXT "-------------------------------------Начало работы программы---------------------------------------"
#define FINISH_TEXT "-------------------------------------Завершение работы программы---------------------------------------"
#define LOG_FILE "log_"+(QDate::currentDate()).toString("dd_MM_yyyy")+".txt"

Logger::Logger()
{
    if(logFileName.isEmpty())
    {
        logFileName = "log.txt";
        logFileName = LOG_FILE;
    }

    bool openFlag = false;
    QFile logFile;
    logFile.setFileName(logFileName);   // задаём имя файла
    openFlag = logFile.open(QIODevice::Append); // пытаемся открыть файл на добавление
    if(!openFlag)
    {
        qDebug() << "Logger: Can't open file " + logFileName + " to append log";
        return;
    }
    else
    {
        allowWrite = true; // ставим фоаг, что запись работает
        logFile.close();

        writeToFile("\r\n" + logTime() + " ");
    }
    writed = false;
}

Logger::Logger(bool printTextToConsole)
{
    this->printText = printTextToConsole;
    Logger();
}

Logger::~Logger()
{
    if(!endText.isEmpty())
    {
        writeToFile("\r\n" + logTime() + " ");
        writeToFile(endText);
    }
}

Logger Logger::getStartFinishObject()
{
    Logger log;
    log.writeToFile(START_TEXT);    // при создании выводим текст создания
    log.endText = FINISH_TEXT;  // при удалении ставим текст, который нужно быть вывести
    log.autoDeleteLog();    // вызываем функцию удаления старых логов
    return log;
}

Logger Logger::operator <<(QVariant var)
{
    if(printText)
    {
        QTextStream out(stdout);

        if(!writed)  // если запись в этой строке уже была, то добавляем пробел
        {
            out << endl;
        }
        out << var.toString();
        out << reset << flush;
    }

//    qDebug() << "Запись " << var;
    writeToFile(var.toString());
    return *this;
}

//Logger Logger::operator <<(int val)
//{
//    return this->operator<<(QString::number(val));
//}

//Logger Logger::operator <<(float val)
//{
//    return this->operator<<(QString::number(val));
//}

//Logger Logger::operator <<(double val)
//{
//    return this->operator<<(QString::number(val));
//}

//Logger Logger::operator <<(bool val)
//{
//    return this->operator<<(QString::number(val));
//}

Logger Logger::operator <<(QStringList list)
{
    QString str;
    for(QString val : list)
    {
        if(!str.isEmpty())
        {
            str.append(", ");
        }
        str.append(val);
    }

    str = "QStringList(" + str;
    str.append(")");
    return this->operator<<(str);
}

QString Logger::toFormat(int number)
{
    QString str;
    if(number < 10)
    {
        str = "0" + QString::number(number);
    }
    else
    {
        str = QString::number(number);
    }
    return str;
}

QString Logger::logTime()
{
    QDate date = QDate::currentDate();
    QString year = QString::number(date.year());
    QString mounth = toFormat(date.month());
    QString day = toFormat(date.day());

    QTime time = QTime::currentTime();
    QString hour = toFormat(time.hour());
    QString minute = toFormat(time.minute());
    QString second = toFormat(time.second());

    return year + "." + mounth+ "." + day + " " + hour + ":" + minute + ":" + second;
}

void Logger::writeToFile(QString str)
{
//    QTextStream out(stdout);

    if(allowWrite)
    {
        QFile logFile;
        bool openFlag = false;
        logFile.setFileName(logFileName);   // задаём имя файла
        openFlag = logFile.open(QIODevice::Append); // пытаемся открыть файл на добавление
        if(!openFlag)
        {
            qDebug() << "Logger: Can't open file " + logFileName + " to append log";
            return;
        }

        if(str != "\r\n")
        {
            if(writed)  // если запись в этой строке уже была, то добавляем пробел
            {
                str = " " + str;
            }
            else
            {
                writed = true;
            }
        }

        logFile.write(str.toUtf8());
        logFile.close();
    }
}

void Logger::autoDeleteLog()
{
    QDirIterator ligIter(QDir::currentPath(),{"*.txt"},QDir::Files);
    while (ligIter.hasNext())
    {
        // Берем следующий файл в списке, подходящий по параметрам
        QFile logFile(ligIter.next());
        QFileInfo inf(logFile); // Эта переменная нужна для возвращения имени файла без полного пути
        QString logFileName = inf.baseName(); // Сохраняем только имя файла без расширения
        if( logFileName.startsWith("log_"))
        {
            // Если это лог-файл
            if (QDate::fromString(logFileName,"'log_'dd_MM_yyyy").daysTo(QDate::currentDate())>30)
            {
                logFile.remove();
            }
        }
    }
}
