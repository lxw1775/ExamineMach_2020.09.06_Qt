#pragma once

#include <QObject>

enum DistanceState {
	eEdge = 0, //边缘 准备测量
	eNear,	//较近 播放近广告
	eRemote,	//较远 播放远广告
};

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
signals:
	void sgnDistanceStateChanged(DistanceState old, DistanceState e);

private slots:
	void OnRealData(int);

private:
	static QDistanceMeasureDelegate* m_instance_ptr;
	QDistanceMeausre_KS10R* m_pQDistanceMeausre_KS10R;

	int m_nearDis, m_edgeDis, m_remoteDis;
	QString m_port;
	DistanceState m_preDisState;
};
