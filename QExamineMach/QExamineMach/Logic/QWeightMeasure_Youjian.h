/*
	�ƽ����ز���
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
	//�򿪶˿�
	bool OpenPort(QString& content);
	bool Send(QString& content);
	//�������� 
	bool SendData(QString& content);
	//���ݷ���
	void DataAnaly();
	//���÷�������
	void setSendCmd();
private slots:
	//��ʱ����
	void OnSend();
	//���յ���Ƭ�����͵����ݽ��н���
	void OnDataReceive();
	//��Ӧ
	void OnDataRespone(QByteArray b);

private:
signals:
	void sgnRespone(QByteArray b);

private:
	QSerialPort* m_serialPort; //������


	QMutex		m_lock;			//�������� 

	QByteArray	m_Cache;			//�������� ����4000�ֽ� ����һ��
	int			m_CmdStartIndex;	//��������Ч��ʼλ
	//int			m_CmdEndIndex;	//��������Ч����λ
	int			m_MinCmdPreLen;		//Ԥ��������С����
	int			m_MaxCmdPreLen;		//Ԥ��������󳤶�

	QTimer*		m_Timer;
	QByteArray	m_sendCmd;
};