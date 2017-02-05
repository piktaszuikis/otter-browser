#include <QDir>
#include <QDebug>
#include <QFile>
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
	for (int i = 0; i < keys.length(); ++i)
	{
		QVariant weight = settings.getValue(keys.at(i));

		weight.convert(QVariant::Int);

		if (weight.isNull())
		{
			weight.setValue(defaultWeight);
		}

		QJsonObject item
		{
			{ QLatin1Literal("value"), keys.at(i).toUpper() },
			{ QLatin1Literal("score"), weight.toInt() },
		};

		array.append(item);
	}

	settings.endGroup();

	return QString::fromUtf8(QJsonDocument(array).toJson(QJsonDocument::Compact));
}

UglyFastForwardProofOfConcept *Otter::UglyFastForwardProofOfConcept::getInstance()
{
	if (!m_instance)
	{
		m_instance = new UglyFastForwardProofOfConcept();
	}

	return m_instance;
}

QString UglyFastForwardProofOfConcept::getScript()
{
	if (m_script.isEmpty())
	{
		QString jsFilePath = SessionsManager::getReadableDataPath(QLatin1String("fastforward.js"));
		QString iniFile = SessionsManager::getReadableDataPath(QLatin1String("fastforward.ini"));
		Settings settings(iniFile);

		QFile jsFile(jsFilePath);
		if (jsFile.open(QFile::ReadOnly))
		{
			QString js = jsFile.readAll();

			m_script = js
				.replace(QLatin1String("{hrefTokens}"), serializeGroup(settings, QLatin1String("Href"), 1))
				.replace(QLatin1String("{classTokens}"), serializeGroup(settings, QLatin1String("Class"), 10))
				.replace(QLatin1String("{idTokens}"), serializeGroup(settings, QLatin1String("Id"), 11))
				.replace(QLatin1String("{textTokens}"), serializeGroup(settings, QLatin1String("Text"), 100))
			;
		}

	}

	return m_script;
}

QString UglyFastForwardProofOfConcept::getHasFastForwardScript()
{
	if (m_hasFastForward.isEmpty())
	{
		m_hasFastForward = getScript().replace(QLatin1String("{isSelectTheBestLink}"), QLatin1String("false"));
	}

	return m_hasFastForward;
}

QString UglyFastForwardProofOfConcept::getFastForwardLinkScript()
{
	if (m_fastForwardLink.isEmpty())
	{
		m_fastForwardLink = getScript().replace(QLatin1String("{isSelectTheBestLink}"), QLatin1String("true"));
	}

	return m_fastForwardLink;
}

}
