#ifndef OPENPROJECT_H
#define OPENPROJECT_H

#include <qdialog.h>
#include "ui_openproject.h"
#include <qstring.h>
#include <qwidget.h>
#include <qlist.h>

class OpenProject : public QDialog
{
	Q_OBJECT

public:
	OpenProject(QWidget *parent, const QList<QString> &lastOpened);
	~OpenProject();
	QString projectPath;
	bool isValid;


private slots:
    void on_findprojectbutton_clicked();

    void on_okButtonFindProject_clicked();

    void on_recentProjectOkBtn_clicked();

    void on_cancelButton_clicked();

private:
	Ui::OpenProject ui;
	QList<QString> _lastOpened;
};

#endif // OPENPROJECT_H
