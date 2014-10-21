#ifndef QTASSETSEDITOR_H
#define QTASSETSEDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_qtassetseditor.h"
#include "applicationsettings.h"

class QtAssetsEditor : public QMainWindow
{
	Q_OBJECT

public:
	QtAssetsEditor(QWidget *parent = 0);
	~QtAssetsEditor();
	inline ApplicationSettings &getSettings() { return _settings; }

	void createProject(const QString &projectPath, const QString &rawPath, const QString &cookedPath);
private:
	Ui::QtAssetsEditorClass ui;
	ApplicationSettings _settings;
};

#endif // QTASSETSEDITOR_H
