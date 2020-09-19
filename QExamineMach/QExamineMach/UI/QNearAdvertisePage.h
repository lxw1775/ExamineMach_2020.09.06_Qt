#pragma once

#include <QWidget>
#include "ui_QNearAdvertisePage.h"

class QNearAdvertisePage : public QWidget
{
	Q_OBJECT

public:
	QNearAdvertisePage(QWidget *parent = Q_NULLPTR);
	~QNearAdvertisePage();

private:
	Ui::QNearAdvertisePage ui;
};
