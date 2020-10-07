#include "QDistanceMeausre_KS10R.h"
#include <QDebug>
#include "common.h"

using namespace Measure;

QDistanceMeausre_KS10R::QDistanceMeausre_KS10R(QObject* parent)
	: QObject(parent)
	, m_CmdStartIndex(0)
	, m_MinCmdPreLen(0)
	, m_MaxCmdPreLen(1024)
	, m_serialPort(NULL)
	, m_Timer(NULL)
{
	m_serialPort = new QSerialPort();
	connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnDataReceive()));
	connect(this, SIGNAL(sgnRespone(QByteArray)), this, SLOT(OnDataRespone(QByteArray)));

	m_Timer = new QTimer(this);
	// m_Timer->setSingleShot(true);  //һ�ζ�ʱ
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnSend()));
}

QDistanceMeausre_KS10R::~QDistanceMeausre_KS10R()
{
	if (m_serialPort)
	{
		disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnDataReceive()));
		disconnect(this, SIGNAL(sgnRespone(QByteArray)), this, SLOT(OnDataRespone(QByteArray)));
		if (m_serialPort->isOpen())
		{
			m_serialPort->close();
		}
		delete m_serialPort;
	}

	if (m_Timer)
		delete m_Timer;
}

int QDistanceMeausre_KS10R::start(QString& port)
{
	if (!m_serialPort)
		return -1;

	//if (!m_serialPort->isOpen())
	if (!OpenPort(port))
		return -1;

	setSendCmd();
	//��Ҫ�Լ�ȷ�϶˿� 
	if (m_Timer)
		m_Timer->start(250);
	return 0;
}

void QDistanceMeausre_KS10R::changeInterval(int mes)
{
	if (m_Timer && m_Timer->isActive())
		m_Timer->setInterval(mes);
}

void QDistanceMeausre_KS10R::stop()
{
	if (m_Timer && m_Timer->isActive())
		m_Timer->stop();

	m_serialPort->close();
	m_serialPort->clear();
	m_Cache.clear();
	m_CmdStartIndex = 0;
}
//�򿪶˿�
bool QDistanceMeausre_KS10R::OpenPort(QString& port)
{
	if (!m_serialPort)
		return false;

	if (m_serialPort->isOpen())//��������Ѿ����� �ȸ����ر���
	{
		m_serialPort->clear();
		m_serialPort->close();
	}

	m_serialPort->setPortName(port);//��ǰѡ��Ĵ������� ��port

	if (!m_serialPort->open(QIODevice::ReadWrite))//��ReadWrite ��ģʽ���Դ򿪴���
	{
		qCritical() << "serial port open error, port=" << port;	//���������־
		return false;
	}

	m_serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);//���ò����ʺͶ�д����
	m_serialPort->setDataBits(QSerialPort::Data8);      //����λΪ8λ
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);	//��������
	m_serialPort->setParity(QSerialPort::NoParity);		//��У��λ
	m_serialPort->setStopBits(QSerialPort::OneStop);	//һλֹͣλ
	m_serialPort->setReadBufferSize(1024);  //���ö���������С

	m_serialPort->clear();

	return true;
}

//���յ���Ƭ�����͵����ݽ��н���
void QDistanceMeausre_KS10R::OnDataReceive()
{
	if (!m_serialPort)
		return;

	m_lock.lock();
	m_Cache.append(m_serialPort->readAll());
	int len = m_Cache.length() - m_CmdStartIndex; //���������Ч�����С
	m_lock.unlock();

	if (len >= m_MinCmdPreLen)
	{
		DataAnaly();
	}
}

void QDistanceMeausre_KS10R::DataAnaly()
{
	m_lock.lock();
	if (m_Cache.length() > 4000)
	{	//����̫�� ����С
		QByteArray arr = m_Cache.mid(m_CmdStartIndex);
		m_Cache.clear();
		m_Cache.append(arr);
		m_CmdStartIndex = 0;
	}
	if (m_Cache.length() - m_CmdStartIndex > m_MaxCmdPreLen)
	{	//������������󳤶�
		m_CmdStartIndex = m_Cache.length() - m_MaxCmdPreLen;
	}

	int start = m_CmdStartIndex;
	if (m_Cache.length() - m_CmdStartIndex < 2)
	{
		m_lock.unlock();
		return;
	}

	QByteArray arr = m_Cache.mid(m_CmdStartIndex, 2);
	m_CmdStartIndex += 2;
	m_lock.unlock();

	int val = 0;
	for (int i = 0; i < arr.size(); ++i)
	{
		val += val * 256 + arr[i];
	}
	emit sgnRealData(val);
	//emit sgnRespone(arr);
	return;
}

void QDistanceMeausre_KS10R::OnDataRespone(QByteArray content)
{
}

void QDistanceMeausre_KS10R::setSendCmd()
{
	m_MinCmdPreLen = 2;
	m_MaxCmdPreLen = 2;
	//QString content = "e8 02 bc";
	m_sendCmd = QByteArray::fromHex(QVariant("E802BC").toByteArray());
}

void QDistanceMeausre_KS10R::OnSend()
{
	if (!m_serialPort)
		return;

	m_serialPort->write(m_sendCmd);
	return;
}

bool QDistanceMeausre_KS10R::SendData(QString& content)
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

