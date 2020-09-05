#include "QCaptureScreen.h"
#include <QMouseEvent>


QCaptureScreen::QCaptureScreen(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//不显示标题栏
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Dialog);
}


void QCaptureScreen::mouseMoveEvent(QMouseEvent* event)
{
	if (m_bDrag)
	{
		//获取鼠标移动的距离
		QPoint pt = event->globalPos() - m_ptMouseStartPoint;
		//改变窗口的位置
		this->move(m_ptWindowTopleftPoint + pt);
	}

}

void QCaptureScreen::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = true;
		//获取鼠标的位置
		m_ptMouseStartPoint = event->globalPos();
		//改变窗口的初始位置
		m_ptWindowTopleftPoint = this->frameGeometry().topLeft();
	}
}

void QCaptureScreen::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_bDrag = false;
	}
}