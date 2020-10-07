#include "QWeightMeasure_Youjian.h"
#include <QDebug>
#include "common.h"

using namespace Measure;

QWeightMeasure_Youjian::QWeightMeasure_Youjian(QObject* parent)
	: QObject(parent)
	, m_CmdStartIndex(0)
	, m_MinCmdPreLen(0)
	, m_MaxCmdPreLen(1024)
	, m_Timer(NULL)
{
	m_serialPort = new QSerialPort();
	m_Timer = new QTimer(this);
	// m_Timer->setSingleShot(true);  //一次定时
	connect(m_Timer, SIGNAL(timeout()), this, SLOT(OnSend()));
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

	if (m_Timer)
		delete m_Timer;
}

int QWeightMeasure_Youjian::weightMeasureStart(QString& port)
{
	if (!m_serialPort)
		return -1;

	//if (!m_serialPort->isOpen())
	if (!OpenPort(port))
		return -1;

	setSendCmd();

	//需要自检确认端口 
	if (m_Timer)
		m_Timer->start(500);
	return 0;
}

void QWeightMeasure_Youjian::stop()
{
	if (m_Timer && m_Timer->isActive())
		m_Timer->stop();

	m_serialPort->close();
	m_serialPort->clear();
	m_Cache.clear();
	m_CmdStartIndex = 0;
}

//打开端口
bool QWeightMeasure_Youjian::OpenPort(QString& port)
{
	if (!m_serialPort)
		return false;

	if (m_serialPort->isOpen())//如果串口已经打开了 先给他关闭了
	{
		m_serialPort->clear();
		m_serialPort->close();
		disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnDataReceive()));
		disconnect(this, SIGNAL(sgnRespone(QByteArray)), this, SLOT(OnDataRespone(QByteArray)));
	}

	m_serialPort->setPortName(port);//当前选择的串口名字 如port

	if (!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
	{
		qCritical() << "serial port open error, port=" << port;	//输出错误日志
		return false;
	}

	m_serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);//设置波特率和读写方向
	m_serialPort->setDataBits(QSerialPort::Data8);      //数据位为8位
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);	//无流控制
	m_serialPort->setParity(QSerialPort::NoParity);		//无校验位
	m_serialPort->setStopBits(QSerialPort::OneStop);	//一位停止位
	m_serialPort->setReadBufferSize(1024);  //设置读缓冲区大小，陈

	m_serialPort->clear();
	connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnDataReceive()));
	connect(this, SIGNAL(sgnRespone(QByteArray)), this, SLOT(OnDataRespone(QByteArray)));

	return true;
}

//接收到单片机发送的数据进行解析
void QWeightMeasure_Youjian::OnDataReceive()
{
	if (!m_serialPort)
		return;

	m_lock.lock();
	m_Cache.append(m_serialPort->readAll());
	int len = m_Cache.length() - m_CmdStartIndex; //计算可能有效缓冲大小
	m_lock.unlock();

	if (len >= m_MinCmdPreLen)
	{
		DataAnaly();
	}
}

void QWeightMeasure_Youjian::DataAnaly()
{
	m_lock.lock();
	if (m_Cache.length() > 4000)
	{	//缓冲太大 先缩小
		QByteArray arr = m_Cache.mid(m_CmdStartIndex);
		m_Cache.clear();
		m_Cache.append(arr);
		m_CmdStartIndex = 0;
	}
	if (m_Cache.length() - m_CmdStartIndex > m_MaxCmdPreLen)
	{	//若超过命令最大长度
		m_CmdStartIndex = m_Cache.length() - m_MaxCmdPreLen;
	}

	int start = m_CmdStartIndex;
	while (1) {
		start = m_Cache.indexOf(0x5A, start);	//协议头
		if (start <= -1)
		{
			m_lock.unlock();
			break;
		}

		int end = m_Cache.indexOf(0xAA, start);	//协议尾
		if (end <= -1)
		{
			m_lock.unlock();
			break;
		}

		if (end - start - 1 != m_Cache[start + 1])//长度判断
		{
			m_lock.unlock();
			break;
		}

		//异或校验
		uchar val = m_Cache[start + 1];
		for (int i = start + 2; i < end - 1; ++i)
		{
			val = val ^ m_Cache[i];
		}
		if ((uchar)(m_Cache[end - 1]) == val)
		{
			m_CmdStartIndex = end + 1;
			QByteArray arr = m_Cache.mid(start + 2, end - start - 3);
			m_lock.unlock();
			emit sgnRespone(arr);
			break;
		}

		start++;
	}
}

void QWeightMeasure_Youjian::OnDataRespone(QByteArray b)
{
	//ui.editReceiveArea->setText(b.toHex(' '));
}

void QWeightMeasure_Youjian::setSendCmd()
{
	m_MinCmdPreLen = 7;
	m_MaxCmdPreLen = 7;
	//QString content = "e8 02 bc";
	m_sendCmd = QByteArray::fromHex(QVariant("A50727A002010026AA").toByteArray()); //测量身高
}

void QWeightMeasure_Youjian::OnSend()
{
	if (!m_serialPort)
		return;
	m_serialPort->write(m_sendCmd);
}

bool QWeightMeasure_Youjian::Send(QString& content)
{
	m_MinCmdPreLen = 7;
	m_MaxCmdPreLen = 7;
	SendData(content);
	return true;
}

//A5 07 27 A0 02 01 00 26 AA  
bool QWeightMeasure_Youjian::SendData(QString& content)
{
	if (!m_serialPort)
		return false;

	QString info = content;
	QByteArray sendBuf;
	if (info.contains(" "))
	{
		info.replace(QRegExp(" "), QString(""));//我这里是把空格去掉，根据你们定的协议来
	}
	convertStringToHex(info, sendBuf); //把QString 转换 为 hex 

	m_serialPort->write(sendBuf); //这句是真正的给单片机发数据 用到的是QIODevice::write 具体可以看文档
	return true;
}