#ifndef DOCUMNETSIGNCREATOR_H
#define DOCUMNETSIGNCREATOR_H

#include <QProcess>
#include <QFile>
#include <QDebug>

class DocumnetSignCreator
{
public:
    DocumnetSignCreator();

    bool processDocument(QString fileInput, QString fileOutput);

    const QString &getSignCertificate() const;
    void setSignCertificate(const QString &newSignCertificate);

    const QString &getSignOwner() const;
    void setSignOwner(const QString &newSignOwner);

    const QString &getSignDateFrom() const;
    void setSignDateFrom(const QString &newSignDateFrom);

    const QString &getSignDateTo() const;
    void setSignDateTo(const QString &newSignDateTo);

    bool getDrawLogo() const;
    void setDrawLogo(bool newDrawLogo);

    bool getCheckTransitionToNewPage() const;
    void setCheckTransitionToNewPage(bool newCheckTransitionToNewPage);

    const QString &getInsertType() const;
    void setInsertType(const QString &newInsertType);

    const QString &getJarFileName() const;
    void setJarFileName(const QString &newJarFileName);

private:
    QString signOwner;
    QString signCertificate;
    QString signDateFrom;
    QString signDateTo;
    bool drawLogo = false;
    bool checkTransitionToNewPage = false;
    QString insertType;

    QString jarFileName;
};

#endif // DOCUMNETSIGNCREATOR_H
