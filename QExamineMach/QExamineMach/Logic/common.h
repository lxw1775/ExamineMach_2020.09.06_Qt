
#pragma once

#include <QString>
#include <QByteArray>


#define localConfigName "localCfg.ini"

QString getLocalCfgPath();

namespace Measure{

//另一个 函数 char 转为 16进制
char convertCharToHex(char ch);

//基本和单片机交互 数据 都是16进制的 我们这里自己写一个 Qstring 转为 16进制的函数	
void convertStringToHex(const QString& str, QByteArray& byteData);

}