#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSettings>

#include "UglyFastForwardProofOfConcept.h"
#include "../../../core/Settings.h"
#include "../../../core/SessionsManager.h"

namespace Otter
{

UglyFastForwardProofOfConcept* Otter::UglyFastForwardProofOfConcept::m_instance(nullptr);

UglyFastForwardProofOfConcept::UglyFastForwardProofOfConcept(QObject *parent)
{

}

QString serializeGroup(Settings &settings, QString groupName, int defaultWeight)
{
	settings.beginGroup(groupName);

	QStringList keys = settings.getKeys();

	QJsonArray array;
	for(int i = 0; i < keys.length(); i++)
	{
		QVariant weight = settings.getValue(keys[i]);
		weight.convert(QVariant::Int);

		if(weight.isNull())
		{
			weight.setValue(defaultWeight);
		}

		QJsonObject item
		{
			{ "value", keys[i].toUpper() },
			{ "score", weight.toInt() },
		};

		array.append(item);
	}

	settings.endGroup();

	return QString::fromUtf8(QJsonDocument(array).toJson(QJsonDocument::Compact));
}

UglyFastForwardProofOfConcept *Otter::UglyFastForwardProofOfConcept::getInstance()
{
	if(!m_instance)
	{
		m_instance = new UglyFastForwardProofOfConcept();
	}

	return m_instance;
}

QString UglyFastForwardProofOfConcept::getScript()
{
	if(m_script.isEmpty())
	{
		//I'm sure there is a better way to read ini file from ~/.config/otter
		QString iniFile = SessionsManager::getProfilePath() + QDir::separator() + QLatin1String("fastforward.ini");
		if(!QFile(iniFile).exists()){
			QFile(SessionsManager::getReadableDataPath(QLatin1String("fastforward.ini"))).copy(iniFile);
		}

		Settings settings(iniFile);
		QJsonObject options;

		settings.beginGroup(QLatin1String("Fast forward"));
		options.insert("threshold", settings.getValue("Threshold", QVariant(10)).toInt());
		options.insert("relScore", settings.getValue("RelScore", QVariant(10)).toInt());
		options.insert("debug", settings.getValue("Debug", QVariant(10)).toBool());
		settings.endGroup();

		QString hrefs = serializeGroup(settings, QLatin1String("Href"), 1);
		QString classes = serializeGroup(settings, QLatin1String("Class"), 10);
		QString ids = serializeGroup(settings, QLatin1String("Id"), 11);
		QString texts = serializeGroup(settings, QLatin1String("Text"), 100);

		QString jsFilePath = SessionsManager::getReadableDataPath(QLatin1String("fastforward.js"));

		QFile jsFile(jsFilePath);
		if(jsFile.open(QFile::ReadOnly))
		{
			QString js = jsFile.readAll();

			m_script = js
				.replace(QLatin1String("%options%"), QJsonDocument(options).toJson(QJsonDocument::Compact))
				.replace(QLatin1String("%hrefTokens%"), hrefs)
				.replace(QLatin1String("%classTokens%"), classes)
				.replace(QLatin1String("%idTokens%"), ids)
				.replace(QLatin1String("%textTokens%"), texts)
			;
			//qDebug() << m_script;
		}

	}

	return m_script;
}

QString UglyFastForwardProofOfConcept::getHasFastForwardScript()
{
	if(m_hasFastForward.isEmpty()){
		m_hasFastForward = getScript();

		if(!m_hasFastForward.isEmpty())
		{
			m_hasFastForward.append(QLatin1String(".hasFastForward()"));
		}
	}

	return m_hasFastForward;
}

QString UglyFastForwardProofOfConcept::getFastForwardLinkScript()
{
	if(m_fastForwardLink.isEmpty()){
		m_fastForwardLink = getScript();

		if(!m_fastForwardLink.isEmpty())
		{
			m_fastForwardLink.append(QLatin1String(".getFastForward()"));
		}
	}

	return m_fastForwardLink;
}

}
