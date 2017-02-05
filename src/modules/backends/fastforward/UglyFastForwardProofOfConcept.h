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

#ifndef OTTER_UGLYFASTFORWARDPROOFOFCONCEPT_H
#define OTTER_UGLYFASTFORWARDPROOFOFCONCEPT_H

#include <QtCore/QObject>

namespace Otter
{

class UglyFastForwardProofOfConcept : public QObject
{
	Q_OBJECT

public:
	QString getHasFastForwardScript();
	QString getFastForwardLinkScript();
	static UglyFastForwardProofOfConcept *getInstance();

protected:
	explicit UglyFastForwardProofOfConcept(QObject *parent = nullptr);

private:
	QString getScript();

	static UglyFastForwardProofOfConcept *m_instance;
	QString m_script;
};

}

#endif
