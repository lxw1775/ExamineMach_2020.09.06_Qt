#include "QHeightMeasureDelegate.h"
#include "QDistanceMeausre_KS10R.h"

QHeightMeasureDelegate* QHeightMeasureDelegate::m_instance_ptr = nullptr;

QHeightMeasureDelegate* QHeightMeasureDelegate::GetInstance()
{
	if (m_instance_ptr == nullptr) {
		m_instance_ptr = new QHeightMeasureDelegate();
	}
	return m_instance_ptr;
}

QHeightMeasureDelegate::QHeightMeasureDelegate(QObject *parent)
	: QObject(parent)
{
	m_pQDistanceMeausre_KS10R = new QDistanceMeausre_KS10R();
}

QHeightMeasureDelegate::~QHeightMeasureDelegate()
{
	if (m_pQDistanceMeausre_KS10R)
	{
		delete m_pQDistanceMeausre_KS10R;
		m_pQDistanceMeausre_KS10R = NULL;
	}
}
