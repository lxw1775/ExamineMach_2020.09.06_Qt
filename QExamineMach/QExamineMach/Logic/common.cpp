#include "common.h"
#include <QCoreApplication>

QString getLocalCfgPath()
{
	return QCoreApplication::applicationDirPath() + "/" + localConfigName;
}

namespace Measure {
	//��һ�� ���� char תΪ 16����
	char convertCharToHex(char ch)
	{
		/*
		0x30����ʮ���Ƶ�48��48Ҳ��0��ASCIIֵ����
		1-9��ASCIIֵ��49-57��������ĳһ��ֵ��0x30����
		���ǽ��ַ�0-9ת��Ϊ0-9

		*/
		if ((ch >= '0') && (ch <= '9'))
			return ch - 0x30;
		else if ((ch >= 'A') && (ch <= 'F'))
			return ch - 'A' + 10;
		else if ((ch >= 'a') && (ch <= 'f'))
			return ch - 'a' + 10;
		else return (-1);
	}

	//�����͵�Ƭ������ ���� ����16���Ƶ� ���������Լ�дһ�� Qstring תΪ 16���Ƶĺ���	
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