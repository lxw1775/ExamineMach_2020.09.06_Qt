#include "QDistanceMeasureDelegate.h"
#include "QDistanceMeausre_KS10R.h"

QDistanceMeasureDelegate* QDistanceMeasureDelegate::m_instance_ptr = nullptr;

QDistanceMeasureDelegate* QDistanceMeasureDelegate::GetInstance()
{
	if (m_instance_ptr == nullptr) {
		m_instance_ptr = new QDistanceMeasureDelegate();
	}
	return m_instance_ptr;
}

QDistanceMeasureDelegate::QDistanceMeasureDelegate(QObject *parent)
	: QObject(parent)
	, m_pQDistanceMeausre_KS10R(NULL)
{
	m_pQDistanceMeausre_KS10R = new QDistanceMeausre_KS10R();
}

QDistanceMeasureDelegate::~QDistanceMeasureDelegate()
{
	if (m_pQDistanceMeausre_KS10R)
	{
		delete m_pQDistanceMeausre_KS10R;
		m_pQDistanceMeausre_KS10R = NULL;
	}
}

int QDistanceMeasureDelegate::start()
{
	if (!m_pQDistanceMeausre_KS10R)
		return -1;
		
	return m_pQDistanceMeausre_KS10R->start();
}

void QDistanceMeasureDelegate::stop()
{
	if (!m_pQDistanceMeausre_KS10R)
		return  ;

	return m_pQDistanceMeausre_KS10R->stop();
}
