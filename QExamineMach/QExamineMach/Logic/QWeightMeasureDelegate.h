/*
	逻辑层 体重测量 代理
*/

#pragma once
#include <QObject>

class QWeightMeasure_Youjian;
class QWeightMeasureDelegate : public QObject
{
	Q_OBJECT

private:
	QWeightMeasureDelegate(QObject* parent = NULL);
public:
	~QWeightMeasureDelegate();
	static QWeightMeasureDelegate* GetInstance();

private:
	static QWeightMeasureDelegate* m_instance_ptr;

	QWeightMeasure_Youjian* m_pQWeightMeasure_Youjian;
};
