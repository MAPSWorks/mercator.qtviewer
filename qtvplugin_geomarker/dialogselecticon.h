#ifndef DIALOGSELECTICON_H
#define DIALOGSELECTICON_H

#include <QDialog>
#include "geographicspixmapitem.h"
namespace Ui {
	class DialogSelectIcon;
}

class DialogSelectIcon : public QDialog
{
	Q_OBJECT

public:
	explicit DialogSelectIcon(QString iniFile,QWidget *parent = 0);
	~DialogSelectIcon();

	QTVP_GEOMARKER::tag_icon m_icon;
private:
	QString iniFileName;
	Ui::DialogSelectIcon *ui;
protected slots:
	void on_pushButton_QTV_ok_clicked();
	void on_pushButton_QTV_cancel_clicked();
	void on_toolButton_QTV_browser_clicked();
};

#endif // DIALOGSELECTICON_H
