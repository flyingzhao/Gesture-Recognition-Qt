#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QPainter>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);


    ~Widget();

private slots:
    void on_pushButton_cmopen_clicked();
    void updateAA();

    void on_pushButton_send_clicked();

    void on_pushButton_cmclose_clicked();


    void on_pushButton_clear_clicked();


private:
    Ui::Widget *ui;
    QSerialPort *my_serialport;

    QByteArray requestData;
    QTimer *timer;
    QPixmap pix2;
    QByteArray status;

    QString filename;
    char tempfile[2];
    int CurrentIndex;

    float ratio;

};

#endif // WIDGET_H
