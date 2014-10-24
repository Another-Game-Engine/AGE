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

	_rawFileSystemModel = nullptr;
	_cookedFileSystemModel = nullptr;

	_dock = new QDockWidget(tr("Assets explorer"), this);
	_dock->setAllowedAreas(Qt::LeftDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea, _dock);

	_assetPropertyDock = new QDockWidget(tr("Asset Property"), this);
	_assetPropertyDock->setAllowedAreas(Qt::RightDockWidgetArea);
	addDockWidget(Qt::RightDockWidgetArea, _assetPropertyDock);

	_splitter = new QSplitter(_dock);

	_rawTreeView = new QTreeView(_splitter);

	_cookedTreeView = new QTreeView(_splitter);

	_dock->setWidget(_splitter);

	QSettings settings;
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("windowState").toByteArray());

	_rawMeshFilters << "*.fbx";
	_rawMeshFilters << "*.obj";
	_rawMaterialFilters << "*.mtl";
	_rawTextureFilters << "*.png";
	_rawTextureFilters << "*.jpg";
	_rawTextureFilters << "*.jpeg";
	_rawTextureFilters << "*.tga";
}

QtAssetsEditor::~QtAssetsEditor()
{
}

bool QtAssetsEditor::createProject(const QString &projectPath, const QString &rawPath, const QString &cookedPath)
{
	if (_project)
		_project->save();
	_project = std::make_unique<AssetsEditorProject>(this, projectPath, rawPath, cookedPath);
	auto returnValue = _project->save();
	createRawView(_project->getRawPath());
	createCookedView(_project->getCookedPath());
	setWindowTitle("AGE Assets Editor : " + projectPath);
	return returnValue;
}

bool QtAssetsEditor::openProject(const QString &projectPath)
{
	if (_project)
		_project->save();
	_project = std::make_unique<AssetsEditorProject>(this, projectPath);
	auto returnValue = _project->load();
	createRawView(_project->getRawPath());
	createCookedView(_project->getCookedPath());
	setWindowTitle("AGE Assets Editor : " + projectPath);
	return returnValue;
}

void QtAssetsEditor::createRawView(const QString &rawPath)
{
	if (_rawFileSystemModel)
		delete _rawFileSystemModel;
	_rawFileSystemModel = new QFileSystemModel();
	_rawFileSystemModel->setRootPath(rawPath);
	_rawFileSystemModel->setNameFilters(_rawMaterialFilters + _rawMeshFilters + _rawTextureFilters);
	_rawFileSystemModel->setNameFilterDisables(false);

	_rawTreeView->setModel(_rawFileSystemModel);
	_rawTreeView->setRootIndex(_rawFileSystemModel->index(rawPath));
	_rawTreeView->show();
}

void QtAssetsEditor::createCookedView(const QString &path)
{
	if (_cookedFileSystemModel)
		delete _cookedFileSystemModel;
	_cookedFileSystemModel = new QFileSystemModel();
	_cookedFileSystemModel->setRootPath(path);

	_cookedTreeView->setModel(_cookedFileSystemModel);
	_cookedTreeView->setRootIndex(_cookedFileSystemModel->index(path));
	_cookedTreeView->show();
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
