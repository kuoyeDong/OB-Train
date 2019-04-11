#include "jsapiobject.h"
#include "serialtest.h"


jsApiObject::jsApiObject(QObject *parent) :
	QObject(parent),
	m_Log(""),
	m_Code("Initializa...")
{

}

void jsApiObject::setCanStartWork()
{

}

void jsApiObject::generalLog(QString someLog)
{
	setLog(someLog);
}

void jsApiObject::generalCode(QString someCode)
{
	setCode(someCode);
}

void jsApiObject::runBlockly(QString code)
{
	QStringList codes = code.split(QRegExp("\n"));
	QStringList goalCodes;
	int len = codes.length() - 1;
	//看见分行即分行，实现for时注意
	//此布尔值用于判断是否包含等待，如不包含则循环取值判断时不需要再做重复判断
	bool hasWait = code.contains("wait");
	/**
	 * @brief goalCodeSum 用于显示抽取后的目标执行代码
	 */
	QString goalCodeSum;
	for (int index = 0; index < len; index++) {
		QString indexCode = codes.value(index);
		if (indexCode.contains("\"")) {
			int sublen = indexCode.length() - 1 - 1;
			if (sublen > 1) {
				QString goalCode = indexCode.mid(1, sublen);
				goalCodes.append(goalCode);
				goalCodeSum.append(indexCode + "\n");
			}
		}
	}
	setLog(QStringLiteral("暂不支持混合模式，\n已抽取OBDroneAPI模块代码执行,\n抽取后代码如下:\n") + goalCodeSum);
	int cmdListLen = goalCodes.length();
	for (int index = 0; index < cmdListLen; index++) {
		QString cmd = goalCodes.value(index);
		if (hasWait&&cmd.contains("wait")) {
			int subStart = cmd.indexOf("t") + 1;
			int subLen = cmd.indexOf("s") - subStart;
			QString waitSecStr = cmd.mid(subStart, subLen);
			bool ok;
			uint waitSec = waitSecStr.toUInt(&ok, 10);
			QThread::currentThread()->sleep(waitSec);
			qDebug() << "waitSec==" + waitSecStr;
		}
		else {
			SerialTest serialTest;
			serialTest.sendto(cmd);
			qDebug() << cmd;
		}
	}
}

/*
void jsApiObject::saveBlockly(QString name, QString txt)
{

}
*/
void jsApiObject::loadUrl(QString url)
{

}


void origiValue() {

}
QString jsApiObject::receiveLog()
{
	return m_Log;
}

void jsApiObject::setLog(QString log)
{
	m_Log = log;
	emit LogChanged();
}

QString jsApiObject::receiveCode()
{
	return  m_Code;
}

void jsApiObject::setCode(QString code)
{
	m_Code = code;
	emit CodeChanged();
}

void jsApiObject::jsOnstart()
{
	emit onStart();
}
