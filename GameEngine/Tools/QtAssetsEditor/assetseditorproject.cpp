#include "assetseditorproject.h"

//AssetsEditorProject::AssetsEditorProject(QObject *parent)
//	: QObject(parent)
//{
//
//}

AssetsEditorProject::AssetsEditorProject(
	QObject *parent
	, const QString &projectPath
	, const QString &rawPath
	, const QString &cookedPath)
	: QObject(parent)
	, _path(projectPath)
	, _rawPath(rawPath)
	, _cookedPath(cookedPath)
{

}

AssetsEditorProject::AssetsEditorProject(
	QObject *parent
	, const QString &projectPath
	)
	: QObject(parent)
	, _path(projectPath)
	, _rawPath("")
	, _cookedPath("")
{

}

AssetsEditorProject::~AssetsEditorProject()
{

}

void AssetsEditorProject::save()
{}