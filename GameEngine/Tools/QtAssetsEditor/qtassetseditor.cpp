#include "qtassetseditor.h"
#include "projectcreationform.h"
#include "openproject.h"

QtAssetsEditor::QtAssetsEditor(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("AGE : Assets Editor");
}

QtAssetsEditor::~QtAssetsEditor()
{
}

void QtAssetsEditor::createProject(const QString &projectPath, const QString &rawPath, const QString &cookedPath)
{
	if (_project)
		_project->save();
	_project = std::make_unique<AssetsEditorProject>(this, projectPath, rawPath, cookedPath);
}

void QtAssetsEditor::openProject(const QString &projectPath)
{
	if (_project)
		_project->save();
	_project = std::make_unique<AssetsEditorProject>(this, projectPath);
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
