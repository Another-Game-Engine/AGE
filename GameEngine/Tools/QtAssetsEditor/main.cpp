#include "qtassetseditor.h"
#include <QtWidgets/QApplication>
#include <qdir.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtAssetsEditor w;

	QDir rawDirectory;
	QDir cookDirectory;

	auto &settings = w.getSettings();
	if (!settings.init())
		return 0;

	w.show();
	auto returnValue = a.exec();

	settings.save();
	return returnValue;
}
