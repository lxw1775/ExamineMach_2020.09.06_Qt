#include "QWeightMeasure_Youjian.h"
#include <QDebug>


//��һ�� ���� char תΪ 16����
char QWeightMeasure_Youjian::convertCharToHex(char ch)
{
	/*
	0x30����ʮ���Ƶ�48��48Ҳ��0��ASCIIֵ����
	1-9��ASCIIֵ��49-57��������ĳһ��ֵ��0x30����
	���ǽ��ַ�0-9ת��Ϊ0-9

	*/
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else return (-1);
}

//�����͵�Ƭ������ ���� ����16���Ƶ� ���������Լ�дһ�� Qstring תΪ 16���Ƶĺ���	
void QWeightMeasure_Youjian::convertStringToHex(const QString& str, QByteArray& byteData)
{
	int hexdata, lowhexdata;
	int hexdatalen = 0;
	int len = str.length();
	byteData.resize(len / 2);
	char lstr, hstr;
	for (int i = 0; i < len; )
	{
		//char lstr,
		hstr = str[i].toLatin1();
		if (hstr == ' ')
		{
			i++;
			continue;
		}
		i++;
		if (i >= len)
			break;
		lstr = str[i].toLatin1();
		hexdata = convertCharToHex(hstr);
		lowhexdata = convertCharToHex(lstr);
		if ((hexdata == 16) || (lowhexdata == 16))
			break;
		else
			hexdata = hexdata * 16 + lowhexdata;
		i++;
		byteData[hexdatalen] = (char)hexdata;
		hexdatalen++;
	}
	byteData.resize(hexdatalen);
}


QWeightMeasure_Youjian::QWeightMeasure_Youjian(QObject *parent)
	: QObject(parent)
{
	m_serialPort = new QSerialPort();
}

QWeightMeasure_Youjian::~QWeightMeasure_Youjian()
{
	if (m_serialPort)
	{
		if (m_serialPort->isOpen())
		{
			m_serialPort->close();
		}
		delete m_serialPort;
	}
}

//�򿪶˿�
bool QWeightMeasure_Youjian::OpenPort(QString& port)
{
	if (!m_serialPort)
		return false;

	if (m_serialPort->isOpen())//��������Ѿ����� �ȸ����ر���
	{
		m_serialPort->clear();
		m_serialPort->close();
		disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnReceiveInfo()));
	}

	m_serialPort->setPortName(port);//��ǰѡ��Ĵ������� ��port

	if (!m_serialPort->open(QIODevice::ReadWrite))//��ReadWrite ��ģʽ���Դ򿪴���
	{
		qCritical() << "open error.";	//���������־
		return false;
	}

	m_serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);//���ò����ʺͶ�д����
	m_serialPort->setDataBits(QSerialPort::Data8);      //����λΪ8λ
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);	//��������
	m_serialPort->setParity(QSerialPort::NoParity);		//��У��λ
	m_serialPort->setStopBits(QSerialPort::OneStop);	//һλֹͣλ

	m_serialPort->clear();
	connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnReceiveInfo()));
	return true;
}

//���յ���Ƭ�����͵����ݽ��н���
void QWeightMeasure_Youjian::OnReceiveInfo()
{
	if (!m_serialPort)
		return ;
	QByteArray info = m_serialPort->readAll();
	QString str(info.toHex(' ')); 
}
 
bool QWeightMeasure_Youjian::Send(QString& content)
{
	if (!m_serialPort)
		return false;

	QString info = content;
	QByteArray sendBuf;
	if (info.contains(" "))
	{
		info.replace(QRegExp(" "), QString(""));//�������ǰѿո�ȥ�����������Ƕ���Э����
	}
	convertStringToHex(info, sendBuf); //��QString ת�� Ϊ hex 

	m_serialPort->write(sendBuf); //����������ĸ���Ƭ�������� �õ�����QIODevice::write ������Կ��ĵ�
	return true;
}