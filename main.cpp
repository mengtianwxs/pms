#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir dir;
    //    QString path;
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    //   设置目录为如下目录  "D:/workspace/qt/build-PMS-Desktop_Qt_5_9_7_MinGW_32bit-Debug/debug"
    //如不设置 默认目录为"D:/workspace/qt/build-PMS-Desktop_Qt_5_9_7_MinGW_32bit-Debug"
    //    path=dir.currentPath();
    //    qDebug()<<path;
    // 使用相对路径
//    QPixmap pixmap("aa.jpg");
//    QSplashScreen splash(pixmap);
//    splash.show();
//    a.processEvents();
//    Sleep(2000);
    MainWindow w;
    w.show();
//    splash.finish(&w);

    return a.exec();
}
