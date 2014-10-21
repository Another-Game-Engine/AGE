#include "assetseditorproject.h"

AssetsEditorProject::AssetsEditorProject(QObject *parent)
	: QObject(parent)
{

}

AssetsEditorProject::AssetsEditorProject(
	QObject *parent
	, const QString &projectPath
	, const QString &rawPath
	, const QString &cookedPath)
	: QObject(parent)
{

}

AssetsEditorProject::~AssetsEditorProject()
{

}

void AssetsEditorProject::save()
{}