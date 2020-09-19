#pragma once

#include <QWidget>
#include "ui_QQRCodeReportPage.h"

class QQRCodeReportPage : public QWidget
{
	Q_OBJECT

public:
	QQRCodeReportPage(QWidget *parent = Q_NULLPTR);
	~QQRCodeReportPage();

private:
	Ui::QQRCodeReportPage ui;
};
