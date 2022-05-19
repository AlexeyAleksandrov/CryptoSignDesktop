#ifndef LOGGER_H
#define LOGGER_H

#include <QString>
#include <QFile>
#include <QDebug>

class Logger
{
public:
    Logger();
    Logger(bool printTextToConsole);
    ~Logger();

    static Logger getStartFinishObject(); // специальный объект, который проверит, надичие старых логов и выведет тект начала и окончания программы

    Logger operator << (QVariant var);
//    Logger operator << (int val);
//    Logger operator << (float val);
//    Logger operator << (double val);
//    Logger operator << (bool val);
    Logger operator << (QStringList list);

    void autoDeleteLog();

private:
    QString logFileName;
    bool allowWrite = false;
    bool printText = false;
    bool writed = false; // флаг, была ли уже запись, при true подставляется пробел

    void writeToFile(QString str);

    QString toFormat(int number);
    QString logTime();

    QString endText;    // текст, который будет выведен после удаления объекта
};

#endif // LOGGER_H
