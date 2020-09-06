#include "QSerialPortDlg.h"
#include "QSerialPortInfo.h"
#include <qDebug>



//��һ�� ���� char תΪ 16����
char convertCharToHex(char ch)
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
void convertStringToHex(const QString& str, QByteArray& byteData)
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


QSerialPortDlg::QSerialPortDlg(QWidget *parent)
	: QDialog(parent)
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
		disconnect(m_serialPort, SIGNAL(readyRead()), this, SLOT(receiveInfo()));
	}

	m_serialPort->setPortName(ui.cbxPortName->currentText());//��ǰѡ��Ĵ������� ui.cbxPortName->currentText()

	if (!m_serialPort->open(QIODevice::ReadWrite))//��ReadWrite ��ģʽ���Դ򿪴���
	{
		qDebug() << "��ʧ��!";
		return;
	}
	qDebug() << "���ڴ򿪳ɹ�!";

	m_serialPort->setBaudRate(QSerialPort::Baud9600, QSerialPort::AllDirections);//���ò����ʺͶ�д����
	m_serialPort->setDataBits(QSerialPort::Data8);      //����λΪ8λ
	m_serialPort->setFlowControl(QSerialPort::NoFlowControl);	//��������
	m_serialPort->setParity(QSerialPort::NoParity);		//��У��λ
	m_serialPort->setStopBits(QSerialPort::OneStop);	//һλֹͣλ

	m_serialPort->clear();
	connect(m_serialPort, SIGNAL(readyRead()), this, SLOT(OnReceiveInfo()));
}

//���յ���Ƭ�����͵����ݽ��н���
void QSerialPortDlg::OnReceiveInfo()
{
	QByteArray info = m_serialPort->readAll();
	QString str(info.toHex(' '));
	ui.editReceiveArea->setText(str);
	qDebug() << "receive info:" << info;
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
	convertStringToHex(info, sendBuf); //��QString ת�� Ϊ hex 

	m_serialPort->write(sendBuf); //����������ĸ���Ƭ�������� �õ�����QIODevice::write ������Կ��ĵ�
}

void QSerialPortDlg::OnSend()
{
	QString info = ui.editSendArea->toPlainText();
	sendInfo(info);
}