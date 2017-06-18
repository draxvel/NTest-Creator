#ifndef RESULT_H
#define RESULT_H

#include <QWidget>

namespace Ui {
class Result;
}

class Result : public QWidget
{
    Q_OBJECT

public:
    explicit Result(QWidget *parent = 0);
    ~Result();

private slots:
    void on_pushButton_clicked();

    QString decodeStr(const QString &str);

    void closeEvent(QCloseEvent *event);

private:
    QString results;

    const  quint32 key = 073; //шифр

    Ui::Result *ui;
};

#endif // RESULT_H
