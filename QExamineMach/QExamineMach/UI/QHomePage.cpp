#include "QHomePage.h"
#include <QMouseEvent>
#include <QPushButton>
#include "QSerialPortDlg.h"
//�Ƿ��ڲر�����
//#define TITLEBAR_HIDE


QHomePage::QHomePage(QWidget *parent)
	: QMainWindow(parent)
	, m_bDrag(false)
{
	ui.setupUi(this);

#ifdef TITLEBAR_HIDE
	//����ʾ������
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);
#endif // TITLEBAR_HIDE

	//SIGNAL�к������� �� SLOT ��������һģһ��
	connect(ui.btnSerialPortTest, SIGNAL(clicked()), this, SLOT(OnBtnSerialPortTest()));
	QPushButton;
}


void QHomePage::mouseMoveEvent(QMouseEvent* event)
{

#ifdef TITLEBAR_HIDE
	if (m_bDrag)
	{
		//��ȡ����ƶ��ľ���
		QPoint pt = event->globalPos() - m_ptMouseStartPoint;
		//�ı䴰�ڵ�λ��
		this->move(m_ptWindowTopleftPoint + pt);
	}
#endif // TITLEBAR_HIDE
}

void QHomePage::mousePressEvent(QMouseEvent* event)
{
#ifdef TITLEBAR_HIDE
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = true;
		//��ȡ����λ��
		m_ptMouseStartPoint = event->globalPos();
		//�ı䴰�ڵĳ�ʼλ��
		m_ptWindowTopleftPoint = this->frameGeometry().topLeft();
	}
#endif // TITLEBAR_HIDE
}

void QHomePage::mouseReleaseEvent(QMouseEvent* event)
{
#ifdef TITLEBAR_HIDE
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
#endif // TITLEBAR_HIDE
}

void QHomePage::OnBtnSerialPortTest()
{
	QSerialPortDlg dlg;
	dlg.exec();
}