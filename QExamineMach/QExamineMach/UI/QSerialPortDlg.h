#pragma once
#include <QDialog>
#include <QSerialPort>
#include <QMutex>
/*
	Qt 串口通信测试窗口
	参考：https://blog.csdn.net/weixin_42837024/article/details/81669540

*/


#include <QWidget>
#include "ui_QSerialPortDlg.h"

class QSerialPortDlg : public QDialog
{
	Q_OBJECT

public:
	QSerialPortDlg(QWidget *parent = Q_NULLPTR);
	~QSerialPortDlg();

private:
	//写两个函数 向单片机发送数据 
	void sendInfo(char* info, int len);
	void sendInfo(QString& info);
	void DataAnaly();
private slots:
	//获取端口列表
	void OnGetPortNameList();
	//打开端口
	void OnOpenPort();
	//接收到单片机发送的数据进行解析
	void OnDataReceive();
	//发送数据
	void OnSend();
	//响应
	void OnDataRespone(QByteArray b);
private:
signals:
	void sgnRespone(QByteArray b);
	void sgnCmd();

private:
	Ui::QSerialPortDlg ui;

	QSerialPort*	m_serialPort; //串口类
	QStringList		m_portNameList;

	QMutex			m_lock;			//缓冲区锁 

	QByteArray	m_Cache;			//缓冲区， 超过4000字节 清理一次
	int			m_CmdStartIndex;	//缓冲区有效起始位
	//int			m_CmdEndIndex;	//缓冲区有效结束位
	int			m_MinCmdPreLen;		//预计命令最小长度
	int			m_MaxCmdPreLen;		//预计命令最大长度
	
};
