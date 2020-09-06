/*
	逻辑层 体重测量 代理
*/

#pragma once
#include <QObject>

class QWeightMeasure_Youjian;
class QWeightMeasureDelegate : public QObject
{
	Q_OBJECT

public:
	QWeightMeasureDelegate(QObject* parent = NULL);
	~QWeightMeasureDelegate();

private:
	QWeightMeasure_Youjian* m_pQWeightMeasure_Youjian;
};
