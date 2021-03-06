#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSqlRecord>
#include <QtSql>
#include <QSqlQuery>
#include <QCloseEvent>

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Для відкриття чи редагування списку студентів
    QObject::connect(this, SIGNAL(openList(QString)), &st, SLOT(Open(QString)));
    QObject::connect(this, SIGNAL(createList()), &st, SLOT(show()));


    ui->comboBoxType->addItem("Одна відповідь");
    ui->comboBoxType->addItem("Декілька відповідей");
    ui->comboBoxType->addItem("Ввести відповідь");

    ui->pushButtonPrevious->setEnabled(false);
    ui->pushButtonDeleteQ->setEnabled(false);
    ui->pushButtonAddQ->setEnabled(true);
    ui->pushButtonSaveNewTest->setVisible(false);
    ui->pushButtonOpen->setEnabled(false);

    QObject::connect(ui->lineEditTitle,SIGNAL(textChanged(QString)), SLOT(FirstPageChanged()));
    QObject::connect(ui->lineEditAuthor,SIGNAL(textChanged(QString)), SLOT(FirstPageChanged()));
    QObject::connect(ui->spinBoxTime,SIGNAL(valueChanged(int)), SLOT(FirstPageChanged()));
    QObject::connect(ui->doubleSpinBoxPoints,SIGNAL(valueChanged(double)), SLOT(FirstPageChanged()));

    QObject::connect(ui->lineEditQuestion,SIGNAL(textChanged(QString)),this, SLOT(SecondPageChanged()));
    QObject::connect(ui->lineEditNote,SIGNAL(textChanged(QString)),this, SLOT(SecondPageChanged()));
    QObject::connect(ui->lineEditAnswer1,SIGNAL(textChanged(QString)),this, SLOT(SecondPageChanged()));
    QObject::connect(ui->lineEditAnswer2,SIGNAL(textChanged(QString)),this, SLOT(SecondPageChanged()));
    QObject::connect(ui->lineEditAnswer3,SIGNAL(textChanged(QString)),this, SLOT(SecondPageChanged()));
    QObject::connect(ui->lineEditAnswer4,SIGNAL(textChanged(QString)),this, SLOT(SecondPageChanged()));

    QObject::connect(ui->comboBoxType,SIGNAL(currentIndexChanged(int)),this, SLOT(SecondPageChanged()));

    QObject::connect(ui->comboBoxType, SIGNAL(currentIndexChanged(int)),SLOT(correctAnswersWidgetShow()));

    QObject::connect(ui->pushButtonAdd,SIGNAL(clicked(bool)), SLOT(FirstPageChanged()));
    QObject::connect(ui->pushButtonOpen,SIGNAL(clicked(bool)), SLOT(FirstPageChanged()));

    QObject::connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(statusBarMessage()));
}

MainWindow::~MainWindow()
{
    //qDebug()<<"delete mainwindow";
    delete Element;
    delete ui;
}

//Відкриваємо БД для редагування
void MainWindow::OpenDB()
{
    fileName = QFileDialog::getOpenFileName(
                this,
                tr("Open Test"),                    //title
                "С:\\",                             //default folder
                "NTest (*.ntk)" //check only database files
                );

    database = QSqlDatabase::addDatabase("QSQLITE");

    database.setDatabaseName(fileName);

    //Якщо БД не відкрилась
    if(!database.open() || fileName.isEmpty())
    {
        QMessageBox::critical(this, tr("Помилка!"), tr("Помилка відкриття БД!"));
    }
    else
    {
            //Покажемо першу форму
            this->show();
            ui->tabWidget->setCurrentIndex(0);
            editMode = true;

            QSqlQuery query;

            //Наповнюємо масив, який складається з питань
            query.exec("SELECT id FROM Question");

            while (query.next())
            {
                SIZE = query.value(0).toInt();
            }

            query.exec("SELECT * FROM Question");

            for (int i = 0; i<=SIZE; i++)
            {
                query.next();

                Element = new OneQuestion;

                Element->id = query.value(0).toInt();

                Element->question = query.value(1).toString();
                Element->note = query.value(2).toString();
                Element->type = query.value(3).toInt();

                Element->answer1 = query.value(4).toString();
                Element->answer2 = query.value(5).toString();
                Element->answer3 = query.value(6).toString();
                Element->answer4 = query.value(7).toString();
                Element->correct = query.value(8).toString();

                vect.push_back(Element);
            }

            Element = NULL;

           //Витягнемо інформацію з БД для початкової сторінки
           query.exec("SELECT * FROM About");

           if(query.next())
           {
                title = query.value(0).toString();
                author  = query.value(1).toString();
                time = query.value(2).toInt();
                perQuestion = query.value(3).toDouble();
           }

           ID = 0;

           ui->pushButtonOpen->setEnabled(true);

           ui->lineEditTitle->setText(title);
           ui->lineEditAuthor->setText(author);
           ui->spinBoxTime->setValue(time);
           ui->doubleSpinBoxPoints->setValue(perQuestion);

           ToUi();
    }
}

//Створюємо БД для внесення питань
void MainWindow::SaveAsDB()
{
    fileName =  QFileDialog::getSaveFileName(
                this,
                tr("Create test"),                   //title
                "С:\\",                             //default folder
                "NTest (*.ntk)"            //check only database files
                );

    database = QSqlDatabase::addDatabase("QSQLITE");

    database.setDatabaseName(fileName);

    if(!database.open() || fileName.isEmpty())
    {
        QMessageBox::critical(this, tr("Помилка!"), tr("Помилка створення БД!"));
    }
    else
    {
            this->show();
            editMode = false;
            SIZE=0;

      QSqlQuery query;

      QString AboutTable = "CREATE TABLE About ("
                       "Title TEXT NOT NULL, "
                       "Author TEXT NOT NULL, "
                       "Time INTEGER NOT NULL, "
                       "PerQuestion REAL NOT NULL, "
                       "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE"
                       ");";

               if (!query.exec(AboutTable))
                  QMessageBox::critical(this, tr("Помилка!"), query.lastError().text());

      QString QuestionTable= "CREATE TABLE Question ("
                       "id	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
                       "text	TEXT NOT NULL,"
                       "note	TEXT,"
                       "type	INTEGER NOT NULL,"
                       "answer1	TEXT,"
                       "answer2	TEXT,"
                       "answer3	TEXT,"
                       "answer4	TEXT,"
                       "Correct	TEXT NOT NULL"
                   ");";

               if (!query.exec(QuestionTable))
                   QMessageBox::critical(this, tr("Помилка!"), query.lastError().text());

      QString StudentsTable= "CREATE TABLE Student ("
                       "id	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,"
                       "surname	TEXT NOT NULL,"
                       "name	TEXT NOT NULL,"
                       "password	INTEGER NOT NULL UNIQUE"
                   ");";

               if (!query.exec(StudentsTable))
                   QMessageBox::critical(this, tr("Помилка!"), query.lastError().text());

            ui->tabWidget->setCurrentIndex(0);

            ID = 0;
            ui->pushButtonNext->setEnabled(false);
            ui->pushButtonAddQ->setVisible(true);
            ui->pushButtonDeleteQ->setVisible(true);

            correctAnswersWidgetShow();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("Завершити?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int res = msgBox.exec();

    if (res == QMessageBox::No)
    {
        event->ignore();
    }
    else
    {
        qDebug()<<"from close event";
        database.close();
        title = "";
        time = 0;
        author = "";
        perQuestion  = 0.0;
        ui->lineEditTitle->clear();
        ui->lineEditAuthor->clear();
        ui->spinBoxTime->clear();
        ui->doubleSpinBoxPoints->clear();

        SIZE = 0;
        vect.clear();
        StudentVect.clear();

        ui->pushButtonPrevious->setEnabled(false);

        ui->pushButtonDeleteQ->setEnabled(false);

        ui->pushButtonSaveNewTest->setVisible(false);

        cleanWidgets();

        event->accept();
    }
}

void MainWindow::ToUi()
{
    statusBarMessage();

    if(editMode)
    {
        ui->pushButtonAddQ->setVisible(false);
        ui->pushButtonDeleteQ->setVisible(false);
    }
    else
    {
        ui->pushButtonAddQ->setVisible(true);
        ui->pushButtonDeleteQ->setVisible(true);
    }

    //Перевірка на перше запитання
    if(ID == 0)
    {
          ui->pushButtonPrevious->setEnabled(false);
          ui->pushButtonDeleteQ->setEnabled(false);
    }
    else
    {
        ui->pushButtonPrevious->setEnabled(true);
        ui->pushButtonSaveNewTest->setVisible(true);
    }

    if(ID<SIZE)
    {
        ui->pushButtonDeleteQ->setEnabled(false);
        ui->pushButtonAddQ->setEnabled(false);
        ui->pushButtonNext->setEnabled(true);
    }
    else
    {
        ui->pushButtonDeleteQ->setEnabled(true);
        ui->pushButtonAddQ->setEnabled(true);
        ui->pushButtonNext->setEnabled(false);
    }

    //очищення попередніх дій
    cleanWidgets();

        Element = vect[ID];

        //Перевірка чи не останнє запитання
        if(ID==SIZE)
            ui->pushButtonNext->setEnabled(false);
        else
            ui->pushButtonNext->setEnabled(true);

//------Заповнення віджетів
        //Отримаємо правильну відповідь
        QString tempCorrectAnswer = Element->correct;

        switch (Element->type)
        {
        case 1:
               {
                    ui->comboBoxType->setCurrentIndex(0);

                    if(tempCorrectAnswer[0] == "1") ui->radioButton->setChecked(true);
                   else
                    if(tempCorrectAnswer[1] == "1") ui->radioButton_2->setChecked(true);
                   else
                    if(tempCorrectAnswer[2] == "1") ui->radioButton_3->setChecked(true);
                   else
                    if(tempCorrectAnswer[3] == "1") ui->radioButton_4->setChecked(true);
                   break;
               }

        case 2:
               {
                    ui->comboBoxType->setCurrentIndex(1);

                   if(tempCorrectAnswer[0] == "1") ui->checkBox->setChecked(true);

                   if(tempCorrectAnswer[1] == "1") ui->checkBox_2->setChecked(true);

                   if(tempCorrectAnswer[2] == "1") ui->checkBox_3->setChecked(true);

                   if(tempCorrectAnswer[3] == "1") ui->checkBox_4->setChecked(true);
                  break;
               }

        case 3:
               {
                   ui->comboBoxType->setCurrentIndex(2);

                   ui->lineEditCorrectAnswer->setText(Element->correct);
               }
        default:
            break;
        }

            correctAnswersWidgetShow();

            ui->lineEditQuestion->setText(Element->question);
            ui->lineEditNote->setText(Element->note);

            ui->lineEditAnswer1->setText(Element->answer1);
            ui->lineEditAnswer2->setText(Element->answer2);
            ui->lineEditAnswer3->setText(Element->answer3);
            ui->lineEditAnswer4->setText(Element->answer4);
}

void MainWindow::saveIntoMass (int n)
{
    //Зберігання даних з віджетів у масив
    if(ui->tabWidget->currentIndex()==0)
    {
        title = ui->lineEditTitle->text();
        time = ui->spinBoxTime->value();
        author = ui->lineEditAuthor->text();
        perQuestion = ui->doubleSpinBoxPoints->value();
    }
    else
    {
                Element = new OneQuestion;

                Element->id = ID;

                Element->question = ui->lineEditQuestion->text();

                Element->note = ui->lineEditNote->text();
                Element->type = ui->comboBoxType->currentIndex()+1;

                Element->answer1 = ui->lineEditAnswer1->text();
                Element->answer2 = ui->lineEditAnswer2->text();
                Element->answer3 = ui->lineEditAnswer3->text();
                Element->answer4 = ui->lineEditAnswer4->text();

                Element->correct = ui->lineEditCorrectAnswer->text();

                QString tempCorrectAnswer = "0000";

                switch (Element->type ) {
                case 1:
                       {
                            if(ui->radioButton->isChecked())   tempCorrectAnswer[0] = '1';
                           else
                            if(ui->radioButton_2->isChecked()) tempCorrectAnswer[1] = '1';
                           else
                            if(ui->radioButton_3->isChecked()) tempCorrectAnswer[2] = '1';
                           else
                            if(ui->radioButton_4->isChecked()) tempCorrectAnswer[3] = '1';
                           break;
                       }

                case 2:
                       {
                            if(ui->checkBox->isChecked())   tempCorrectAnswer[0] = '1';

                            if(ui->checkBox_2->isChecked()) tempCorrectAnswer[1] = '1';

                            if(ui->checkBox_3->isChecked()) tempCorrectAnswer[2] = '1';

                            if(ui->checkBox_4->isChecked()) tempCorrectAnswer[3] = '1';
                           break;
                       }

                case 3:
                       {
                            tempCorrectAnswer = ui->lineEditCorrectAnswer->text();
                       }
                default:
                    break;
                }

                Element->correct = tempCorrectAnswer.toLower();

                                if(editMode) vect[n] = Element;
                                else
                                {
                                    vect.push_back(Element);
                                    qDebug()<<"push back new element";
                                }

//                if(!addNew) vect[n] = Element;
//                else
//                {
//                    vect.push_back(Element);
//                    qDebug()<<"push back new element";
//                    addNew = false;
//                }
    }
}

void MainWindow::UpdateDB()
{
//    if(editModePLUS)
//    {
//        ClearDB();
//        SaveDB();
//    }
  //  else
    {
        QSqlQuery query(fileName);
        //Оновлення основної інформації у БД
        query.prepare("UPDATE About SET Title='"+title+"', Time='"+QString::number(time)+"', Author='"+author+"'"
                       ", PerQuestion='"+QString::number(perQuestion)+"'"
                       "WHERE id = '1'");

        if(query.exec())
        {
            QMessageBox msgb;
            msgb.setText("Дані оновлено");
            msgb.exec();
        }
        else
                QMessageBox::critical(this, tr("Помилка оновлення даних"), query.lastError().text());


        //Оновлення запитань у БД
        for (int i=0; i!=vect.size(); i++)
        {
            Element = new OneQuestion;
            Element = vect[i];

            if(Element->type ==3)
            {
                query.prepare("UPDATE Question SET text='"+Element->question+"', "
                               "note='"+Element->note+"', "
                               "type='"+QString::number(Element->type)+"', "
                               "answer1=" ","
                               "answer2=" ","
                               "answer3=" ","
                               "answer4=" ","
                               "Correct='"+Element->correct+"'"
                               " WHERE id ='"+QString::number(i)+"'");
            }
            else
            {
                query.prepare("UPDATE Question SET text='"+Element->question+"', "
                               "note='"+Element->note+"', "
                               "type='"+QString::number(Element->type)+"', "
                               "answer1='"+Element->answer1+"',"
                               "answer2='"+Element->answer2+"', "
                               "answer3='"+Element->answer3+"',"
                               "answer4='"+Element->answer4+"',"
                               "Correct='"+Element->correct+"'"
                               " WHERE id ='"+QString::number(i)+"'");
            }
            if(!query.exec())
                     QMessageBox::critical(this, tr("Помилка!"),
                                           query.lastError().text()+"\nПомиилка запитання №"+QString::number(i)+" ");
        }
    }
}

void MainWindow::SaveDB()
{
    QSqlQuery query;

    query.exec("SELECT id FROM About");

    int s=0;
    while (query.next())
    {
        s = query.value(0).toInt();
    }
    qDebug()<<"s in saveDB =            - "<<s;

    if(s==1)
    {
        query.prepare("UPDATE About SET Title='"+title+"', Time='"+QString::number(time)+"', Author='"+author+"'"
                       ", PerQuestion='"+QString::number(perQuestion)+"'"
                       "WHERE id = '1'");

        if(query.exec())
        {
            QMessageBox msgb;
            msgb.setText("Дані оновлено");
            msgb.exec();
        }
    }
    else
    {
        //Додавання основної інформації у БД
        query.prepare("INSERT INTO About (Title, Author, Time, PerQuestion) "
                      "VALUES (?, ?, ?, ?)");
        query.addBindValue(title);
        query.addBindValue(author);
        query.addBindValue(time);
        query.addBindValue(perQuestion);
    if(query.exec())
    {
        QMessageBox msgBox;
        msgBox.setText("Дані збережено");
        msgBox.exec();
    }
    else
             QMessageBox::critical(this, tr("Помилка збереження даних: "), query.lastError().text());
    }


    query.prepare("DELETE FROM Question");
    query.exec();
    query.prepare("VACUUM");
    query.exec();

    //Оновлення запитань у БД
    for (int i = 0; i!=SIZE; i++)
    {
        Element  = new OneQuestion;
        Element = vect[i];

        //qDebug()<<" in SAVE BD - quest = "<<Element->question;

        query.prepare("INSERT INTO Question (id, text, note, type, answer1, answer2, answer3, answer4, Correct)"
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");

        query.addBindValue(Element->id);
        query.addBindValue(Element->question);
        query.addBindValue(Element->note);
        query.addBindValue(Element->type);

        query.addBindValue(Element->answer1);
        query.addBindValue(Element->answer2);
        query.addBindValue(Element->answer3);
        query.addBindValue(Element->answer4);

        query.addBindValue(Element->correct);

        if(!query.exec())
                 QMessageBox::critical(this, tr("Error2"),
                                       query.lastError().text()+"\nПомиилка (запитання №"+QString::number(i)+" ");
                                       //query.lastError().text());
    }
}

void MainWindow::ClearDB()
{
    //qDebug()<<" in ClearDB";
    QSqlQuery query;
    //Очищення основної інформації у БД
    query.prepare("DELETE FROM About");
    query.exec();
    query.prepare("VACUUM");

    if(query.exec())
            QMessageBox::critical(this, tr("Чудово!"), tr("БД очищено!"));
    else
            QMessageBox::critical(this, tr("Помилка очищення даних"), query.lastError().text());

    //Очищення запитань у БД
    query.prepare("DELETE FROM Question");
    query.exec();
    query.prepare("VACUUM");
        if(!query.exec())
            QMessageBox::critical(this, tr("Помилка очищення запитань"), query.lastError().text());
}

void MainWindow::on_pushButtonNext_clicked()
{   
    //qDebug()<<"----------------------------------NExt click";
    if(checkLineEdit())
    {
                    //Якщо ще є запитання
                    if(ID<=SIZE)
                    {
                        saveIntoMass (ID);
                        ID++;
                        ToUi();
                    }
                    else
                    {
                        if(editMode)
                        UpdateDB();
                    }
     }
}

void MainWindow::on_pushButtonPrevious_clicked()
{
    //Якщо ще є попередні запитання
    if(ID)
    {
            saveIntoMass (ID);
            ID--;
            ToUi();
     }
}

void MainWindow::on_pushButtonSaveFirstPage_clicked()
{
    if(checkLineEdit())
    {
        saveIntoMass (ID);
        ui->pushButtonSaveFirstPage->setEnabled(false);
        ui->pushButtonSaveFirstPage->setText("Збережено");
        if(editMode) UpdateDB();
        else SaveDB();
        ui->statusBar->showMessage(fileName+ " | Дані  збережено");
    }

    //запис у БД

    if(!StudentVect.empty())
    {
                 QSqlQuery q(database);
                 q.prepare("DELETE FROM Student");
                 q.exec();
                 q.prepare("VACUUM");
                 q.exec();

                 for(int i = 0; i<StudentVect.size(); i++)
                  {
                      TempStudent = new Student;
                      TempStudent = StudentVect[i];

                     // qDebug()<<TempStudent->id;
                      //qDebug()<<TempStudent->surname;
                      //qDebug()<<TempStudent->name;
                      //qDebug()<<TempStudent->password;

                      q.prepare("INSERT INTO Student (id, surname, name, password)"
                                    "VALUES (?, ?, ?, ?)");
                      q.addBindValue(TempStudent->id);
                      q.addBindValue(TempStudent->surname);
                      q.addBindValue(TempStudent->name);
                      q.addBindValue(TempStudent->password);

                      if(!q.exec())
                          ui->statusBar->showMessage(fileName + " | "+ q.lastError().text());
                      else
                      ui->statusBar->showMessage(fileName+ " | Завантаження списку студенті...["+QString::number(i)+"]");

                     // delete TempStudent;
                    }

     }

    QSqlQuery q(database);

    q.exec("SELECT id FROM Student");

    int s=-1;
    while (q.next())
    {
        s = q.value(0).toInt();
        //qDebug()<<"s ="<<s;
    }
    //qDebug()<<"s =            - "<<s;

    if(s==-1)
    {
        QMessageBox::critical(this, tr("От халепа!"), tr("Список студентів порожній"));
        ui->statusBar->showMessage(fileName+ " | Список студентів порожній");
    }
    else
    {
        ui->statusBar->showMessage(fileName+ " | Список завантажено");
        ui->pushButtonOpen->setEnabled(true);
    }
    s=-1;

}

void MainWindow::FirstPageChanged()
{
    ui->pushButtonSaveFirstPage->setEnabled(true);
    ui->pushButtonSaveFirstPage->setText("Зберегти");
}

void MainWindow::SecondPageChanged()
{
    ui->pushButtonSaveNewTest->setEnabled(true);
    ui->pushButtonSaveNewTest->setText("Зберегти");
}

bool MainWindow::checkLineEdit()
{
    bool itsOk = false;

    if(ui->tabWidget->currentIndex()==0)
    {
        if((!ui->lineEditTitle->text().isEmpty() && !ui->lineEditAuthor->text().isEmpty())
                &&
            (ui->spinBoxTime->text().toInt()!=0 && ui->doubleSpinBoxPoints->text().toStdString()!="0,00"))
          {
            itsOk = true;
          }
        else
        {
            QMessageBox::critical(this, tr("Помилка!"), tr("Заповніть усі поля!"));
            return false;
        }
    }
    else
    {
       if(ui->lineEditQuestion->text().isEmpty())
       {
           QMessageBox::critical(this, tr("Помилка!"), tr("Заповніть поле \"Запитання\""));
           return false;
       }

       if(ui->comboBoxType->currentIndex()!=2)
       {
            if (ui->lineEditAnswer1->text().isEmpty() || ui->lineEditAnswer2->text().isEmpty() ||
                   ui->lineEditAnswer3->text().isEmpty() || ui->lineEditAnswer4->text().isEmpty())
           {
                QMessageBox::critical(this, tr("Помилка!"), tr("Заповніть усі варіанти відповіді!"));
                return false;
           }
       }

       switch (ui->comboBoxType->currentIndex()) {
       case 0:
       {
            if(ui->radioButton->isChecked())   itsOk = true;
           else
            if(ui->radioButton_2->isChecked()) itsOk = true;
           else
            if(ui->radioButton_3->isChecked()) itsOk = true;
           else
            if(ui->radioButton_4->isChecked()) itsOk = true;
            else
            itsOk = false;
            break;
       }
       case 1:
       {
            if(ui->checkBox->isChecked())   itsOk = true;
           else
            if(ui->checkBox_2->isChecked()) itsOk = true;
           else
            if(ui->checkBox_3->isChecked()) itsOk = true;
           else
            if(ui->checkBox_4->isChecked()) itsOk = true;
            else
                itsOk = false;
            break;
       }
       case 2:
       {
            if (!ui->lineEditCorrectAnswer->text().isEmpty())
                itsOk = true;
            break;
       }
       default:
           break;
       }
    }

    if(itsOk) return true;
    else
    {
        QMessageBox::critical(this, tr("Помилка!"), tr("Потрібно додати відповідь до запитання!"));
        return false;
    }
}

void MainWindow::on_pushButtonSaveNewTest_clicked()
{
    if(checkLineEdit())
    {
        saveIntoMass (ID);
        ui->pushButtonSaveNewTest->setEnabled(false);
        ui->pushButtonSaveNewTest->setText("Збережено");
        if(editMode)
            UpdateDB();
            else
        {
            saveIntoMass (ID);

            SaveDB();
        }
        //qDebug()<<"End!";
    }
}

void MainWindow::correctAnswersWidgetShow()
{
    ui->lineEditAnswer1->setEnabled(true);
    ui->label_8->setEnabled(true);
    ui->lineEditAnswer2->setEnabled(true);
    ui->label_9->setEnabled(true);
    ui->lineEditAnswer3->setEnabled(true);
    ui->label_10->setEnabled(true);
    ui->lineEditAnswer4->setEnabled(true);
    ui->label_11->setEnabled(true);

    switch (ui->comboBoxType->currentIndex())
    {
    case 0:
        ui->stackedWidget->setCurrentIndex(0);
        break;
    case 1:
        ui->stackedWidget->setCurrentIndex(1);
        break;
    case 2:
    {
        ui->stackedWidget->setCurrentIndex(2);
        ui->lineEditAnswer1->setEnabled(false);
        ui->label_8->setEnabled(false);
        ui->lineEditAnswer2->setEnabled(false);
        ui->label_9->setEnabled(false);
        ui->lineEditAnswer3->setEnabled(false);
        ui->label_10->setEnabled(false);
        ui->lineEditAnswer4->setEnabled(false);
        ui->label_11->setEnabled(false);
        break;
    }
    default:
        break;
    }
}

void MainWindow::cleanWidgets()
{
    //qDebug()<<"In clean widgets";
        ui->lineEditQuestion->clear();
        ui->lineEditNote->clear();

        ui->lineEditAnswer1->clear();
        ui->lineEditAnswer2->clear();
        ui->lineEditAnswer3->clear();
        ui->lineEditAnswer4->clear();

        ui->comboBoxType->setCurrentIndex(0);

        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(false);
        ui->radioButton_3->setChecked(false);
        ui->radioButton_4->setChecked(false);

        ui->checkBox->setChecked(false);
        ui->checkBox_2->setChecked(false);
        ui->checkBox_3->setChecked(false);
        ui->checkBox_4->setChecked(false);

        ui->lineEditCorrectAnswer->clear();
}

void MainWindow::on_pushButtonAddQ_clicked()
{
    if(checkLineEdit())
    {
           // qDebug()<<"in UI (create mode)";
           // addNew = true;

            //if()
            saveIntoMass (ID);

            cleanWidgets();

            SIZE++;
            ID = SIZE;

            statusBarMessage();

            ui->pushButtonPrevious->setEnabled(true);
            ui->pushButtonSaveNewTest->setVisible(true);
            ui->pushButtonDeleteQ->setEnabled(true);

            //editModePLUS = true;
        }
}

void MainWindow::on_pushButtonDeleteQ_clicked()
{
     ID--;
     SIZE--;
     vect.pop_back();
    // editModePLUS = true;
     ToUi();
}

void MainWindow::statusBarMessage()
{
    //Нумерація запитання у СтатусБар
    //Нумеруємо лише коли активна сторінка з питаннями
    if(ui->tabWidget->currentIndex()==1)
    {
       if(editMode)
        ui->statusBar->showMessage("Запитання: "+QString::number(ID+1)+"/"+QString::number(SIZE+1));
        else ui->statusBar->showMessage("Запитання: "+QString::number(ID+1)+"/"+QString::number(SIZE));
    }
    else
        ui->statusBar->showMessage(fileName);

}

void MainWindow::on_pushButtonOpen_clicked()
{
    //qDebug()<<"fileName = "<<fileName;
    //if(editMode)
        emit openList(fileName);
    createList();
}

void MainWindow::on_pushButtonAdd_clicked()
{
    if(!StudentVect.empty())
    {
        StudentVect.clear();
    }

    QString tempFileName = QFileDialog::getOpenFileName(
                this,
                tr("Open List"),                    //title
                "С:\\",                             //default folder
                "Student list (*.ntkList)" //check only database files
                );

    QSqlDatabase Tempdatabase = QSqlDatabase::addDatabase("QSQLITE", "Students");

    Tempdatabase.setDatabaseName(tempFileName);

    //Якщо БД не відкрилась
    if(!Tempdatabase.open() || tempFileName.isEmpty())
    {
        QMessageBox::critical(this, tr("Помилка!"), tr("Помилка відкриття таблиці!"));
        return;
    }
    else
    {
        QSqlQuery q("Students", Tempdatabase);

//        if(!q.exec("SELECT id FROM Student"))
//            qDebug()<<q.lastError();

//        while (q.next())
//        {
//            SIZEt = q.value(0).toInt();
//            qDebug()<<q.value(0).toInt();
//            qDebug()<<q.value(3).toInt();
//        }

//        //врахуємо 0 елемент
//        SIZEt=SIZEt+1;
//        qDebug()<<"size = "<<SIZEt;

//        if(SIZEt<0 || SIZEt > 999)
//        {
//            QMessageBox::critical(this, tr("Помилка!"), tr("Таблиця порожня!"));
//            return;
//        }

        q.exec("SELECT * FROM Student");

        while(q.next())
        {
            TempStudent = new Student;

            TempStudent->id = q.value(0).toInt();
            TempStudent->surname = q.value(1).toString();
            TempStudent->name = q.value(2).toString();
            TempStudent->password = q.value(3).toInt();

            //qDebug()<<TempStudent->id;
           // qDebug()<<TempStudent->password;

            if(TempStudent->surname=="" || TempStudent->password == 0)
            {
                QMessageBox::critical(this, tr("Помилка!"), tr("Таблиця містить порожні поля. Заповніть за допомогою редактора."));
                return;
            }
            else
            {
                StudentVect.push_back(TempStudent);
                //qDebug()<<"pushback student";
            }
        }

        Tempdatabase.close();
    }
}
