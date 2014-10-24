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

	bool createProject(const QString &projectPath, const QString &rawPath, const QString &cookedPath);
	bool openProject(const QString &projectPath);
private slots:
    void on_actionOpen_project_triggered();

    void on_actionCreate_project_triggered();

	virtual void closeEvent(QCloseEvent *event);
private:
	void createRawView(const QString &rawPath);
	void createCookedView(const QString &path);
private:
	Ui::QtAssetsEditorClass ui;
	ApplicationSettings _settings;
	std::unique_ptr<AssetsEditorProject> _project;

	QDockWidget *_dock;
	QDockWidget *_assetPropertyDock;

	QSplitter *_splitter;

	QFileSystemModel *_rawFileSystemModel;
	QTreeView *_rawTreeView;

	QStringList _rawMaterialFilters;
	QStringList _rawMeshFilters;
	QStringList _rawTextureFilters;

	QFileSystemModel *_cookedFileSystemModel;
	QTreeView *_cookedTreeView;
};

#endif // QTASSETSEDITOR_H
