#pragma once

#include <QObject>

class QDistanceMeausre_KS10R;
class QHeightMeasureDelegate : public QObject
{
	Q_OBJECT

private:
	QHeightMeasureDelegate(QObject* parent = NULL);
public:
	~QHeightMeasureDelegate();
	static QHeightMeasureDelegate* GetInstance();

private:
	static QHeightMeasureDelegate* m_instance_ptr;
	QDistanceMeausre_KS10R* m_pQDistanceMeausre_KS10R;

};
