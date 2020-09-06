#include "QWeightMeasure_Youjian.h"
#include <QDebug>


//另一个 函数 char 转为 16进制
char QWeightMeasure_Youjian::convertCharToHex(char ch)
{
	/*
	0x30等于十进制的48，48也是0的ASCII值，，
	1-9的ASCII值是49-57，，所以某一个值－0x30，，
	就是将字符0-9转换为0-9

	*/
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else return (-1);
}

//基本和单片机交互 数据 都是16进制的 我们这里自己写一个 Qstring 转为 16进制的函数	
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

//打开端口
bool QWeightMeasure_Youjian::OpenPort(QString& port)
{
	if (!m_serialPort)
		return false;

	if (m_serialPort->isOpen())//如果串口已经打开了 先给他关闭了
	{
		m_serialPort->clear();
		m_serialPort->close();
		disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnReceiveInfo()));
	}

	m_serialPort->setPortName(port);//当前选择的串口名字 如port

	if (!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
	{
		qCritical() << "open error.";	//输出错误日志
		return false;
	}

	m_serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);//设置波特率和读写方向
	m_serialPort->setDataBits(QSerialPort::Data8);      //数据位为8位
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);	//无流控制
	m_serialPort->setParity(QSerialPort::NoParity);		//无校验位
	m_serialPort->setStopBits(QSerialPort::OneStop);	//一位停止位

	m_serialPort->clear();
	connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnReceiveInfo()));
	return true;
}

//接收到单片机发送的数据进行解析
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
		info.replace(QRegExp(" "), QString(""));//我这里是把空格去掉，根据你们定的协议来
	}
	convertStringToHex(info, sendBuf); //把QString 转换 为 hex 

	m_serialPort->write(sendBuf); //这句是真正的给单片机发数据 用到的是QIODevice::write 具体可以看文档
	return true;
}