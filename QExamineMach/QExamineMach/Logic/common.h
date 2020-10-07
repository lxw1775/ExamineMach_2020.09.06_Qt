
#pragma once

#include <QString>
#include <QByteArray>


#define localConfigName "localCfg.ini"

QString getLocalCfgPath();

namespace Measure{

//��һ�� ���� char תΪ 16����
char convertCharToHex(char ch);

//�����͵�Ƭ������ ���� ����16���Ƶ� ���������Լ�дһ�� Qstring תΪ 16���Ƶĺ���	
void convertStringToHex(const QString& str, QByteArray& byteData);

}