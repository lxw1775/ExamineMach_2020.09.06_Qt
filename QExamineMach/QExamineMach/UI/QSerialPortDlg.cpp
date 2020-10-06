#include "QSerialPortDlg.h"
#include "QSerialPortInfo.h"
#include <qDebug>



//��һ�� ���� char תΪ 16����
char convertCharToHex2(char ch)
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

//��ȡ�˿��б�
void QSerialPortDlg::OnGetPortNameList()
{
	ui.cbxPortName->clear();
	QStringList m_serialPortName;
	//����ϵͳ�Ͽ��ô��ж˿ڵ��б�
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		m_serialPortName << info.portName();
		ui.cbxPortName->addItem(info.portName());
		qDebug() << "serialPortName:" << info.portName();
	}
	//return m_serialPortName;
}

//�򿪶˿�
void QSerialPortDlg::OnOpenPort()
{
	if (m_serialPort->isOpen())//��������Ѿ����� �ȸ����ر���
	{
		m_serialPort->clear();
		m_serialPort->close();
		disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnDataReceive()));
		disconnect(this, SIGNAL(sgnRespone(QByteArray)), this, SLOT(OnDataRespone(QByteArray)));
	}

	m_serialPort->setPortName(ui.cbxPortName->currentText());//��ǰѡ��Ĵ������� ui.cbxPortName->currentText()

	if (!m_serialPort->open(QIODevice::ReadWrite))//��ReadWrite ��ģʽ���Դ򿪴���
	{
		qDebug() << "��ʧ��!";
		return;
	}

	m_serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);//���ò����ʺͶ�д����
	m_serialPort->setDataBits(QSerialPort::Data8);      //����λΪ8λ
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);	//��������
	m_serialPort->setParity(QSerialPort::NoParity);		//��У��λ
	m_serialPort->setStopBits(QSerialPort::OneStop);	//һλֹͣλ
	m_serialPort->setReadBufferSize(1024);  //���ö���������С����

	m_serialPort->clear();
	connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnDataReceive()));
	connect(this, SIGNAL(sgnRespone(QByteArray)), this, SLOT(OnDataRespone(QByteArray)));
}

//���յ���Ƭ�����͵����ݽ��н���
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

	//�ƽ����س����ݷ���
	//int start = m_CmdStartIndex;
	//while (1) {
	//	start = m_Cache.indexOf(0x5A, start);	//Э��ͷ
	//	if (start <= -1)
	//	{
	//		m_lock.unlock();
	//		break;
	//	}

	//	int end = m_Cache.indexOf(0xAA, start);	//Э��β
	//	if (end <= -1)
	//	{
	//		m_lock.unlock();
	//		break;
	//	}

	//	if (end - start - 1 != m_Cache[start + 1])//�����ж�
	//	{
	//		m_lock.unlock();
	//		break;
	//	}

	//	//���У��
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

//д�������� ��Ƭ���������� 
void QSerialPortDlg::sendInfo(char* info, int len)
{
	for (int i = 0; i < len; ++i)
	{
		printf("0x%x\n", info[i]);
	}
	m_serialPort->write(info, len);//����������ĸ���Ƭ�������� �õ�����QIODevice::write ������Կ��ĵ� 
}

void QSerialPortDlg::sendInfo(QString& info)
{
	QByteArray sendBuf;
	if (info.contains(" "))
	{
		info.replace(QRegExp(" "), QString(""));//�������ǰѿո�ȥ�����������Ƕ���Э����
	}
	qDebug() << "Write to serial: " << info;
	convertStringToHex2(info, sendBuf); //��QString ת�� Ϊ hex 

	m_serialPort->write(sendBuf); //����������ĸ���Ƭ�������� �õ�����QIODevice::write ������Կ��ĵ�
}

void QSerialPortDlg::OnSend()
{
	QString info = ui.editSendArea->toPlainText();
	m_MinCmdPreLen = 2;
	m_MaxCmdPreLen = 2;
	sendInfo(info);
}