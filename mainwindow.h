#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cryptopro_csp.h>
#include <dialogsearchsertificate.h>
#include <my_tablewidget.h>
#include <QPaintEvent>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#define KEY_LAST_CERTIFICATE "lastCerificate"   // последний использованный сертификат
#define KEY_MAIN_CERTIFICATE "mainCertificate"  // сертификат по-умолчанию
#define KEY_DRAW_LOGO "drawLogo"    // флаг рисования герба
#define KEY_OUTPUT_DIRECTORY "outputDir"    // папка вывода
#define KEY_DISPLAY_NAME "displayName"  // флаг отображения имени владельца, вместо названия сертификата
#define KEY_SIGN_TYPE "signType"    // флаг типа подписи - откреплённая или прикреплённая
#define KEY_SIGN_FORMAT "signFormat"    // флаг формата - DER или base64

#define SIGN_TYPE_DETACHED 0
#define SIGN_TYPE_ATTACHED 1
#define SIGN_TYPE_NONE 2

#define SIGN_FORMAT_DER 0
#define SIGN_FORMAT_BASE64 1

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

    enum fileProperty
    {
        SOURCE_FILE,
        SIGN_FILE
    };

private:
    void loadProgramData();
    void saveProgramData();


private:
    void updateCertificatesList();  // функция обновления сертификатов
    CryptoPRO_CSP::CryptoSignData getCurrentSign(); // получить текущую выбранную подпись
    void setFileStatus(int i, fileStatus status);    // установить статус файлу
    QString getFileNameInPDFFormat(QString fileName);   // конвертирует название в PDF формат
    void runFile(QString file); // функция открытия файла на рабочем столе
    QString getFileProperty(int i, fileProperty fileProperty);  // получить свойство файла
    void setFileProperty(int i, fileProperty fileProperty, QString value);  // установить свойство файлу
    QString getSourceFileName(int i);   // получить путь к файлу по индексу
    QString getSignFileName(int i); // получить путь к подписанному файлу по индексу
    void setSourceFileName(int i, QString fileName);    // установить исходный файл
    void setSignFileName(int i, QString fileName);  // установить файл подписи

private slots:
//    void on_pushButton_clicked();

    void on_pushButton_filesAdd_clicked();

    void on_pushButton_filesRemove_clicked();

    void on_pushButton_createSign_clicked();

    void on_comboBox_certificates_currentIndexChanged(int index);

    void on_toolButton_searchCertificate_clicked();

    void on_tableWidget_files_itemDoubleClicked(QTableWidgetItem *item);

    void paintEvent(QPaintEvent *);

private slots:
    void addFiles(QStringList file); // слот добавления файла
    void filesTableMouseRightClick(QTableWidgetItem *item);
    void filesTableMouseDoubleClick(QTableWidgetItem *item);
    QStringList getSelectedFiles();
    QString getFileDirByIndex(int index);

    void on_toolButton_choseOutputDir_clicked();

    void on_toolButton_signInsertTypeInfo_clicked();

    void on_checkBox_drawLogo_stateChanged(int arg1);

private:
    Ui::MainWindow *ui;

    CryptoPRO_CSP cryptoPro;
    bool signProcessNeedCancel = false;

};
#endif // MAINWINDOW_H
