#include "applicationsettings.h"
#include <qapplication.h>
#include <qsettings>
#include <qdebug.h>

ApplicationSettings::ApplicationSettings()
	: _settingsFilePath("")
{
}

bool ApplicationSettings::init()
{
	_settingsFilePath = QApplication::applicationDirPath() + "/assetsEditorSettings.ini";

	QFile settingsFile(_settingsFilePath);

	if (!settingsFile.exists())
	{
		if (!settingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
			return false;
		settingsFile.close();
	}

	QSettings settings(_settingsFilePath, QSettings::NativeFormat);

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
	QSettings settings(_settingsFilePath, QSettings::NativeFormat);

	settings.beginWriteArray("LastOpened");
	for (auto i = 0; i < _lastOpened.size(); ++i)
	{
		settings.setArrayIndex(i);
		settings.setValue("path", _lastOpened.at(i));
	}
	settings.endArray();
}

void ApplicationSettings::addProject(const QString &path)
{
	for (auto &e : _lastOpened)
		if (e == path)
			return;
	_lastOpened.push_front(path);
}
