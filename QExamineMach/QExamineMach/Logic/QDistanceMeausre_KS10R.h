#pragma once

#include <QObject>
#include <QSerialPort>
#include <QMutex>
#include <QTimer>

class QDistanceMeausre_KS10R : public QObject
{
	Q_OBJECT

public:
	QDistanceMeausre_KS10R(QObject *parent= nullptr);
	~QDistanceMeausre_KS10R();

	//�򿪶˿�
	bool OpenPort(QString& content);
	int start();
	void stop();
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
	//
	void OnSend();
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
};
