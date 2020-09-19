/*
	�ƽ����ز���
*/

#pragma once

#include <QObject>
#include <QSerialPort>
#include <QMutex>
#include <QWidget>
#include "ui_QSerialPortDlg.h"

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
	 
	//�򿪶˿�
	bool OpenPort(QString& content);
	bool Send(QString& content);
private:
	//�������� 
	bool SendData(QString& content);
	//���ݷ���
	void DataAnaly();
private slots:
	//���յ���Ƭ�����͵����ݽ��н���
	void OnDataReceive();
	//��Ӧ
	void OnDataRespone(QByteArray b);
private:
signals:
	void sgnRespone(QByteArray b);
	void sgnCmd();

private:
	QSerialPort* m_serialPort; //������


	QMutex		m_lock;			//�������� 

	QByteArray	m_Cache;			//�������� ����4000�ֽ� ����һ��
	int			m_CmdStartIndex;	//��������Ч��ʼλ
	//int			m_CmdEndIndex;	//��������Ч����λ
	int			m_MinCmdPreLen;		//Ԥ��������С����
	int			m_MaxCmdPreLen;		//Ԥ��������󳤶�

};
