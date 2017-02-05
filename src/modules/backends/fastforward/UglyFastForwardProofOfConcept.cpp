/**************************************************************************
* Otter Browser: Web browser controlled by the user, not vice-versa.
* Copyright (C) 2017 Michal Dutkiewicz aka Emdek <michal@emdek.pl>
* Copyright (C) 2017 Piktas Zuikis <piktas.zuikis@inbox.lt>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
**************************************************************************/

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

QString serializeGroup(Settings &settings, QString groupName)
{
	settings.beginGroup(groupName);

	QStringList keys = settings.getKeys();

	QJsonArray array;
	for (int i = 0; i < keys.length(); ++i)
	{
		QJsonObject item
		{
			{ QLatin1Literal("value"), keys.at(i).toUpper() },
			{ QLatin1Literal("score"), settings.getValue(keys.at(i)).toInt() },
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
				.replace(QLatin1String("{hrefTokens}"), serializeGroup(settings, QLatin1String("Href")))
				.replace(QLatin1String("{classTokens}"), serializeGroup(settings, QLatin1String("Class")))
				.replace(QLatin1String("{idTokens}"), serializeGroup(settings, QLatin1String("Id")))
				.replace(QLatin1String("{textTokens}"), serializeGroup(settings, QLatin1String("Text")))
			;
		}

	}

	return m_script;
}

QString UglyFastForwardProofOfConcept::getHasFastForwardScript()
{
	return getScript().replace(QLatin1String("{isSelectingTheBestLink}"), QLatin1String("false"));
}

QString UglyFastForwardProofOfConcept::getFastForwardLinkScript()
{
	return getScript().replace(QLatin1String("{isSelectingTheBestLink}"), QLatin1String("true"));
}

}
