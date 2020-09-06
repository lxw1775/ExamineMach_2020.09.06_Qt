/*
	�ƽ����ز���
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
	 
	//�򿪶˿�
	bool OpenPort(QString& content);
	//�������� 
	bool Send(QString& content);

	char QWeightMeasure_Youjian::convertCharToHex(char ch);

	//�����͵�Ƭ������ ���� ����16���Ƶ� ���������Լ�дһ�� Qstring תΪ 16���Ƶĺ���	
	void QWeightMeasure_Youjian::convertStringToHex(const QString& str, QByteArray& byteData);

private slots:
	//���յ���Ƭ�����͵����ݽ��н���
	void OnReceiveInfo();

private:
	QSerialPort* m_serialPort; //������
};
