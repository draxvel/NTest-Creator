#ifndef STUDENTS_H
#define STUDENTS_H

#include <QWidget>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqldatabase>
#include <QSqlQueryModel>

namespace Ui {
class Students;
}

class Students : public QWidget
{
    Q_OBJECT

public:
    explicit Students(QWidget *parent = 0);
    ~Students();

public slots:
    void Open(QString filename);

private slots:
    void on_pushButtonCreateList_clicked();

    void on_pushButtonOpenList_clicked();


    void on_pushButtonAdd_clicked();

    void on_pushButtonDelete_clicked();

    void RefreshTable();

    void on_pushButtonUpdate_clicked();

    void closeEvent(QCloseEvent *event);

    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButtonPrint_clicked();


private:
    Ui::Students *ui;

    QString fileName;
    QSqlDatabase database;
};

#endif // STUDENTS_H
