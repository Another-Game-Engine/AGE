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
