#pragma once

#include <QObject>

class QDistanceMeausre_KS10R;
class QDistanceMeasureDelegate : public QObject
{
	Q_OBJECT

private:
	QDistanceMeasureDelegate(QObject* parent = NULL);
public:
	~QDistanceMeasureDelegate();
	static QDistanceMeasureDelegate* GetInstance();

	int start();
	void stop();

private:
	static QDistanceMeasureDelegate* m_instance_ptr;	
	QDistanceMeausre_KS10R* m_pQDistanceMeausre_KS10R;
};
