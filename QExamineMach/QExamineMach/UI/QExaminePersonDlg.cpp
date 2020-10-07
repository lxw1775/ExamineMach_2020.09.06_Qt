#include "QExaminePersonDlg.h"

QExaminePersonDlg::QExaminePersonDlg(QWidget* parent)
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
}

void QExaminePersonDlg::Init()
{
	QDistanceMeasureDelegate* m_pDistanceMeasure = QDistanceMeasureDelegate::GetInstance();
	bool b = connect(m_pDistanceMeasure, SIGNAL(sgnDistanceStateChanged(DistanceState)), this, SLOT(OnDistanceStateChanged(DistanceState)));
	m_pDistanceMeasure->start();
}

void QExaminePersonDlg::UnInit()
{
	QDistanceMeasureDelegate* m_pDistanceMeasure = QDistanceMeasureDelegate::GetInstance();
	m_pDistanceMeasure->stop();
	disconnect(m_pDistanceMeasure, SIGNAL(sgnDistanceStateChanged(DistanceState, DistanceState)), this, SLOT(OnDistanceStateChanged(DistanceState, DistanceState)));
}

void QExaminePersonDlg::showEvent(QShowEvent* e)
{

}

void QExaminePersonDlg::OnDistanceStateChanged(DistanceState eOld, DistanceState e)
{
	ui.wgtRemoteAdvertise->hide();
	ui.wgtNearAdvertise->hide();
	ui.wgtBMI->hide();
	ui.wgtBodyFat->hide();
	ui.wgtQRCodeReport->hide();

	switch (e)
	{
	case eRemote:
		ui.wgtRemoteAdvertise->show();
		break;
	case eNear:
		ui.wgtNearAdvertise->show();
		break;
	case eEdge:
		ui.wgtBMI->show();
		break;
	}
}