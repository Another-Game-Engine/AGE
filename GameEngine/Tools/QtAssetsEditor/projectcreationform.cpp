#include "projectcreationform.h"
#include <qdial.h>

ProjectCreationForm::ProjectCreationForm(QWidget *parent)
	: QDialog(parent)
	, isValid(false)
{
	ui.setupUi(this);
	projectPath = "C:/Users/cesar/Desktop/ASSET_EDITOR_TESTS/test.AssetsEditorProject";
	rawDir = "C:/Users/cesar/Desktop/ASSET_EDITOR_TESTS/RAW";
	cookedDir = "C:/Users/cesar/Desktop/ASSET_EDITOR_TESTS/COOKED";
}

ProjectCreationForm::~ProjectCreationForm()
{

}

void ProjectCreationForm::on_createProjectFileBtn_clicked()
{
    projectPath = QFileDialog::getSaveFileName(this, "Create File",
                                "",
                                "*.AssetsEditorProject");
	ui.projectFilePathText->setText(projectPath + ".AssetsEditorProject");
	QFile(projectPath).open(QIODevice::ReadWrite);
}

void ProjectCreationForm::on_selectRawDirBtn_clicked()
{
    rawDir = QFileDialog::getExistingDirectory(this, "Select Raw Directory",
                                "");
	ui.rawDirPathText->setText(rawDir);
}

void ProjectCreationForm::on_selectCookedDirBtn_clicked()
{
    cookedDir = QFileDialog::getExistingDirectory(this, "Select Cooked Directory",
                                "");
	ui.cookedDirPathText->setText(cookedDir);
}

void ProjectCreationForm::on_saveBtn_clicked()
{
	if (
		!projectPath.isEmpty()
		&& !rawDir.isEmpty()
		&& !cookedDir.isEmpty()
		&& QFile(projectPath).open(QIODevice::ReadOnly)
		&& QDir(rawDir).exists()
		&& QDir(cookedDir).exists()
		)
	{
		isValid = true;
		this->close();
	}
}

void ProjectCreationForm::on_cookedDirPathText_textChanged(const QString &arg1)
{
    cookedDir = arg1;
}

void ProjectCreationForm::on_rawDirPathText_textChanged(const QString &arg1)
{
    rawDir = arg1;
}

void ProjectCreationForm::on_projectFilePathText_textChanged(const QString &arg1)
{
    projectPath = arg1;
}
