#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqldatabase>
#include <QSqlQuery>
#include <vector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:

    openList(QString filename);

    createList();

public slots:
    void OpenDB();
    void SaveAsDB();

    void closeEvent(QCloseEvent *event);

private slots:
    void on_pushButtonNext_clicked();

    void on_pushButtonPrevious_clicked();

    void on_pushButtonSaveFirstPage_clicked();

    void ToUi();

    void saveIntoMass(int n);

    void FirstPageChanged();

    void SecondPageChanged();

    bool checkLineEdit();

    void UpdateDB();
    void SaveDB();
    void ClearDB();


    void on_pushButtonSaveNewTest_clicked();

    void correctAnswersWidgetShow();

    void cleanWidgets();

    void on_pushButtonAddQ_clicked();

    void on_pushButtonDeleteQ_clicked();

    void statusBarMessage();

    void on_pushButtonOpen_clicked();

    void on_pushButtonAdd_clicked();

private:
    Ui::MainWindow *ui;

    struct OneQuestion
    {
        int id;
        QString question;
        QString note;
        int type;
        QString answer1;
        QString answer2;
        QString answer3;
        QString answer4;
        QString correct;
    };

    QString title;
    int time;
    QString author;
    double perQuestion;

    OneQuestion *Element = new OneQuestion;
    std::vector <OneQuestion *> vect;
   // std::vector <OneQuestion *> :: iterator iter;

//------------------------------------
    struct Temp
    {
        int id;
        QString surname;
        QString name;
        int password;
    };

    std::vector <Temp *> TempElementVect;
    Temp *TempElement = new Temp;

    int SIZEt;
//--------------------------------------

    int ID;
    int SIZE = 0;

    bool editMode;
    bool editModePLUS = false;
    bool addNew = false;

    QString fileName;
    QSqlDatabase database;
};

#endif // MAINWINDOW_H
