#include "openproject.h"
#include <qfiledialog.h>

OpenProject::OpenProject(QWidget *parent, const QList<QString> &lastOpened)
	: QDialog(parent)
	, isValid(false)
	, _lastOpened(lastOpened)
{
	ui.setupUi(this);
    ui.comboRecentProjects->addItems(_lastOpened);
}

OpenProject::~OpenProject()
{

}

void OpenProject::on_findprojectbutton_clicked()
{
    projectPath = QFileDialog::getOpenFileName(this, "Open project",
                                "",
                                "*.AssetsEditorProject");
    ui.findProjectText->setText(projectPath);
	//ui.projectFilePathText->setText(projectPath + ".AssetsEditorProject");
	//QFile(projectPath).open(QIODevice::ReadWrite);
}

void OpenProject::on_okButtonFindProject_clicked()
{
    if (projectPath.size() > 0 && QFile(projectPath).exists())
    {
        isValid = true;
        this->close();
    }
}

void OpenProject::on_recentProjectOkBtn_clicked()
{
    projectPath = _lastOpened.at(ui.comboRecentProjects->currentIndex());
    if (projectPath.size() > 0 && QFile(projectPath).exists())
    {
        isValid = true;
        this->close();
    }
}

void OpenProject::on_cancelButton_clicked()
{
    isValid = false;
    this->close();
}
