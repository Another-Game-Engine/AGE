#include "applicationsettings.h"
#include <qapplication.h>
#include <qsettings>
#include <qdebug.h>

ApplicationSettings::ApplicationSettings()
{
}

bool ApplicationSettings::init()
{
	QSettings settings;

	auto size = settings.beginReadArray("LastOpened");
	if (size != -1)
	{
		for (int i = 0; i < size; ++i)
		{
			settings.setArrayIndex(i);
			_lastOpened.push_back(settings.value("path").toString());
		}
	}
	settings.endArray();

	return true;
}

void ApplicationSettings::save() const
{
	QSettings settings;

	settings.beginWriteArray("LastOpened");
	for (auto i = 0; i < _lastOpened.size(); ++i)
	{
		settings.setArrayIndex(i);
		settings.setValue("path", _lastOpened.at(i));
	}
	settings.endArray();
	settings.sync();
}

void ApplicationSettings::addProject(const QString &path)
{
	for (auto &e : _lastOpened)
		if (e == path)
			return;
	_lastOpened.push_front(path);
}
