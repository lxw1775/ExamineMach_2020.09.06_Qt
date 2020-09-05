#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QCaptureScreen.h"

class QCaptureScreen : public QMainWindow
{
	Q_OBJECT

public:
	QCaptureScreen(QWidget *parent = Q_NULLPTR);


protected:
	//当不显示标题栏时 允许移动窗口
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);

private:
	Ui::QCaptureScreenClass ui;

	//当不显示标题栏时 允许移动窗 变量
	bool		m_bDrag;
	QPoint		m_ptMouseStartPoint;
	QPoint		m_ptWindowTopleftPoint;
};
