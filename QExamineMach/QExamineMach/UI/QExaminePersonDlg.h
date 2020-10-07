#pragma once

#include <QDialog>
#include <QShowEvent>
#include "QDistanceMeasureDelegate.h"
#include "ui_QExaminePersonDlg.h"

class QExaminePersonDlg : public QDialog
{
	Q_OBJECT

public:
	QExaminePersonDlg(QWidget *parent = Q_NULLPTR);
	~QExaminePersonDlg();

	void Init();
	void UnInit();
protected:
	void showEvent(QShowEvent* e);

public slots:
	void OnDistanceStateChanged(DistanceState eOld, DistanceState e);

private:
	Ui::QExaminePersonDlg ui;	
};
