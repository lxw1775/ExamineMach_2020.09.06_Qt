#include "./UI/QHomePage.h"
#include <QtWidgets/QApplication>
#include <QDateTime>
#include <QFile>
#include <QTextStream>   
#include <QDir>
#include <QDebug>

#include <tchar.h>
#include <Windows.h>
#include <DbgHelp.h>
#pragma comment(lib, "user32.lib")

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
        txt = QString("%1 Debug <%2> [%3]: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    case QtWarningMsg:
        txt = QString("%1 Warning <%2> [%3]: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    case QtCriticalMsg:
        txt = QString("%1 Critical <%2> [%3]: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    case QtFatalMsg:
        txt = QString("%1 Fatal <%2> [%3]: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
        break;
    case QtInfoMsg:
        txt = QString("%1 Info <%2> [%3]: %4").arg(timeStr).arg(path).arg(ctx.line).arg(msg);
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




int GenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
	// 定义函数指针
	typedef BOOL(WINAPI* MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);
	// 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary(_T("DbgHelp.dll"));
	if (NULL == hDbgHelp)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (NULL == pfnMiniDumpWriteDump)
	{
		FreeLibrary(hDbgHelp);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	// 创建 dmp 文件件
	TCHAR szFileName[MAX_PATH] = { 0 };
	TCHAR szVersion[] = L"DumpFile";
	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);
	wsprintf(szFileName, L"dump/%s-%04d%02d%02d-%02d%02d%02d.dmp",
		szVersion, stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);
	HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (INVALID_HANDLE_VALUE == hDumpFile)
	{
		FreeLibrary(hDbgHelp);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	// 写入 dmp 文件
	MINIDUMP_EXCEPTION_INFORMATION expParam;
	expParam.ThreadId = GetCurrentThreadId();
	expParam.ExceptionPointers = pExceptionPointers;
	expParam.ClientPointers = FALSE;
	pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : NULL), NULL, NULL);
	// 释放文件
	CloseHandle(hDumpFile);
	FreeLibrary(hDbgHelp);
	return EXCEPTION_EXECUTE_HANDLER;
}

LONG WINAPI ExceptionFilter(LPEXCEPTION_POINTERS lpExceptionInfo)
{
	// 这里做一些异常的过滤或提示
	if (IsDebuggerPresent()) {
		return EXCEPTION_CONTINUE_SEARCH;
	}
	return GenerateMiniDump(lpExceptionInfo);
}



int main(int argc, char *argv[])
{
    QDir dir;
    if (!dir.exists(QDir::currentPath() + QString("/Log/")))
        bool ok = dir.mkpath(QDir::currentPath() + QString("/Log/"));//创建多级目录
	if (!dir.exists(QDir::currentPath() + QString("/dump/")))
		bool ok = dir.mkpath(QDir::currentPath() + QString("/dump/"));//创建多级目录

    //加入打印日志
    qInstallMessageHandler(customMessageHandler);
    qInfo() << " start. "; //输出日志

	//dump设置
	SetUnhandledExceptionFilter(ExceptionFilter);

    QApplication a(argc, argv);

	QHomePage w;
	w.show();
	return a.exec();
}
