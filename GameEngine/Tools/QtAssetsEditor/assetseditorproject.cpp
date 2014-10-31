#include "assetseditorproject.h"
#include <qfile.h>
#include <qsettings.h>

AssetsEditorProject::AssetsEditorProject(
	QObject *parent
	, const QString &projectPath
	, const QString &rawPath
	, const QString &cookedPath)
	: QObject(parent)
	, _path(projectPath)
	, _rawPath(rawPath)
	, _cookedPath(cookedPath)
{
}

AssetsEditorProject::AssetsEditorProject(
	QObject *parent
	, const QString &projectPath
	)
	: QObject(parent)
	, _path(projectPath)
	, _rawPath("")
	, _cookedPath("")
{

}

AssetsEditorProject::~AssetsEditorProject()
{
	save();
}

bool AssetsEditorProject::save()
{
	QFile settingsFile(_path);

	if (!settingsFile.exists())
	{
		if (!settingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
			return false;
		settingsFile.close();
	}

	QSettings settings(_path, QSettings::IniFormat);
	settings.setValue("path", _path);
	settings.setValue("rawPath", _rawPath);
	settings.setValue("cookedPath", _cookedPath);
	return true;
}

bool AssetsEditorProject::load()
{
	QFile settingsFile(_path);

	if (!settingsFile.exists())
	{
		if (!settingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
			return false;
		settingsFile.close();
	}

	QSettings settings(_path, QSettings::IniFormat);
	_path = settings.value("path").toString();
	_rawPath = settings.value("rawPath").toString();
	_cookedPath = settings.value("cookedPath").toString();
	return true;
}