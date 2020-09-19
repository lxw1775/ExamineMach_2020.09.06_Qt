#pragma once
#include <QDialog>
#include <QSerialPort>
#include <QMutex>
/*
	Qt ����ͨ�Ų��Դ���
	�ο���https://blog.csdn.net/weixin_42837024/article/details/81669540

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
	//д�������� ��Ƭ���������� 
	void sendInfo(char* info, int len);
	void sendInfo(QString& info);
	void DataAnaly();
private slots:
	//��ȡ�˿��б�
	void OnGetPortNameList();
	//�򿪶˿�
	void OnOpenPort();
	//���յ���Ƭ�����͵����ݽ��н���
	void OnDataReceive();
	//��������
	void OnSend();
	//��Ӧ
	void OnDataRespone(QByteArray b);
private:
signals:
	void sgnRespone(QByteArray b);
	void sgnCmd();

private:
	Ui::QSerialPortDlg ui;

	QSerialPort*	m_serialPort; //������
	QStringList		m_portNameList;

	QMutex			m_lock;			//�������� 

	QByteArray	m_Cache;			//�������� ����4000�ֽ� ����һ��
	int			m_CmdStartIndex;	//��������Ч��ʼλ
	//int			m_CmdEndIndex;	//��������Ч����λ
	int			m_MinCmdPreLen;		//Ԥ��������С����
	int			m_MaxCmdPreLen;		//Ԥ��������󳤶�
	
};
