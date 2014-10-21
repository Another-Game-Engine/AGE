#include "qtassetseditor.h"

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

}
