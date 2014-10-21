#include "qtassetseditor.h"
#include "projectcreationform.h"
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

	if (settings.getLastOpenend().size() == 0)
	{
		ProjectCreationForm pcf;
		pcf.show();
		pcf.exec();
		if (pcf.isValid)
		{
			settings.addProject(pcf.projectPath);
			w.createProject(pcf.projectPath, pcf.rawDir, pcf.cookedDir);
		}
		else
			return 0;
	}

	w.show();
	auto returnValue = a.exec();

	settings.save();
	return returnValue;
}
