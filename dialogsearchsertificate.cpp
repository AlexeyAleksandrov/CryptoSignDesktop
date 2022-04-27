#include "dialogsearchsertificate.h"
#include "ui_dialogsearchsertificate.h"

DialogSearchSertificate::DialogSearchSertificate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSearchSertificate)
{
    ui->setupUi(this);

    connect(ui->lineEdit_email, &QLineEdit::textChanged, this, &DialogSearchSertificate::updateSearchedSerts);
    connect(ui->lineEdit_name, &QLineEdit::textChanged, this, &DialogSearchSertificate::updateSearchedSerts);
    connect(ui->lineEdit_sertificate, &QLineEdit::textChanged, this, &DialogSearchSertificate::updateSearchedSerts);
    connect(ui->lineEdit_fio, &QLineEdit::textChanged, this, &DialogSearchSertificate::updateSearchedSerts);
//    connect(ui->lineEdit_lastName, &QLineEdit::textChanged, this, &DialogSearchSertificate::updateSearchedSerts);
//    connect(ui->lineEdit_patronymic, &QLineEdit::textChanged, this, &DialogSearchSertificate::updateSearchedSerts);
}

DialogSearchSertificate::~DialogSearchSertificate()
{
    delete ui;
}

void DialogSearchSertificate::updateSearchedSerts(QString t)
{
    Q_UNUSED(t);
//    qDebug() << "Функция обновления списка" << allSertificatesList.size();
    QString name = ui->lineEdit_name->text();
//    QString firstName = ui->lineEdit_firstName->text();
//    QString lastName = ui->lineEdit_lastName->text();
//    QString patronymic = ui->lineEdit_patronymic->text();
    QString fio = ui->lineEdit_fio->text();
    QString email = ui->lineEdit_email->text();
    QString sertNumber = ui->lineEdit_sertificate->text();

    findedSerts.clear();    // сбрасываем все найденные сертификаты

    for (int i=0; i<allSertificatesList.size(); i++)
    {
        auto sert = allSertificatesList.at(i);
        bool ok = true;    // флаг корректности
        // проверяем название
        if(name != "")
        {
            if(!sert.name.toLower().contains(name.toLower()))
            {
                ok = false;
            }
        }
        // проверяем ФИО
        if(ok)
        {
            if(fio != "")
            {
                QStringList fioList = fio.split(" ", SPLITTER); // делим ФИО на куски
                for(auto fioPart : fioList) // для каждого куска
                {
                    if(!sert.subname.toLower().contains(fioPart.toLower())) // если хотя бы одно слово не содежится
                    {
                        ok = false;
                    }
                }

            }
        }
        // проверяем e-mail
        if(ok)
        {
            if(email != "")
            {
                if(!sert.email.toLower().contains(email.toLower()))
                {
                    ok = false;
                }
            }
        }
        // проверяем сертификат
        if(ok)
        {
            if(sertNumber != "")
            {
                if(!sert.serial.toLower().contains(sertNumber.toLower()))
                {
                    ok = false;
                }
            }
        }
        // добавляем в список
        if(ok)
        {
            findedSerts.append(sert);
        }
    }

    setCertificatesToTable(findedSerts);    // выводим сертификаты в таблицу
}

void DialogSearchSertificate::setAllSertificatesList(const QList<CryptoPRO_CSP::CryptoSignData> &value)
{
    allSertificatesList = value;
    setCertificatesToTable(value);
//    for (auto &&sert : allSertificatesList)
//    {
//        qDebug() << "email = " << sert.email << "name = " << sert.name << "serial = " << sert.serial << "subname = " << sert.subname << "surname = " << sert.surname << "name_and_patronymic = " << sert.name_and_patronymic << "date_start = " << sert.startDate.toString() << "date_end = " << sert.finishDate.toString() << "\n";
//    }
}

void DialogSearchSertificate::on_tableWidget_findedSerts_itemDoubleClicked(QTableWidgetItem *item)
{
   if(item)
   {
//       for (int i=0; i<allSertificatesList.size(); i++)
//       {
//            QString name = ui->tableWidget_findedSerts->item(item->row(), 0)->text();
//            QString subname = ui->tableWidget_findedSerts->item(item->row(), 1)->text();
//            QString email = ui->tableWidget_findedSerts->item(item->row(), 2)->text();
//            QString serial = ui->tableWidget_findedSerts->item(item->row(), 3)->text();

//            auto sert = allSertificatesList.at(i);
//            if(sert.name == name && sert.subname == subname && sert.email == email && sert.serial == serial)
//            {
//                currentSertificateNumber = i;
//                break;
//            }
//       }
       currentSertificateNumber = item->row();

       this->close();
   }
}

//int DialogSearchSertificate::getCurrentSertificateNumber() const
//{
//    return currentSertificateNumber;
//}

SignData DialogSearchSertificate::getChosedSertificate()
{
    if(currentSertificateNumber >= 0)
    {
        return findedSerts.at(currentSertificateNumber);
    }
    else
    {
        return SignData();
    }
}

void DialogSearchSertificate::on_pushButton_cancel_clicked()
{
    this->close();
}

void DialogSearchSertificate::on_pushButton_apply_clicked()
{
    QMessageBox::information(this, "Применить", "Для выбора сертификата нажмите двойным кликом на любой ячейке с необходимым сертификатом.");
}

void DialogSearchSertificate::setCertificatesToTable(const QList<CryptoPRO_CSP::CryptoSignData> &certificates)
{
    ui->tableWidget_findedSerts->clear();
    ui->tableWidget_findedSerts->setColumnCount(4);
    ui->tableWidget_findedSerts->setRowCount(certificates.size());
    ui->tableWidget_findedSerts->setHorizontalHeaderLabels(
                QStringList() << "Название" << "ФИО" << "Сертификат" << "email");

    for (int i=0; i<certificates.size(); i++)
    {
        // функция вставки текста в ячейку
        auto setItem = [=](int i, int j, QString text)
        {
//            // проверяем строки
//            if(i <= ui->tableWidget_findedSerts->rowCount())
//            {
//                ui->tableWidget_findedSerts->setRowCount(i+1);
//                qDebug() << "Установили новое количество строк: " << i;
//            }
//            // проверяем столбцы
//            if(j <= ui->tableWidget_findedSerts->columnCount())
//            {
//                ui->tableWidget_findedSerts->setColumnCount(j+1);
//                qDebug() << "Установили новое количество столбцов: " << j;
//            }
            // проверяем ячейку
            QTableWidgetItem *item = ui->tableWidget_findedSerts->item(i, j);
            if(item == nullptr)
            {
                item = new QTableWidgetItem();
                item->setText(text);
                ui->tableWidget_findedSerts->setItem(i, j, item);
            }
            // выводим текст
            item->setText(text);
//            qDebug() << "Выводимый текст: " << text;
        };

        auto sert = certificates.at(i);

        setItem(i, 0, sert.name);
        setItem(i, 1, sert.subname);
        setItem(i, 2, sert.serial);
        setItem(i, 3, sert.email);
//        qDebug() << "Выводим сертификат";
//        qDebug() << "email = " << sert.email << "name = " << sert.name << "serial = " << sert.serial << "subname = " << sert.subname << "surname = " << sert.surname << "name_and_patronymic = " << sert.name_and_patronymic << "date_start = " << sert.startDate.toString() << "date_end = " << sert.finishDate.toString() << "\r\n";
    }
}
