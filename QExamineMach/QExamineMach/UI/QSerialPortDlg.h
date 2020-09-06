#pragma once
#include <QDialog>
#include <QSerialPort>
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

private slots:
	//��ȡ�˿��б�
	void OnGetPortNameList();
	//�򿪶˿�
	void OnOpenPort();
	//���յ���Ƭ�����͵����ݽ��н���
	void OnReceiveInfo();
	//��������
	void OnSend();

private:
	Ui::QSerialPortDlg ui;

	QSerialPort* m_serialPort; //������
	QStringList m_portNameList;
};
