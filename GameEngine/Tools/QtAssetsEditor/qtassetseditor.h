#ifndef QTASSETSEDITOR_H
#define QTASSETSEDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_qtassetseditor.h"
#include "applicationsettings.h"
#include "assetseditorproject.h"
#include <memory>
#include <qsplitter.h>
#include <qfilesystemmodel.h>

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

	virtual void closeEvent(QCloseEvent *event);
private:
	void createRawView(const QString &rawPath);
private:
	Ui::QtAssetsEditorClass ui;
	ApplicationSettings _settings;
	std::unique_ptr<AssetsEditorProject> _project;

	QDockWidget *_dock;

	QSplitter *_splitter;
	QFileSystemModel *_fileSystemModel;
	QTreeView *_rawTreeView;
};

#endif // QTASSETSEDITOR_H
