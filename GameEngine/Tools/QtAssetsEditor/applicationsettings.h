#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <qfile.h>

class ApplicationSettings
{
public:
    ApplicationSettings();
	bool init();
	void save() const;
	inline const QList<QString> &getLastOpened() const { return _lastOpened; }
	void addProject(const QString &path);
private:
	QList < QString > _lastOpened;
};

#endif // APPLICATIONSETTINGS_H
