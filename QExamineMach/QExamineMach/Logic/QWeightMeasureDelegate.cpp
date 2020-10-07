#include "QWeightMeasureDelegate.h"
#include "QWeightMeasure_Youjian.h"
#include "common.h"
#include <QSettings>
#include <QDebug>

QWeightMeasureDelegate* QWeightMeasureDelegate::m_instance_ptr = nullptr;

QWeightMeasureDelegate* QWeightMeasureDelegate::GetInstance()
{
	if (m_instance_ptr == nullptr) {
		m_instance_ptr = new QWeightMeasureDelegate();
	}
	return m_instance_ptr;
}

QWeightMeasureDelegate::QWeightMeasureDelegate(QObject* parent)
	: QObject(parent)
{
	m_pQWeightMeasure_Youjian = new QWeightMeasure_Youjian;

	// 根据ini文件路径新建QSettings类
	QSettings m_IniFile(getLocalCfgPath(), QSettings::IniFormat);

	m_port = m_IniFile.value("SerialPort/Weight").toString();
}

QWeightMeasureDelegate::~QWeightMeasureDelegate()
{
	if (m_pQWeightMeasure_Youjian)
	{
		delete m_pQWeightMeasure_Youjian;
		m_pQWeightMeasure_Youjian = NULL;
	}
}

int QWeightMeasureDelegate::start()
{
	if (!m_pQWeightMeasure_Youjian)
		return -1;

	if (m_port == "")
	{
		qCritical() << "Weight Meausre start error, port=null";//日志
		return -1;
	}
	int iRet = m_pQWeightMeasure_Youjian->weightMeasureStart(m_port);
	if (iRet != 0)
	{
		qCritical() << "QWeightMeasure_Youjian start error, ret=" << iRet;//日志
		return iRet;
	}
	return iRet;
}

void QWeightMeasureDelegate::stop()
{
	if (!m_pQWeightMeasure_Youjian)
		return;

	return m_pQWeightMeasure_Youjian->stop();
}