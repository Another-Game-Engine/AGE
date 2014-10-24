#include "qtassetseditor.h"
#include "projectcreationform.h"
#include "openproject.h"
#include <qtreeview.h>
#include <qdockwidget.h>
#include <qsettings.h>

QtAssetsEditor::QtAssetsEditor(QWidget *parent)
	: QMainWindow(parent)
{
	QCoreApplication::setOrganizationName("AGE");
    QCoreApplication::setOrganizationDomain("another game engine");
    QCoreApplication::setApplicationName("Assets Editor");

	ui.setupUi(this);
	this->setWindowTitle("AGE : Assets Editor");

	_fileSystemModel = nullptr;

	_dock = new QDockWidget(tr("dock widget title"), this);
	_dock->setAllowedAreas(Qt::LeftDockWidgetArea
		| Qt::RightDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, _dock);

	_splitter = new QSplitter(_dock);

	_rawTreeView = new QTreeView(_splitter);
	_dock->setWidget(_splitter);

	QSettings settings;
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());

}

QtAssetsEditor::~QtAssetsEditor()
{
}

void QtAssetsEditor::createProject(const QString &projectPath, const QString &rawPath, const QString &cookedPath)
{
	if (_project)
		_project->save();
	_project = std::make_unique<AssetsEditorProject>(this, projectPath, rawPath, cookedPath);
	createRawView(projectPath);
}

void QtAssetsEditor::openProject(const QString &projectPath)
{
	if (_project)
		_project->save();
	_project = std::make_unique<AssetsEditorProject>(this, projectPath);
	createRawView(projectPath);
}

void QtAssetsEditor::createRawView(const QString &rawPath)
{
	if (_fileSystemModel)
		delete _fileSystemModel;
	_fileSystemModel = new QFileSystemModel();
	_fileSystemModel->setRootPath(rawPath);
	_rawTreeView->setModel(_fileSystemModel);
	_rawTreeView->setRootIndex(_fileSystemModel->index(rawPath));
}

void QtAssetsEditor::on_actionOpen_project_triggered()
{
	OpenProject o(this, this->_settings.getLastOpened());
	o.setModal(true);
	o.show();
	o.exec();
	if (o.isValid)
	{
		_settings.addProject(o.projectPath);
		openProject(o.projectPath);
	}
}

void QtAssetsEditor::on_actionCreate_project_triggered()
{
    ProjectCreationForm pcf;
	pcf.setModal(true);
    pcf.show();
    pcf.exec();
    if (pcf.isValid)
    { 
        _settings.addProject(pcf.projectPath);
        createProject(pcf.projectPath, pcf.rawDir, pcf.cookedDir);
    }
}

void QtAssetsEditor::closeEvent(QCloseEvent *event)
{
	QSettings settings;
	settings.setValue("geometry", saveGeometry());
	settings.setValue("windowState", saveState());
	QMainWindow::closeEvent(event);
}
