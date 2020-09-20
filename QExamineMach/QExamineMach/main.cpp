#include "./UI/QHomePage.h"
#include <QtWidgets/QApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>   
#include <QDir>
#include <QDebug>


void customMessageHandler(QtMsgType type, const QMessageLogContext& ctx, const QString& msg)
{
    QString txt;
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyy-MM-dd hh:mm:ss");

    QString path= ctx.file;
    int i = -1;
    if ((i = path.indexOf("QExamineMach")) != -1)
        path = path.mid(i + 13);

    switch (type)
    {
    case QtDebugMsg:
        txt = QString("%1 <%2> [%3] Debug: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("%1 <%2> [%3] Warning: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("%1 <%2> [%3] Critical: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("%1 <%2> [%3] Fatal: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    case QtInfoMsg:
        txt = QString("%1 <%2> [%3] Info: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    default:
        break;

    }
    
    QFile outFile(QDir::currentPath() + QString("/Log/") + QDateTime::currentDateTime().toString("yyyy-MM-dd") + QString(".log"));//yyyy-MM-dd-hh-mm-ss

    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt.toUtf8() << endl;
    outFile.close();
}


int main(int argc, char *argv[])
{
    QDir dir(QDir::currentPath() + QString("/Log/"));
    if (!dir.exists())
        bool ok = dir.mkpath(QDir::currentPath() + QString("/Log/"));//创建多级目录

    //加入打印日志
    qInstallMessageHandler(customMessageHandler);
    qInfo() << " start. "; //输出日志

    QApplication a(argc, argv);

	QHomePage w;
	w.show();
	return a.exec();
}
