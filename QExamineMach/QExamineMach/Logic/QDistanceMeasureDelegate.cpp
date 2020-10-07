#include "QDistanceMeasureDelegate.h"
#include "QDistanceMeausre_KS10R.h"
#include <QSettings>
#include <qDebug>
#include "common.h"

QDistanceMeasureDelegate* QDistanceMeasureDelegate::m_instance_ptr = nullptr;

QDistanceMeasureDelegate* QDistanceMeasureDelegate::GetInstance()
{
	if (m_instance_ptr == nullptr) {
		m_instance_ptr = new QDistanceMeasureDelegate();
	}
	return m_instance_ptr;
}

QDistanceMeasureDelegate::QDistanceMeasureDelegate(QObject* parent)
	: QObject(parent)
	, m_pQDistanceMeausre_KS10R(NULL)
	, m_nearDis(0)
	, m_edgeDis(0)
	, m_remoteDis(0)
	, m_preDisState(eRemote)
{
	m_pQDistanceMeausre_KS10R = new QDistanceMeausre_KS10R();
	bool b = connect(m_pQDistanceMeausre_KS10R, SIGNAL(sgnRealData(int)), this, SLOT(OnRealData(int)));

	// 根据ini文件路径新建QSettings类
	QSettings m_IniFile(getLocalCfgPath(), QSettings::IniFormat);

	//通过setValue函数将键值对放在相对于的节下面
	//m_IniFile.setValue("DistanceSplit/edge", "键对应的值");
	//m_IniFile.setValue("DistanceSplit/near", "键对应的值");
	//m_IniFile.setValue("DistanceSplit/remote", "键对应的值");
	bool bOk = false;
	m_edgeDis = m_IniFile.value("DistanceSplit/edge").toInt(&bOk);
	if (!bOk)
		m_edgeDis = 500;
	m_nearDis = m_IniFile.value("DistanceSplit/near").toInt(&bOk);
	if (!bOk)
		m_nearDis = 5000;
	m_remoteDis = m_IniFile.value("DistanceSplit/remote").toInt(&bOk);
	if (!bOk)
		m_remoteDis = 10000;

	m_port = m_IniFile.value("SerialPort/Distance").toString();
}

QDistanceMeasureDelegate::~QDistanceMeasureDelegate()
{
	if (m_pQDistanceMeausre_KS10R)
	{
		disconnect(m_pQDistanceMeausre_KS10R, SIGNAL(sgnRealData(int)), this, SLOT(OnRealData(int)));
		delete m_pQDistanceMeausre_KS10R;
		m_pQDistanceMeausre_KS10R = NULL;
	}
}

int QDistanceMeasureDelegate::start()
{
	if (!m_pQDistanceMeausre_KS10R)
		return -1;

	if (m_port == "")
	{
		qCritical() << "Distance Meausre start error, port=null";//日志
		return -1;
	}

	int iRet = m_pQDistanceMeausre_KS10R->start(m_port);
	if (iRet != 0)
	{
		qCritical() << "QDistanceMeausre_KS10R start error, ret=" << iRet;//日志
		return iRet;
	}

	m_pQDistanceMeausre_KS10R->changeInterval(1500);
	return 0;
}

void QDistanceMeasureDelegate::stop()
{
	if (!m_pQDistanceMeausre_KS10R)
		return;

	return m_pQDistanceMeausre_KS10R->stop();
}

void QDistanceMeasureDelegate::OnRealData(int val)
{
	DistanceState state = eRemote;
	if (val <= m_edgeDis)
	{
		state = eEdge;
	}
	else if (val < m_nearDis)
	{
		state = eNear;
	}

	if (m_preDisState != state)
	{
		emit sgnDistanceStateChanged(m_preDisState, state);
		m_preDisState = state;
		m_pQDistanceMeausre_KS10R->changeInterval((state == eRemote) ? 1500 : 750);
	}

}