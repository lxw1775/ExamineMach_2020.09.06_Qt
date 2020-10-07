#include "QBMIMeasurePage.h"
#include "QWeightMeasureDelegate.h"

QBMIMeasurePage::QBMIMeasurePage(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

QBMIMeasurePage::~QBMIMeasurePage()
{
}

void QBMIMeasurePage::startMeasure()
{
	connect(QWeightMeasureDelegate::GetInstance(), SIGNAL(), this, SLOT());
	QWeightMeasureDelegate::GetInstance()->start();
}

void QBMIMeasurePage::stopMeasure()
{
	QWeightMeasureDelegate::GetInstance()->stop();
	connect(QWeightMeasureDelegate::GetInstance(), SIGNAL(), this, SLOT());
}