#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QHomePage.h"

class QHomePage : public QMainWindow
{
	Q_OBJECT

public:
	QHomePage(QWidget *parent = Q_NULLPTR);

protected:
	//当不显示标题栏时 允许移动窗口
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);

private slots:
	void OnBtnSerialPortTest();
	void OnBtnExamineMachStart();

private:
	Ui::QHomePage ui;

	//当不显示标题栏时 允许移动窗 变量
	bool		m_bDrag;
	QPoint		m_ptMouseStartPoint;
	QPoint		m_ptWindowTopleftPoint;
};
