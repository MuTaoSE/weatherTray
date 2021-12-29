#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>
#include <QJsonParseError>
#include <QJsonObject>
#include <QVariantMap>
#include <QJsonArray>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QImage>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->page);
    mNetManager = new QNetworkAccessManager(this);
    mNetRequest = new QNetworkRequest;
    connect(mNetManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onReplyFinished(QNetworkReply*)));
    onGetWeather();
}

MainWindow::~MainWindow()
{
    delete ui;
    mNetManager->deleteLater();
    if(mNetRequest != NULL){
        delete mNetRequest;
        mNetRequest = NULL;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)//此函数在QWidget关闭时执行
{
    hide();
    //不退出App
    event->ignore();
}

//访问天气Api并捕获返回的数据
void MainWindow::onGetWeather()
{
    qDebug() << QTime::currentTime().toString();
    QString localApi = "https://devapi.qweather.com/v7/weather/7d?location=101281601&key=919a6461d3904ecdb3d7212f421ffba1";
    QString webApi = "https://devapi.qweather.com/v7/weather/7d?location=";
    QString key = "&key=919a6461d3904ecdb3d7212f421ffba1";
    if(!cityID.isEmpty())
        mNetRequest->setUrl(QUrl(webApi + cityID + key));
    else
        mNetRequest->setUrl(QUrl(localApi));

    mNetRequest->setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
    mNetManager->get(*mNetRequest);
    message = "weather7";
}

//将捕获的数据转换为字符串，便于调试
void MainWindow::onReplyFinished(QNetworkReply *reply)
{
    if(message == "weather7"){
        qDebug() << QTime::currentTime().toString();
        QByteArray weather = reply->readAll();
        if(!weather.isEmpty())
            analyWeatherXML(weather); //解析天气信息的数据
        reply->deleteLater();
    }
    else if(message == "city"){
        qDebug() << QTime::currentTime().toString();
        QByteArray city = reply->readAll();
        if(!city.isEmpty())
            analyWeatherXML(city); //解析城市的数据
        reply->deleteLater();
    }
}


//解析Api返回的数据
void MainWindow::analyWeatherXML(QByteArray json)
{
    if(message == "weather7"){
        if(json.isEmpty())
                return ;
//        qDebug() << json.data();
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(json, &err);
        QImage *img = new QImage;
        QString filename;
        QJsonArray forecast = jsonDoc.object().value("daily").toArray();
        ui->labelTime->setText(jsonDoc.object().value("updateTime").toString());
        QJsonObject day1 = forecast[0].toObject(); //今天
        filename = ":\\icon\\" + day1.value("iconDay").toString() + ".png";
        qDebug() << filename;
        img->load(filename);
        ui->img1->setPixmap(QPixmap::fromImage(img->scaled(70,70)));
        ui->textBrowserday1->clear();
        ui->textBrowserday1->append(JsonObj2String(day1));
        ui->textBrowserday1->setStyleSheet("background:transparent");
        ui->temperature->setText(day1.value("tempMin").toString() + "℃");
        ui->sunrise->setText(day1.value("sunrise").toString());
        ui->sunset->setText(day1.value("sunset").toString());
        ui->uvIndex->setText(day1.value("uvIndex").toString());
        ui->pressure->setText(day1.value("pressure").toString());
        ui->day1->setText(day1.value("fxDate").toString());
        QJsonObject day2 = forecast[1].toObject();
        filename = ":\\icon\\" + day2.value("iconDay").toString() + ".png";
        qDebug() << filename;
        img->load(filename);
        ui->img2->setPixmap(QPixmap::fromImage(img->scaled(70,70)));
        ui->textBrowserday2->clear();
        ui->textBrowserday2->append(JsonObj2String(day2));
        ui->textBrowserday2->setStyleSheet("background:transparent");
        ui->day2->setText(day2.value("fxDate").toString());
        QJsonObject day3 = forecast[2].toObject();
        filename = ":\\icon\\" + day3.value("iconDay").toString() + ".png";
        qDebug() << filename;
        img->load(filename);
        ui->img3->setPixmap(QPixmap::fromImage(img->scaled(70,70)));
        ui->textBrowserday3->clear();
        ui->textBrowserday3->append(JsonObj2String(day3));
        ui->textBrowserday3->setStyleSheet("background:transparent");
        ui->day3->setText(day3.value("fxDate").toString());
        QJsonObject day4 = forecast[3].toObject();
        filename = ":\\icon\\" + day4.value("iconDay").toString() + ".png";
        qDebug() << filename;
        img->load(filename);
        ui->img4->setPixmap(QPixmap::fromImage(img->scaled(70,70)));
        ui->textBrowserday4->clear();
        ui->textBrowserday4->append(JsonObj2String(day4));
        ui->textBrowserday4->setStyleSheet("background:transparent");
        ui->day4->setText(day4.value("fxDate").toString());
        QJsonObject day5 = forecast[4].toObject();
        filename = ":\\icon\\" + day5.value("iconDay").toString() + ".png";
        qDebug() << filename;
        img->load(filename);
        ui->img5->setPixmap(QPixmap::fromImage(img->scaled(70,70)));
        ui->textBrowserday5->clear();
        ui->textBrowserday5->append(JsonObj2String(day5));
        ui->textBrowserday5->setStyleSheet("background:transparent");
        ui->day5->setText(day5.value("fxDate").toString());
        filename = ":\\images\\01.jpg";
        qDebug() << filename;
        img->load(filename);
        ui->qvxian->setPixmap(QPixmap::fromImage(img->scaled(651,211)));
    }
    else if(message == "city"){
        if(json.isEmpty())
            return ;
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(json, &err);
        QJsonArray jsonObj = jsonDoc.object().value("location").toArray();
        QJsonObject cityMessage = jsonObj[0].toObject();
        cityID = cityMessage.value("id").toString();
        onGetWeather();
    }

}

QString MainWindow::JsonObj2String(const QJsonObject jsonObj)
{
    QString weather;

    if(!jsonObj.isEmpty())
    {
        weather += jsonObj.value("textDay").toString() + "\n";
        weather += "最低：" + jsonObj.value("tempMin").toString() + "\n";
        weather += "最高：" + jsonObj.value("tempMax").toString() + "\n";
        weather += "风向：" + jsonObj.value("windDirDay").toString() + "\n";
        weather += "风速：" + jsonObj.value("windSpeedDay").toString() + "\n";
        weather += "湿度：" + jsonObj.value("humidity").toString();
    }
    return weather;
}

void MainWindow::on_refreshButton_clicked()
{
    message = "weather7";
    onGetWeather();
}

void MainWindow::on_changeButton_clicked()
{
    QString cityApi = "https://geoapi.qweather.com/v2/city/lookup?location=";
    QString key = "&key=919a6461d3904ecdb3d7212f421ffba1";
    city = ui->cityEdit->text();
    ui->city->setText(city);
    mNetRequest->setUrl(QUrl(cityApi + city + key));
    mNetRequest->setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
    mNetManager->get(*mNetRequest);
    message = "city";
}

void MainWindow::on_cutButton_clicked()
{
    QString t = ui->cutButton->text();
    if(t == "查看变化曲线"){
        ui->cutButton->setText("查看天气信息");
        ui->label->setText("温度变化曲线");
        ui->stackedWidget->setCurrentWidget(ui->page_2);
    }
    else{
        ui->cutButton->setText("查看变化曲线");
        ui->label->setText("一周天气一览");
        ui->stackedWidget->setCurrentWidget(ui->page);
    }
}

