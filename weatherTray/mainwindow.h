#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<QNetworkRequest>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString city;
    QString cityID;
    QString message;

private slots:
    void on_refreshButton_clicked();

    void on_changeButton_clicked();

    void on_cutButton_clicked();

    void onReplyFinished(QNetworkReply *reply);
    void analyWeatherXML(QByteArray json);

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);
    void onGetWeather();


    QString JsonObj2String(const QJsonObject jsonObj);
    QNetworkRequest *mNetRequest;
    QNetworkAccessManager *mNetManager;



};

#endif // MAINWINDOW_H
