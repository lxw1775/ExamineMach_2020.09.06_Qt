#pragma once

#include <QWidget>
#include "ui_QBodyFatMeasurePage.h"

class QBodyFatMeasurePage : public QWidget
{
	Q_OBJECT

public:
	QBodyFatMeasurePage(QWidget *parent = Q_NULLPTR);
	~QBodyFatMeasurePage();

private:
	Ui::QBodyFatMeasurePage ui;
};
