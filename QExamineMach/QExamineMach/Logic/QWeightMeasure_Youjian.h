/*
	悠健体重测量
*/

#pragma once

#include <QObject>
#include <QSerialPort>
#include <QMutex>
#include <QTimer>


enum deviceType
{
	eCommon = 0x27
};

enum cmdType
{
	eCmd = 0
};

class QWeightMeasure_Youjian : public QObject
{
	Q_OBJECT

public:
	QWeightMeasure_Youjian(QObject *parent = NULL);
	~QWeightMeasure_Youjian();
	 

	int weightMeasureStart(QString& port);
	void stop();

private:
	//打开端口
	bool OpenPort(QString& content);
	bool Send(QString& content);
	//发送数据 
	bool SendData(QString& content);
	//数据分析
	void DataAnaly();
	//设置发送命令
	void setSendCmd();
private slots:
	//定时发送
	void OnSend();
	//接收到单片机发送的数据进行解析
	void OnDataReceive();
	//响应
	void OnDataRespone(QByteArray b);

private:
signals:
	void sgnRespone(QByteArray b);

private:
	QSerialPort* m_serialPort; //串口类


	QMutex		m_lock;			//缓冲区锁 

	QByteArray	m_Cache;			//缓冲区， 超过4000字节 清理一次
	int			m_CmdStartIndex;	//缓冲区有效起始位
	//int			m_CmdEndIndex;	//缓冲区有效结束位
	int			m_MinCmdPreLen;		//预计命令最小长度
	int			m_MaxCmdPreLen;		//预计命令最大长度

	QTimer*		m_Timer;
	QByteArray	m_sendCmd;
};