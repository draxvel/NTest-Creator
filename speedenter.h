#ifndef SPEEDENTER_H
#define SPEEDENTER_H

#include <QDialog>
#include "mainwindow.h"
#include "students.h"
#include "result.h"
#include "about.h"

namespace Ui {
class SpeedEnter;
}

class SpeedEnter : public QDialog
{
    Q_OBJECT

public:
    explicit SpeedEnter(QWidget *parent = 0);
    ~SpeedEnter();

signals:
    createTest();
    editTest();
    students();

private slots:
    void on_pushButtonCreateTest_clicked();

    void on_pushButtonEditTest_clicked();

    void on_pushButtonStudents_clicked();

    void on_pushButtonAbout_clicked();

    void on_pushButtonStudents_3_clicked();

private:
    Ui::SpeedEnter *ui;

    MainWindow w;
    Result r;
    Students st;
    About a;
};

#endif // SPEEDENTER_H
