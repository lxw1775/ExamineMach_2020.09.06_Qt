#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QHomePage.h"

class QHomePage : public QMainWindow
{
	Q_OBJECT

public:
	QHomePage(QWidget *parent = Q_NULLPTR);

protected:
	//������ʾ������ʱ �����ƶ�����
	virtual void mousePressEvent(QMouseEvent*);
	virtual void mouseMoveEvent(QMouseEvent*);
	virtual void mouseReleaseEvent(QMouseEvent*);

private slots:
	void OnBtnSerialPortTest();

private:
	Ui::QHomePage ui;

	//������ʾ������ʱ �����ƶ��� ����
	bool		m_bDrag;
	QPoint		m_ptMouseStartPoint;
	QPoint		m_ptWindowTopleftPoint;
};
