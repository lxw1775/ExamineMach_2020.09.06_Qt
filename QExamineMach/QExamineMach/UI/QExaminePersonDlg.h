#pragma once

#include <QDialog>
#include "ui_QExaminePersonDlg.h"

class QExaminePersonDlg : public QDialog
{
	Q_OBJECT

public:
	QExaminePersonDlg(QWidget *parent = Q_NULLPTR);
	~QExaminePersonDlg();

private:
	Ui::QExaminePersonDlg ui;
};
