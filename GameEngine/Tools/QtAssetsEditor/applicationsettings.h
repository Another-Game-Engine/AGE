#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <qfile.h>

class ApplicationSettings
{
public:
    ApplicationSettings();
	bool init();
	void save() const;
	inline const QList<QString> &getLastOpenend() const { return _lastOpened; }
	void addProject(const QString &path);
private:
	QString _settingsFilePath;
	QList < QString > _lastOpened;
};

#endif // APPLICATIONSETTINGS_H
