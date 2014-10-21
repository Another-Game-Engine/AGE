#ifndef PROJECTCREATIONFORM_H
#define PROJECTCREATIONFORM_H

#include <QWidget>
#include "ui_projectcreationform.h"
#include <qdialog.h>
#include <qstring.h>
#include <qfiledialog.h>

class ProjectCreationForm : /*public QWidget,*/ public QDialog
{
	Q_OBJECT

public:
	ProjectCreationForm(QWidget *parent = 0);
	~ProjectCreationForm();
	QString projectPath;
	QString rawDir;
	QString cookedDir;
	bool isValid;
private slots:
    void on_createProjectFileBtn_clicked();

    void on_selectRawDirBtn_clicked();

    void on_selectCookedDirBtn_clicked();

    void on_saveBtn_clicked();

    void on_cookedDirPathText_textChanged(const QString &arg1);

    void on_rawDirPathText_textChanged(const QString &arg1);

    void on_projectFilePathText_textChanged(const QString &arg1);

private:
	Ui::ProjectCreationForm ui;
};

#endif // PROJECTCREATIONFORM_H
