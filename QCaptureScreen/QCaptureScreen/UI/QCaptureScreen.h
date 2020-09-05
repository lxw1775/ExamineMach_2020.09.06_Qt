#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QCaptureScreen.h"

class QCaptureScreen : public QMainWindow
{
	Q_OBJECT

public:
	QCaptureScreen(QWidget *parent = Q_NULLPTR);

private:
	Ui::QCaptureScreenClass ui;
};
