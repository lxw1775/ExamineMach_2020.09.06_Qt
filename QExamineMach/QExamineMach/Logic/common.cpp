#include "common.h"
#include <QCoreApplication>

QString getLocalCfgPath()
{
	return QCoreApplication::applicationDirPath() + "/" + localConfigName;
}

namespace Measure {
	//另一个 函数 char 转为 16进制
	char convertCharToHex(char ch)
	{
		/*
		0x30等于十进制的48，48也是0的ASCII值，，
		1-9的ASCII值是49-57，，所以某一个值－0x30，，
		就是将字符0-9转换为0-9

		*/
		if ((ch >= '0') && (ch <= '9'))
			return ch - 0x30;
		else if ((ch >= 'A') && (ch <= 'F'))
			return ch - 'A' + 10;
		else if ((ch >= 'a') && (ch <= 'f'))
			return ch - 'a' + 10;
		else return (-1);
	}

	//基本和单片机交互 数据 都是16进制的 我们这里自己写一个 Qstring 转为 16进制的函数	
	void convertStringToHex(const QString& str, QByteArray& byteData)
	{
		int hexdata, lowhexdata;
		int hexdatalen = 0;
		int len = str.length();
		byteData.resize(len / 2);
		char lstr, hstr;
		for (int i = 0; i < len; )
		{
			//char lstr,
			hstr = str[i].toLatin1();
			if (hstr == ' ')
			{
				i++;
				continue;
			}
			i++;
			if (i >= len)
				break;
			lstr = str[i].toLatin1();
			hexdata = convertCharToHex(hstr);
			lowhexdata = convertCharToHex(lstr);
			if ((hexdata == 16) || (lowhexdata == 16))
				break;
			else
				hexdata = hexdata * 16 + lowhexdata;
			i++;
			byteData[hexdatalen] = (char)hexdata;
			hexdatalen++;
		}
		byteData.resize(hexdatalen);
	}

}