#include "documnetsigncreator.h"

#include <QFileInfo>
#include <QTextCodec>

DocumnetSignCreator::DocumnetSignCreator()
{

}

bool DocumnetSignCreator::processDocument(QString fileInput, QString fileOutput)
{
    if(!QFile::exists(fileInput))
    {
        qDebug() << QString::fromLocal8Bit("Файл для подписи не найден! ") + fileInput;
        log << QString::fromLocal8Bit("Файл для подписи не найден! ") + fileInput;
        return false;
    }

    if(jarFileName.isEmpty() || !QFile::exists(jarFileName))
    {
        qDebug() << QString::fromLocal8Bit("Файл запуска JAR не найден!");
        log << QString::fromLocal8Bit("Файл запуска JAR не найден!");
        return false;
    }

    if(!fileOutput.endsWith(".pdf"))
    {
        qDebug() << QString::fromLocal8Bit("Выходной файл должен быть формата *.pdf ") + fileOutput;
        log << QString::fromLocal8Bit("Выходной файл должен быть формата *.pdf ") + fileOutput;
        return false;
    }

    // готовися к запуску
    QProcess process;   // запускаемый процесс
//    process.setProgram("C:/Program Files/Java/jre1.8.0_301/bin/java.exe");    // запускаемая программа
//    process.setProgram("C:/Users/ASUS/Downloads/application_jar/application_jar/application.exe");
    process.setProgram("java");
    QStringList params; // параметры запуска

    // переводим bool в строку
    QString drawLogoStr = drawLogo ? "true" : "false";
    QString checkTransitionToNewPageStr = checkTransitionToNewPage ? "true" : "false";
    params
            << "-jar"
            << jarFileName
           << fileInput
           << fileOutput
           << signOwner
           << signCertificate
           << signDateFrom
           << signDateTo
           << drawLogoStr
           << checkTransitionToNewPageStr
           << insertType;

    qDebug() << params;

    QString workingJarDirectory = QFileInfo(jarFileName).absolutePath();
    qDebug() << "workingJarDirectory = " << workingJarDirectory;
    process.setWorkingDirectory(workingJarDirectory);
    process.setArguments(params);
    process.setReadChannel(QProcess::StandardOutput);
    process.start();
    bool started = process.waitForStarted();    // ждём запуска программы
    if(!started)
    {
        qDebug() << QString::fromLocal8Bit("Не удалось запустить программу");
        log << QString::fromLocal8Bit("Не удалось запустить программу");
        return false;
    }

    bool finished = process.waitForFinished(300000);    // время ожидания программы - 5 минут (для слабых ПК)
    if(!finished)
    {
        process.terminate();    // принудительно завершаем процесс
        qDebug() << QString::fromLocal8Bit("Превышено время ожидания программы!");
        log << QString::fromLocal8Bit("Превышено время ожидания программы!");
        return false;
    }

    int exitCode = process.exitCode();
    if(exitCode != 0)
    {
        qDebug() << QString::fromLocal8Bit("Процесс завершился с кодом ") << exitCode;
        log << QString::fromLocal8Bit("Процесс завершился с кодом ") << exitCode;

        QString text = process.readAllStandardOutput();

        QString lineSplitter;
#ifdef __linux__
        lineSplitter = "\n";
#elif _WIN32
        lineSplitter = "\r\n";
#endif

        QStringList error_list = text.split(lineSplitter, QString::SkipEmptyParts);
        qDebug() << "run_error_text = \r\n";  // выводим ошибку
        for(auto &&line : error_list)
        {
            qDebug() << line;
            log << line;
        }

        return false;
    }

    if(QFile::exists(fileOutput))   // если файл в итоге создан
    {
        return true;
    }
    else
    {
        return false;
    }
}

const QString &DocumnetSignCreator::getSignCertificate() const
{
    return signCertificate;
}

void DocumnetSignCreator::setSignCertificate(const QString &newSignCertificate)
{
    signCertificate = newSignCertificate;
}

const QString &DocumnetSignCreator::getSignOwner() const
{
    return signOwner;
}

void DocumnetSignCreator::setSignOwner(const QString &newSignOwner)
{
    signOwner = newSignOwner;
}

const QString &DocumnetSignCreator::getSignDateFrom() const
{
    return signDateFrom;
}

void DocumnetSignCreator::setSignDateFrom(const QString &newSignDateFrom)
{
    signDateFrom = newSignDateFrom;
}

const QString &DocumnetSignCreator::getSignDateTo() const
{
    return signDateTo;
}

void DocumnetSignCreator::setSignDateTo(const QString &newSignDateTo)
{
    signDateTo = newSignDateTo;
}

bool DocumnetSignCreator::getDrawLogo() const
{
    return drawLogo;
}

void DocumnetSignCreator::setDrawLogo(bool newDrawLogo)
{
    drawLogo = newDrawLogo;
}

bool DocumnetSignCreator::getCheckTransitionToNewPage() const
{
    return checkTransitionToNewPage;
}

void DocumnetSignCreator::setCheckTransitionToNewPage(bool newCheckTransitionToNewPage)
{
    checkTransitionToNewPage = newCheckTransitionToNewPage;
}

const QString &DocumnetSignCreator::getInsertType() const
{
    return insertType;
}

void DocumnetSignCreator::setInsertType(const QString &newInsertType)
{
    insertType = newInsertType;
}

const QString &DocumnetSignCreator::getJarFileName() const
{
    return jarFileName;
}

void DocumnetSignCreator::setJarFileName(const QString &newJarFileName)
{
    jarFileName = newJarFileName;
}
