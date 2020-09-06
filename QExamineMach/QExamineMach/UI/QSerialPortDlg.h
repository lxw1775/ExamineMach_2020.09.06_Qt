#pragma once
#include <QDialog>
#include <QSerialPort>
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

private slots:
	//获取端口列表
	void OnGetPortNameList();
	//打开端口
	void OnOpenPort();
	//接收到单片机发送的数据进行解析
	void OnReceiveInfo();
	//发送数据
	void OnSend();

private:
	Ui::QSerialPortDlg ui;

	QSerialPort* m_serialPort; //串口类
	QStringList m_portNameList;
};
