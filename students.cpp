#include "students.h"
#include "ui_students.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QTime>
#include <QTextCodec>
#include <QCloseEvent>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTextDocument>
#include <QTextStream>


Students::Students(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Students)
{
    ui->setupUi(this);

    QTextCodec *codec = QTextCodec::codecForName("UTF8");
    QTextCodec::setCodecForLocale(codec);

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setColumnWidth(0,50);
    ui->tableWidget->setColumnWidth(1,200);
    ui->tableWidget->setColumnWidth(2,200);
    ui->tableWidget->setColumnWidth(3,50);

    ui->tableWidget->verticalHeader()->setVisible(false);

    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem(tr("id")));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem(tr("Прізвище")));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem(tr("Ім'я")));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem(tr("Пароль")));
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget->clearContents();
}

Students::~Students()
{
    delete ui;
    ui->tableWidget->clearContents();
}

void Students::on_pushButtonCreateList_clicked()
{
    if(ui->tableWidget->rowCount() > 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Завершити роботу зі списком?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int res = msgBox.exec();

        if (res == QMessageBox::No)
        {
            RefreshTable();
            return;
        }
        else
        {
            ui->tableWidget->setRowCount(0);
            database.close();
            database.removeDatabase(database.connectionName());
        }
    }

    fileName =  QFileDialog::getSaveFileName(
                this,
                tr("Open List"),                    //title
                "С:\\",                             //default folder
                "Student list (*.ntkList)" //check only database files
                );

    database = QSqlDatabase::addDatabase("QSQLITE");

    database.setDatabaseName(fileName);

    if(!database.open() || fileName.isEmpty())
    {
        QMessageBox::critical(this, tr("Помилка!"), tr("Помилка створення таблиці!"));
    }
    else
    {
        QString StudentsTable= "CREATE TABLE Student ("
                       "id	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
                       "surname	TEXT NOT NULL,"
                       "name	TEXT NOT NULL,"
                       "password	INTEGER NOT NULL UNIQUE"
                   ");";

        QSqlQuery query;

               if (!query.exec(StudentsTable))
                   {
                       QMessageBox msgBox;
                       msgBox.setText("Помилка створення таблиці!");
                       msgBox.exec();
                   }

    ui->pushButtonAdd->setEnabled(true);
    ui->pushButtonDelete->setEnabled(true);
    ui->spinBox->setEnabled(true);

    ui->lineEditSurname->setEnabled(true);
    ui->lineEditName->setEnabled(true);
    }
}

void Students::on_pushButtonOpenList_clicked()
{
    if(ui->tableWidget->rowCount() > 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Завершити роботу з таблицею?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int res = msgBox.exec();

        if (res == QMessageBox::No)
        {
            RefreshTable();
            return;
        }
        else
        {
            ui->tableWidget->setRowCount(0);
            database.close();
            database.removeDatabase(database.connectionName());
        }
    }

    fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open List"),                    //title
                "С:\\",                             //default folder
                "Student list (*.ntkList)" //check only database files
                );

   Open(fileName);
}

void Students::Open(QString filename)
{
    database = QSqlDatabase::addDatabase("QSQLITE");

    database.setDatabaseName(filename);

    //Якщо БД не відкрилась
    if(!database.open() || filename.isEmpty())
    {
        QMessageBox::critical(this, tr("Помилка!"), tr("Помилка відкриття таблиці!"));
    }
    else
    {
        RefreshTable();

        ui->pushButtonAdd->setEnabled(true);
        ui->pushButtonDelete->setEnabled(true);

        ui->spinBox->setEnabled(true);

        ui->lineEditSurname->setEnabled(true);
        ui->lineEditName->setEnabled(true);
    }
}

void Students::on_pushButtonAdd_clicked()
{
      if(ui->lineEditSurname->text()=="" || ui->lineEditName->text()=="")
      {
          QMessageBox msgBox;
          msgBox.setText("Заповніть усі поля!");
          msgBox.exec();
      }
      else
      {
          QSqlQuery query;

          query.prepare("INSERT INTO Student (id, surname, name, password)"
                        "VALUES (?, ?, ?, ?)");
          query.addBindValue(ui->spinBox->value());
          query.addBindValue(ui->lineEditSurname->text());
          query.addBindValue(ui->lineEditName->text());
          int n = 999 + qrand() % 9000;
          query.addBindValue(n);

          if(!query.exec())
          {
              QMessageBox msgBox;
              msgBox.setText("Студент з таким ID вже існує");
              msgBox.exec();
          }
          else
          {
              ui->spinBox->setValue(ui->spinBox->value()+1);
              RefreshTable();
              ui->lineEditSurname->clear();
              ui->lineEditName->clear();
          }
      }
}

void Students::on_pushButtonDelete_clicked()
{
    if(ui->tableWidget->currentIndex().row() >= 0)
    {
            QSqlQuery query;
            query.prepare("DELETE FROM Student WHERE id = :id;");
            query.bindValue(":id", ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 0)->text());
            if(!query.exec())
            {
                QMessageBox msgBox;
                msgBox.setText("Помилка видалення");
                msgBox.exec();
            }

            RefreshTable();
    }
}

void Students::RefreshTable()
{
       int n = ui->tableWidget->rowCount();

        for( int i = 0; i < n; i++ ) ui->tableWidget->removeRow( 0 );

        QSqlQuery query;
        query.exec("SELECT * FROM Student;");

        while (query.next())
        {
             ui->tableWidget->insertRow(0);
             ui->tableWidget->setItem(0, 0, new QTableWidgetItem(query.value(0).toString()));
             ui->tableWidget->setItem(0, 1, new QTableWidgetItem(query.value(1).toString()));
             ui->tableWidget->setItem(0, 2, new QTableWidgetItem(query.value(2).toString()));
             ui->tableWidget->setItem(0, 3, new QTableWidgetItem(query.value(3).toString()));
             ui->tableWidget->setRowHeight(0, 20);
        }

        if(ui->tableWidget->rowCount() > 0)
        {
            ui->pushButtonUpdate->setEnabled(true);
            ui->pushButtonPrint->setEnabled(true);
        }
}

void Students::closeEvent(QCloseEvent *event)
{
        QMessageBox msgBox;
        msgBox.setText("Завершити роботу?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setDefaultButton(QMessageBox::Yes);
        int res = msgBox.exec();

        if (res == QMessageBox::No)
        {
            RefreshTable();
            event->ignore();
        }
        else
            event->accept();
}

void Students::on_pushButtonUpdate_clicked()
{
    RefreshTable();
}

void Students::on_tableWidget_cellClicked(int row, int column)
{         QSqlQuery query;
             query.prepare("UPDATE Student SET surname='"+(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 1)->text())+"', "
                            "name='"+(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 2)->text())+"', "
                            "password='"+(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 3)->text())+"'"
                            " WHERE id ='"+(ui->tableWidget->item(ui->tableWidget->currentIndex().row(), 0)->text())+"'");

             if(!query.exec())
             {
                 QMessageBox msgBox;
                 msgBox.setText("Помилка оновлення!");
                 msgBox.exec();
             }
}

void Students::on_pushButtonPrint_clicked()
{
    QString strStream;
    QTextStream out(&strStream);

     const int rowCount = ui->tableWidget->rowCount(); //контейнер для хранения строк;
     const int columnCount = ui->tableWidget->columnCount();//контейнер для хранения столбцов;

     out <<"<html>\n"<<"<head>\n"
         <<"<meta Content=\"Text/html;charset=Windows-1251\">\n"    //кодування
         <<QString("<title>%1</title>\n").arg("Список студентів")   //назва  - report
         <<  "</head>\n"
         <<"<body bgcolor = #ffffff link=#5000A0>\n"                //колір
         <<"<table border=1 cellspacing=0 cellpadding=2>\n";        //межі

         // headers
     out << "<thead><tr bgcolor=#f0f0f0>";
     for (int column = 0; column < columnCount; column++)           //йдемо по стовпцях
             if (!ui->tableWidget->isColumnHidden(column))          //якщо колонка не захована
         out << QString("<th>%1</th>").arg(ui->tableWidget->horizontalHeaderItem(column)->text());
     out << "</tr></thead>\n";

               // data table
               for (int row = 0; row < rowCount; row++)             //йдем по рядках
               {
                   out << "<tr>";
                   for (int column = 0; column < columnCount; column++)
                   {
                       if (!ui->tableWidget->isColumnHidden(column))
                       {
                           QString data = ui->tableWidget->item(row, column)->text().simplified();

                           //якщо даниї немає то нерозривний пробіл
                           out << QString("<td bkcolor=0>%1</td>").arg((!data.isEmpty()) ? data : QString("&nbsp;"));
                       }
                   }
                   out << "</tr>\n";
               }
               out <<  "</table>\n"
                       "</body>\n"
                       "</html>\n";

               QTextDocument *document = new QTextDocument();
               document->setHtml(strStream); // наповнення документа;

               QPrinter printer;

               QPrintDialog *dialog = new QPrintDialog(&printer, NULL); //виклик вікна друку
               if (dialog->exec() == QDialog::Accepted)
               {
                   document->print(&printer); //друк документа
               }

               delete document;
}
