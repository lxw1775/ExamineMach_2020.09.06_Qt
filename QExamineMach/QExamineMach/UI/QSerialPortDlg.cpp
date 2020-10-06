#include "QSerialPortDlg.h"
#include "QSerialPortInfo.h"
#include <qDebug>



//另一个 函数 char 转为 16进制
char convertCharToHex2(char ch)
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
void convertStringToHex2(const QString& str, QByteArray& byteData)
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
		hexdata = convertCharToHex2(hstr);
		lowhexdata = convertCharToHex2(lstr);
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


QSerialPortDlg::QSerialPortDlg(QWidget* parent)
	: QDialog(parent)
	, m_CmdStartIndex(0)
	//, m_CmdEndIndex(0)
	, m_MinCmdPreLen(0)
	, m_MaxCmdPreLen(1024)
{
	ui.setupUi(this);

	m_serialPort = new QSerialPort();
	bool b = connect(ui.btnFind, SIGNAL(clicked()), this, SLOT(OnGetPortNameList()));
	bool b2 = connect(ui.btnOpen, SIGNAL(clicked()), this, SLOT(OnOpenPort()));
	bool b3 = connect(ui.btnSend, SIGNAL(clicked()), this, SLOT(OnSend()));
}

QSerialPortDlg::~QSerialPortDlg()
{
	if (m_serialPort->isOpen())
	{
		m_serialPort->close();
	}
	delete m_serialPort;
}

//获取端口列表
void QSerialPortDlg::OnGetPortNameList()
{
	ui.cbxPortName->clear();
	QStringList m_serialPortName;
	//返回系统上可用串行端口的列表
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		m_serialPortName << info.portName();
		ui.cbxPortName->addItem(info.portName());
		qDebug() << "serialPortName:" << info.portName();
	}
	//return m_serialPortName;
}

//打开端口
void QSerialPortDlg::OnOpenPort()
{
	if (m_serialPort->isOpen())//如果串口已经打开了 先给他关闭了
	{
		m_serialPort->clear();
		m_serialPort->close();
		disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnDataReceive()));
		disconnect(this, SIGNAL(sgnRespone(QByteArray)), this, SLOT(OnDataRespone(QByteArray)));
	}

	m_serialPort->setPortName(ui.cbxPortName->currentText());//当前选择的串口名字 ui.cbxPortName->currentText()

	if (!m_serialPort->open(QIODevice::ReadWrite))//用ReadWrite 的模式尝试打开串口
	{
		qDebug() << "打开失败!";
		return;
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
}

//接收到单片机发送的数据进行解析
void QSerialPortDlg::OnDataReceive()
{
	m_lock.lock();
	m_Cache.append(m_serialPort->readAll());
	int len = m_Cache.length() - m_CmdStartIndex;
	m_lock.unlock();

	if (len >= m_MinCmdPreLen)
	{
		DataAnaly();
	}
}

void QSerialPortDlg::DataAnaly()
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

	//悠健体重秤数据分析
	//int start = m_CmdStartIndex;
	//while (1) {
	//	start = m_Cache.indexOf(0x5A, start);	//协议头
	//	if (start <= -1)
	//	{
	//		m_lock.unlock();
	//		break;
	//	}

	//	int end = m_Cache.indexOf(0xAA, start);	//协议尾
	//	if (end <= -1)
	//	{
	//		m_lock.unlock();
	//		break;
	//	}

	//	if (end - start - 1 != m_Cache[start + 1])//长度判断
	//	{
	//		m_lock.unlock();
	//		break;
	//	}

	//	//异或校验
	//	uchar val = m_Cache[start+1];
	//	for (int i = start + 2; i < end-1; ++i)
	//	{
	//		val = val ^ m_Cache[ i];
	//	}
	//	if ((uchar)(m_Cache[end - 1]) == val)
	//	{ 
	//		m_CmdStartIndex = end + 1;
	//		QByteArray arr = m_Cache.mid(start + 2, end - start - 3);
	//		m_lock.unlock();
	//		emit sgnRespone(arr);
	//		break;
	//	}

	//	start++;
	//}	

	QByteArray arr = m_Cache.mid(m_CmdStartIndex);
	m_lock.unlock();
	emit sgnRespone(arr);
}

void QSerialPortDlg::OnDataRespone(QByteArray b)
{
	ui.editReceiveArea->setText(b.toHex(' '));
}

//写两个函数 向单片机发送数据 
void QSerialPortDlg::sendInfo(char* info, int len)
{
	for (int i = 0; i < len; ++i)
	{
		printf("0x%x\n", info[i]);
	}
	m_serialPort->write(info, len);//这句是真正的给单片机发数据 用到的是QIODevice::write 具体可以看文档 
}

void QSerialPortDlg::sendInfo(QString& info)
{
	QByteArray sendBuf;
	if (info.contains(" "))
	{
		info.replace(QRegExp(" "), QString(""));//我这里是把空格去掉，根据你们定的协议来
	}
	qDebug() << "Write to serial: " << info;
	convertStringToHex2(info, sendBuf); //把QString 转换 为 hex 

	m_serialPort->write(sendBuf); //这句是真正的给单片机发数据 用到的是QIODevice::write 具体可以看文档
}

void QSerialPortDlg::OnSend()
{
	QString info = ui.editSendArea->toPlainText();
	m_MinCmdPreLen = 2;
	m_MaxCmdPreLen = 2;
	sendInfo(info);
}