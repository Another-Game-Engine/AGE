#ifndef QTASSETSEDITOR_H
#define QTASSETSEDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_qtassetseditor.h"
#include "applicationsettings.h"
#include "assetseditorproject.h"
#include <memory>

class QtAssetsEditor : public QMainWindow
{
	Q_OBJECT

public:
	QtAssetsEditor(QWidget *parent = 0);
	~QtAssetsEditor();
	inline ApplicationSettings &getSettings() { return _settings; }

	void createProject(const QString &projectPath, const QString &rawPath, const QString &cookedPath);
	void openProject(const QString &projectPath);
private slots:
    void on_actionOpen_project_triggered();

    void on_actionCreate_project_triggered();

private:
	Ui::QtAssetsEditorClass ui;
	ApplicationSettings _settings;
	std::unique_ptr<AssetsEditorProject> _project;
};

#endif // QTASSETSEDITOR_H
