/*
	悠健体重测量
*/

#pragma once

#include <QObject>
#include <QSerialPort>

class QWeightMeasure_Youjian : public QObject
{
	Q_OBJECT

public:
	QWeightMeasure_Youjian(QObject *parent = NULL);
	~QWeightMeasure_Youjian();
	 
	//打开端口
	bool OpenPort(QString& content);
	//发送数据 
	bool Send(QString& content);

	char QWeightMeasure_Youjian::convertCharToHex(char ch);

	//基本和单片机交互 数据 都是16进制的 我们这里自己写一个 Qstring 转为 16进制的函数	
	void QWeightMeasure_Youjian::convertStringToHex(const QString& str, QByteArray& byteData);

private slots:
	//接收到单片机发送的数据进行解析
	void OnReceiveInfo();

private:
	QSerialPort* m_serialPort; //串口类
};
