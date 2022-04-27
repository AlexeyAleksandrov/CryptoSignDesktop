#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cryptopro_csp.h>
#include <dialogsearchsertificate.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    enum fileStatus
    {
        ADDED,
        WAITING,
        IN_PROCESS,
        ERROR_IMAGE_NO_INSERTED,
        ERROR_SIGN_NO_CREATED,
        READY
    };

private:
    void updateCertificatesList();  // функция обновления сертификатов
    CryptoPRO_CSP::CryptoSignData getCurrentSign(); // получить текущую выбранную подпись
    void setFileStatus(int i, fileStatus status);    // установить статус файлу
    QString getFileNameInPDFFormat(QString fileName);   // конвертирует название в PDF формат
    void runFile(QString file); // функция открытия файла на рабочем столе

private slots:
//    void on_pushButton_clicked();

    void on_pushButton_filesAdd_clicked();

    void on_pushButton_filesRemove_clicked();

    void on_pushButton_createSign_clicked();

    void on_comboBox_certificates_currentIndexChanged(int index);

    void on_pushButton_choseOutputDir_clicked();

    void on_toolButton_searchCertificate_clicked();

    void on_tableWidget_files_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::MainWindow *ui;

    CryptoPRO_CSP cryptoPro;
};
#endif // MAINWINDOW_H
