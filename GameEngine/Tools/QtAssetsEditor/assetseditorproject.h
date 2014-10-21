#ifndef ASSETSEDITORPROJECT_H
#define ASSETSEDITORPROJECT_H

#include <QObject>

class AssetsEditorProject : public QObject
{
	Q_OBJECT

public:
//	AssetsEditorProject(QObject *parent) = delete;
	AssetsEditorProject(QObject *parent, const QString &projectPath, const QString &rawPath, const QString &cookedPath); // used at creation
	AssetsEditorProject(QObject *parent, const QString &projectPath); // used at load
	~AssetsEditorProject();
	void save();
private:
	QString _path;
	QString _rawPath;
	QString _cookedPath;
};

#endif // ASSETSEDITORPROJECT_H
