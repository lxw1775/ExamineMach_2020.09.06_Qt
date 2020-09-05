#include "QHomePage.h"
#include <QMouseEvent>

//是否掩藏标题栏
//#define TITLEBAR_HIDE


QCaptureScreen::QCaptureScreen(QWidget *parent)
	: QMainWindow(parent)
	, m_bDrag(false)
{
	ui.setupUi(this);

#ifdef TITLEBAR_HIDE
	//不显示标题栏
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);
#endif // TITLEBAR_HIDE
}


void QCaptureScreen::mouseMoveEvent(QMouseEvent* event)
{

#ifdef TITLEBAR_HIDE
	if (m_bDrag)
	{
		//获取鼠标移动的距离
		QPoint pt = event->globalPos() - m_ptMouseStartPoint;
		//改变窗口的位置
		this->move(m_ptWindowTopleftPoint + pt);
	}
#endif // TITLEBAR_HIDE
}

void QCaptureScreen::mousePressEvent(QMouseEvent* event)
{
#ifdef TITLEBAR_HIDE
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = true;
		//获取鼠标的位置
		m_ptMouseStartPoint = event->globalPos();
		//改变窗口的初始位置
		m_ptWindowTopleftPoint = this->frameGeometry().topLeft();
	}
#endif // TITLEBAR_HIDE
}

void QCaptureScreen::mouseReleaseEvent(QMouseEvent* event)
{
#ifdef TITLEBAR_HIDE
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
#endif // TITLEBAR_HIDE
}