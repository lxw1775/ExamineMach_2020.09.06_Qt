#include "QExaminePersonDlg.h"

QExaminePersonDlg::QExaminePersonDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//ui.wgtRemoteAdvertise->hide();
	//ui.wgtNearAdvertise->hide();
	//ui.wgtBMI->hide();
	//ui.wgtBodyFat->hide();
	//ui.wgtQRCodeReport->hide();
}

QExaminePersonDlg::~QExaminePersonDlg()
{
	QDistanceMeasureDelegate::GetInstance()->stop();
}

void QExaminePersonDlg::Init()
{
	QDistanceMeasureDelegate* m_pDistanceMeasure = QDistanceMeasureDelegate::GetInstance();
	m_pDistanceMeasure->start();
}

void QExaminePersonDlg::showEvent(QShowEvent* e)
{

}
 