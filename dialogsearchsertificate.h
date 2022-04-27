#ifndef DIALOGSEARCHSERTIFICATE_H
#define DIALOGSEARCHSERTIFICATE_H

#include <QDialog>
#include <cryptopro_csp.h>
#include <QTableWidgetItem>
#include <QMessageBox>

// автоматический define сплиттера, в зависимости от версии компилятора
// если не произошла проблема, исправь версию. Чтобы узнать версию используй QString::number(QT_VERSION, 16)
#if QT_VERSION >= 0x050f00 // версия Qt 5.15.0
#define SPLITTER Qt::SplitBehavior(Qt::SkipEmptyParts)
#else
#define SPLITTER QString::SkipEmptyParts
#endif

typedef CryptoPRO_CSP::CryptoSignData SignData;

namespace Ui {
class DialogSearchSertificate;
}

class DialogSearchSertificate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSearchSertificate(QWidget *parent = nullptr);
    ~DialogSearchSertificate();

    void setAllSertificatesList(const QList<CryptoPRO_CSP::CryptoSignData> &value);

//    int getCurrentSertificateNumber() const;
    SignData getChosedSertificate(); // получить выбранный сертификат

private:
    Ui::DialogSearchSertificate *ui;

public slots:
    void updateSearchedSerts(QString t = "");


private slots:
    void on_tableWidget_findedSerts_itemDoubleClicked(QTableWidgetItem *item);

    void on_pushButton_cancel_clicked();

    void on_pushButton_apply_clicked();

private:
    void setCertificatesToTable(const QList<CryptoPRO_CSP::CryptoSignData> &certificates);

private:
    QList<SignData> allSertificatesList;   // список всех сертификатов
    QList<SignData> findedSerts; // список найденных сертификатов
    int currentSertificateNumber = -1;
    SignData currentSertificate;

};

#endif // DIALOGSEARCHSERTIFICATE_H
