#include "mainwindow.h"
#include <QApplication>
#include <QtNetwork>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QStyle>
#include <QColor>
#include <QPalette>
#include <QDebug>
#include <QMessageBox>

MainWindow *win;

void windowForecast(){
    win->show();
    win->raise();
}

void windowAbout(){
    QMessageBox aboutMB(QMessageBox::NoIcon, "关于", "201941404224吴炤泉制作的天气小控件");
    aboutMB.setIconPixmap(QPixmap(":/images/0.png"));
    aboutMB.exec();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    QAction *action_forecast, *action_about, *action_quit;
    QSystemTrayIcon *systray;
    MainWindow window;
    QPalette plt(window.palette());
    plt.setColor(QPalette::WindowText,Qt::white);
    //plt.setColor(QPalette::Background,Qt::black);
    window.setPalette(plt);
    win = &window;
    // 移除最小化
    window.setWindowFlags((window.windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowMinimizeButtonHint );
    // 不在任务栏显示
    //window.setWindowFlags(Qt::Tool);
    // 窗体透明
    window.setWindowOpacity(0.8);
    //窗体背景完全透明
    window.setAttribute(Qt::WA_TranslucentBackground,true);
    window.show();

    systray = new QSystemTrayIcon();
    systray->setToolTip("天气小控件");
    systray->setIcon(QIcon(":/images/0.png"));
    systray->setVisible(true);
    QMenu *traymenu = new QMenu();
    action_forecast = new QAction("预报",traymenu);
    QStyle* style = QApplication::style();
    QIcon icon = style->standardIcon(QStyle::SP_ComputerIcon);
    action_forecast->setIcon(icon);

    action_about = new QAction("关于",traymenu);
    icon = style->standardIcon(QStyle::SP_MessageBoxInformation);
    action_about->setIcon(icon);
    action_quit=new QAction("退出",traymenu);
    icon = style->standardIcon(QStyle::SP_DialogCloseButton);
    action_quit->setIcon(icon);
    traymenu->addAction(action_forecast);
    traymenu->addAction(action_about);
    traymenu->addAction(action_quit);
    systray->setContextMenu(traymenu);
    systray->show();
    QObject::connect(action_forecast, &QAction::triggered, &windowForecast);
    QObject::connect(action_about, &QAction::triggered, &windowAbout);
    QObject::connect(action_quit, SIGNAL(triggered()), &a, SLOT(quit()));
    return a.exec();
}
