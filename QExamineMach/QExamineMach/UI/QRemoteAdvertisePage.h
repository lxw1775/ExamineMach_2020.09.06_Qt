#pragma once

#include <QWidget>
#include "ui_QRemoteAdvertisePage.h"

class QRemoteAdvertisePage : public QWidget
{
	Q_OBJECT

public:
	QRemoteAdvertisePage(QWidget *parent = Q_NULLPTR);
	~QRemoteAdvertisePage();

private:
	Ui::QRemoteAdvertisePage ui;
};
