#pragma once

#include <QWidget>
#include "ui_QBMIMeasurePage.h"

class QBMIMeasurePage : public QWidget
{
	Q_OBJECT

public:
	QBMIMeasurePage(QWidget *parent = Q_NULLPTR);
	~QBMIMeasurePage();

	void startMeasure();
	void stopMeasure();

private:
	Ui::QBMIMeasurePage ui;
};
