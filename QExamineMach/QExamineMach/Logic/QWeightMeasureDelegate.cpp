#include "QWeightMeasureDelegate.h"
#include "QWeightMeasure_Youjian.h"

QWeightMeasureDelegate* QWeightMeasureDelegate::m_instance_ptr = nullptr;

QWeightMeasureDelegate* QWeightMeasureDelegate::GetInstance()
{
	if (m_instance_ptr == nullptr) {
		m_instance_ptr = new QWeightMeasureDelegate();
	}
	return m_instance_ptr;
}

QWeightMeasureDelegate::QWeightMeasureDelegate(QObject *parent)
	: QObject(parent)
{
	m_pQWeightMeasure_Youjian = new QWeightMeasure_Youjian;
}

QWeightMeasureDelegate::~QWeightMeasureDelegate()
{
	if(m_pQWeightMeasure_Youjian)
	{
		delete m_pQWeightMeasure_Youjian;
		m_pQWeightMeasure_Youjian = NULL;
	}
}
