#ifndef CRYPTOPRO_CSP_H
#define CRYPTOPRO_CSP_H

#include <QDate>
#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QDir>
#include <QThread>

//#include "logclass.h"

#ifdef __linux__
    #define CRYPTO_PRO_DIRECTORY "/opt/cprocsp/bin/amd64/"
    #define CERTMGR "certmgr"
    #define CSPTEST "csptest"
    #define SPLITTER_NEW_LINE "\n"
#elif _WIN32
    #define CRYPTO_PRO_DIRECTORY "C:/Program Files/Crypto Pro/CSP/"
    #define CERTMGR "certmgr.exe"
    #define CSPTEST "csptest.exe"
    #define SPLITTER_NEW_LINE "\r\n"
#else

#endif

#if QT_VERSION >= 0x050f00 // версия Qt 5.15.2
    #define SPLITTER Qt::SplitBehavior(Qt::SkipEmptyParts)
#else
    #define SPLITTER QString::SkipEmptyParts
#endif

class CryptoPRO_CSP : public QObject
{
    Q_OBJECT
public:
    explicit CryptoPRO_CSP(QObject *parent = nullptr);

    void setCryptoProDirectory(const QString &value);

    // главная структура
    struct CryptoSignData
    {
        // общая информацию о подписи
        QString name = "";
        QString email = "";
        QString serial = "";
        QString subname = "";
        QString surname = "";
        QString name_and_patronymic = "";
        QDate startDate;
        QDate finishDate;

        int index = -1; // информация о подписи для создания sig файла

        QString toString()
        {
            return "email = " + this->email + ", name = " + this->name + ", serial = " + this->serial + ", subname = " + this->subname + ", surname = " + this->surname + ", name_and_patronymic = " + this->name_and_patronymic + ", date_start = " + this->startDate.toString() + ", date_end = " + this->finishDate.toString();
        }
    };

signals:

private:
    QString CryptoProDirectory;

    static int getSignIndex(QList<CryptoSignData> allSignsList, CryptoSignData searchSign); // получает подпись по e-mail

    struct s_certmgr
    {
    public:
        QString getConsoleText(QStringList options); // запустить программу с опциями
        QList<CryptoSignData> getSertifactesList(); // получить список сертификатов
        void setCryptoProDirectory(const QString &value);
    private:
        QString runfile = CERTMGR;
//        logClass log;

    };
    struct s_csptest
    {
    public:
        bool createSign(QString file, CryptoSignData sign); // создать подпись для файла
        void setCryptoProDirectory(const QString &value);
    private:
        QString runfile = CSPTEST;
//        logClass log;
    };

public:
    s_certmgr certmgr; // менеджер сертификатов
    s_csptest csptest; // отвечает за создание sig файла

private:
//    logClass log;

};

#endif // CRYPTOPRO_CSP_H
