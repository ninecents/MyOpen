#include "mydll.h"
#include <Windows.h>
#include <QIcon>
#include <QPixmap>
#include <QMediaContent>
#include <QSound>
#include <QResource>
#include <QFile>
#include <fstream>
#include <QDir>
#include <Windows.h>
#include <QTextStream>
#include <QDebug>

Mydll g_dll;

void test_export_resource()
{
    // 测试一个icon
    {
        char *res = ":/new/prefix1/logo.ico";
        QString localname = QString("D") + res;
        QPixmap icon(res);
        icon.save(localname);
    }
    return;

    // 测试一组图片
    {
        char *res[] = {
        ":/toolbar/a.png",
        ":/toolbar/b.png",
        ":/toolbar/c.png",
        ":/picture/default.bmp",
        ":/status/x.png",
        ":/status/y.gif",
        ":/status/z.png",};

        for(int i = 0; i < sizeof(res)/sizeof(char*); ++i)
        {
            break;
            QString localname = QString("D") + res[i];
            QPixmap icon(res[i]);
            icon.save(localname);
        }
    }
    return;

    // 测试音频文件（失败了）
    {
        QFile fres("qrc:/media/audio/KD1.mp3");
        QString msg = QString("khz%1---").arg(fres.exists());
        OutputDebugStringA(msg.toLatin1().data());
        QFile f("D:\\1.mp3");
        // f.write((const char*)f.data(), res2.size());
    }
    /*
    QMediaContent content("qrc:/media/audio/1.mp3");
    QMediaResourceList reslist = content.canonicalResource();
    QMediaResource res3 = reslist[0];
    std::ofstream of2("D:\\100.mp3");
    of2.write((const char*)res3.().data(), res3.dataSize());
    return;
    QResource res2(QString(":/media/audio/100.mp3"));
    std::ofstream of("D:\\100.mp3");
    of.write((const char*)res2.data(), res2.size());
    return;*/

//    QMediaContent content("qrc:/media/audio/1.mp3");
}

void MyLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    OutputDebugStringA(msg.toStdString().data());
}

void test_qdebug()
{
    qInstallMessageHandler(MyLog);
}

Mydll::Mydll()
{
    test_qdebug();
    return;
    test_export_resource();
}
