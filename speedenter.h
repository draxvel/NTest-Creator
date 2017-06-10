#ifndef SPEEDENTER_H
#define SPEEDENTER_H

#include <QDialog>

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
    about();

private slots:
    void on_pushButtonCreateTest_clicked();

    void on_pushButtonEditTest_clicked();

    void on_pushButtonStudents_clicked();

    void on_pushButtonAbout_clicked();

private:
    Ui::SpeedEnter *ui;
};

#endif // SPEEDENTER_H
