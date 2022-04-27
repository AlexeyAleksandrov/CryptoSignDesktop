#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include "documnetsigncreator.h"
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    cryptoPro.setCryptoProDirectory(CRYPTO_PRO_DIRECTORY);
    ui->tableWidget_files->setColumnCount(2);

    updateCertificatesList();

    connect(ui->tableWidget_files, &my_tableWidget::dropFiles, this, &MainWindow::addFiles);
//    connect(ui->tableWidget_files, SIGNAL(dropFiles(QStringList)), this, SLOT(addFiles(QStringList)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateCertificatesList()
{
    int currentIndex = ui->comboBox_certificates->currentIndex();

    auto signsList = cryptoPro.certmgr.getSertifactesList();
    QStringList signsData;
    for(auto sign : signsList)
    {
        QString name = sign.name;
        QString subname = sign.subname;
        if(subname == "")
        {
            subname = "- " + name;
        }
        if(name == "")
        {
            name = "-";
        }
        //        qDebug() << "name = " << name << "subname = " << subname << "nameOrSubjectState = " << nameOrSubjectState;
        int nameOrSubjectState = ui->radioButton_displayName->isChecked();
        switch (nameOrSubjectState)
        {
        case 0:
        {
            signsData.append(name);
            break;
        }
        default:
        {
            signsData.append(subname);
            break;
        }
        }
    }

    ui->comboBox_certificates->addItems(signsData);
}

CryptoPRO_CSP::CryptoSignData MainWindow::getCurrentSign()
{
    auto sertList = cryptoPro.certmgr.getSertifactesList();
    int currentIndex = ui->comboBox_certificates->currentIndex();
    if(sertList.size() > currentIndex && currentIndex >= 0)
    {
        return sertList.at(currentIndex);   // получаем текущую подпись
    }
    else
    {
        qDebug() << "Не удалось получить текущую подпись. Некооректный индекс. Всего подписей: " << sertList.size() << " индекс = " << currentIndex;
        return CryptoPRO_CSP::CryptoSignData(); // возвращаем пустой результат
    }
}

void MainWindow::setFileStatus(int i, fileStatus status)
{
    QTableWidgetItem *itemFile = ui->tableWidget_files->item(i, 0);
    QTableWidgetItem *itemStatus= ui->tableWidget_files->item(i, 1);

    if(itemFile == nullptr || itemStatus == nullptr)
    {
        qDebug() << "Невозможно установить статус файлу " << i;
        QApplication::processEvents();
        return;
    }

    QColor color;
    QString text;
    switch (status)
    {
    case ADDED:
        color = Qt::white;
        text = "Добавлен";
        break;
    case WAITING:
        color = Qt::gray;
        text = "В очереди";
        break;
    case IN_PROCESS:
        color = Qt::yellow;
        text = "В процессе";
        break;
    case ERROR_IMAGE_NO_INSERTED:
        color = Qt::red;
        text = "Ошибка конвертации";
        break;
    case ERROR_SIGN_NO_CREATED:
        color = Qt::red;
        text = "Ошибка подписи";
        break;
    case READY:
        color = Qt::green;
        text = "Готово";
        break;
    default:
        color = Qt::blue;
        text = "Неизвестная ошибка";
        break;
    }

    itemFile->setBackground(color);
    itemStatus->setBackground(color);
    itemStatus->setText(text);

    QApplication::processEvents();
    QApplication::processEvents();
    QApplication::processEvents();
}

QString MainWindow::getFileNameInPDFFormat(QString fileName)
{
    return fileName
            .replace(".docx", ".pdf")
            .replace(".doc", ".pdf")
            .replace(".rtf", ".pdf")
            .replace(".xlsx", ".pdf")
            .replace(".xls", ".pdf")
            .replace(".odt", ".pdf");
}

void MainWindow::runFile(QString file)
{
    QDesktopServices::openUrl(QUrl("file:" + file));
}


//void MainWindow::on_pushButton_clicked()
//{
//    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
//                                                    "/home",
//                                                    tr("Documents (*.docx *.doc *.rtf)"));

//    QFile file(fileName);
//    file.open(QIODevice::WriteOnly);
//    file.write("кириллица в файле");
//    file.close();

//    qDebug() << "Готово!";
//}


void MainWindow::on_pushButton_filesAdd_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Open File"),
                                                      "/home",
                                                      tr("Documents (*.docx *.doc *.rtf *.pdf *.xlsx *.xls)"));
    addFiles(files);
}


void MainWindow::on_pushButton_filesRemove_clicked()
{
    int k = 0;
    while(k < ui->tableWidget_files->rowCount())
    {
        bool isSelected = false;
        if(ui->tableWidget_files->item(k, 0)->isSelected() || ui->tableWidget_files->item(k, 1)->isSelected())
        {
            isSelected = true;
        }
        if(isSelected)
        {
            ui->tableWidget_files->removeRow(k);
        }
        else
        {
            k++;
        }
    }
}


void MainWindow::on_pushButton_createSign_clicked()
{
    int filesCount = ui->tableWidget_files->rowCount();
    QString outputDir = ui->lineEdit_outputDir->text();
    if(filesCount == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Вы не доабвили ни одного файла!");
        return;
    }

    if(ui->comboBox_certificates->count() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "В системне не установлено ни одного сертификата!");
        return;
    }

    if(outputDir == "")
    {
        QMessageBox::warning(this, "Ошибка", "Не выбрана папка вывода!");
        return;
    }

    QString jarDir = QDir::currentPath() + "/application_jar/application.jar";
    if(!QFile::exists(jarDir))
    {
        QMessageBox::warning(this, "Ошибка", "Ошибка! Файл application.jar не найден! + " + jarDir);
        return;
    }

    for(int i=0; i<filesCount; i++)
    {
        setFileStatus(i, WAITING);
    }

    CryptoPRO_CSP::CryptoSignData currentSign = getCurrentSign();

    QString owner;
    QString certificate;
    QString dateValidFrom;
    QString dateValidTo;
    bool drawLogo;
    bool checkNewPage;
    QString insertType;

    owner = ui->comboBox_certificates->currentText();   // поле владелец
    certificate = currentSign.serial;
    dateValidFrom = currentSign.startDate.toString("dd.MM.yyyy");
    dateValidTo = currentSign.finishDate.toString("dd.MM.yyyy");

    drawLogo = ui->checkBox_drawLogo->isChecked();
    checkNewPage = false;

    insertType = ui->comboBox_insertType->currentText();

    DocumnetSignCreator docSignCreator; // генератор подписей
    docSignCreator.setJarFileName(jarDir);
    docSignCreator.setSignOwner(owner);
    docSignCreator.setSignCertificate(certificate);
    docSignCreator.setSignDateFrom(dateValidFrom);
    docSignCreator.setSignDateTo(dateValidTo);
    docSignCreator.setDrawLogo(drawLogo);
    docSignCreator.setCheckTransitionToNewPage(checkNewPage);
    docSignCreator.setInsertType(insertType);

    for(int i=0; i<filesCount; i++)
    {
        setFileStatus(i, IN_PROCESS);

        QString inputFile;
        QString outputFile;

        inputFile = ui->tableWidget_files->item(i, 0)->toolTip();   // обрабатываемый файл
        outputFile = outputDir + "/" + QFileInfo(inputFile).fileName(); // файл на выходе
        outputFile = getFileNameInPDFFormat(outputFile);    // конвертируем название в PDF

        bool result = docSignCreator.processDocument(inputFile, outputFile);  // выполняем обработку
        if(!result)
        {
            setFileStatus(i, ERROR_IMAGE_NO_INSERTED);
            continue;
        }

        bool signCreated = cryptoPro.csptest.createSign(outputFile, currentSign);   // создаём подпись к файлу
        if(signCreated)
        {
            setFileStatus(i, READY);
        }
        else
        {
            setFileStatus(i, ERROR_SIGN_NO_CREATED);
        }
    }

    QMessageBox::information(this, "", "Готово!");
}


void MainWindow::on_comboBox_certificates_currentIndexChanged(int index)
{
    auto currentSign = getCurrentSign();
    ui->label_signCertificateValue->setText(currentSign.serial);
    ui->label_signNameValue->setText(currentSign.name);
    ui->label_signConfirmDatesValue->setText("c " + currentSign.startDate.toString("dd.MM.yyyy") + " до " + currentSign.finishDate.toString("dd.MM.yyyy"));
    ui->label_signOwnerValue->setText(currentSign.subname);
}


void MainWindow::on_pushButton_choseOutputDir_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(dir != "")
    {
        ui->lineEdit_outputDir->setText(dir);
    }

}


void MainWindow::on_toolButton_searchCertificate_clicked()
{
    DialogSearchSertificate dialog; // создаём диалог поиска сертификата
    dialog.setAllSertificatesList(cryptoPro.certmgr.getSertifactesList());  // задаём имеющиеся сертификаты
    dialog.exec();  //запускаем диалог и ждём выбора
    auto cert = dialog.getChosedSertificate();
    if(cert.serial != "")
    {
        auto signsList = cryptoPro.certmgr.getSertifactesList();    // получаем список сертификатов
        int index = signsList.indexOf(cert);    // получаем индекс выбранного сертификата
        ui->comboBox_certificates->setCurrentIndex(index);  // устанавливаем выбранный индекс
    }
}


void MainWindow::on_tableWidget_files_itemDoubleClicked(QTableWidgetItem *item)
{
    int row = item->row();
    QString sourceFile = ui->tableWidget_files->item(row, 0)->toolTip();    // получаем путь исходного файла
    runFile(sourceFile);
}

void MainWindow::addFiles(QStringList files)
{
    for(auto file : files)
    {
        int rows = ui->tableWidget_files->rowCount();
        // проверяем, что такого файла ещё нет
        bool contains = false;
        for(int i=0; i<rows; i++)
        {
            if(ui->tableWidget_files->item(i, 0)->toolTip() == file)
            {
                contains = true;
                break;
            }
        }
        if(contains)    // если файла содержится в списке
        {
            return;   // то просто игнорируем его
        }

        // добавляем файл в таблицу
        ui->tableWidget_files->setRowCount(rows+1);   // добавляем строку

        //        ui->tableWidget->setItem(rows, 0, new QTableWidgetItem);    // столбец файла
        ui->tableWidget_files->setItem(rows, 1, new QTableWidgetItem("Добавлен"));    // столбец статуса

        QString filename = QFileInfo(file).fileName();
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(filename);
        item->setToolTip(file); // записываем полный путь к файлу в toolTip

        ui->tableWidget_files->setItem(rows, 0, item);    // задаем ячейку
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
#ifdef SHOW_CENTRAL_GEOMERTY
    qDebug() << "Высота: " << this->height() << "Ширина: " << this->width();
#endif
#ifdef SHOW_TABLEFILES_GEOMERTY
    qDebug() << ui->tableWidget_filestatus->height() << ui->tableWidget_filestatus->width();
#endif
    if(ui->tableWidget_files->columnCount() >= 2)
    {
        int sizeMax = ui->tableWidget_files->width();
        int sizeMin = sizeMax/100;
        int size60 = sizeMin*60;
        int size40 = sizeMax - size60 - 2;
        ui->tableWidget_files->setColumnWidth(0, size60);
        ui->tableWidget_files->setColumnWidth(1, size40);
    }
}

