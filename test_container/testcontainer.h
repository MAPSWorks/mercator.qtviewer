#ifndef TESTCONTAINER_H
#define TESTCONTAINER_H

#include <QDialog>
#include <QStandardItemModel>
namespace Ui {
	class testcontainer;
}

class testcontainer : public QDialog
{
	Q_OBJECT
private:
	QStandardItemModel * m_pModel;
	QString map_to_string(const QMap<QString, QVariant> & m);
	QMap<QString, QVariant> string_to_map(const QString & s);
public:
	explicit testcontainer(QWidget *parent = 0);
	~testcontainer();
protected:
	void closeEvent(QCloseEvent *);
	void timerEvent(QTimerEvent *);
private:
	Ui::testcontainer *ui;
	int m_nAnTimer;
	//QString m_str_gridLayerName;
	//QString m_str_markerLayerName;
	//void confirmLayerNames();
	void show_message(QString);
#ifdef _WIN32
protected slots:
	void slot_message(QString);
#endif
protected slots:
	void on_pushButton_QTV_test_adds_clicked();
	void on_pushButton_QTV_test_cache_clicked();
	void on_pushButton_QTV_test_autodl_clicked();
	void on_pushButton_QTV_test_navigate_clicked();
	void on_pushButton_QTV_test_layers_clicked();
	void on_pushButton_QTV_test_layer_move_clicked();
	void on_pushButton_QTV_test_grid_enable_clicked();
	void on_pushButton_QTV_test_grid_getPolygon_clicked();
	void on_pushButton_QTV_test_mark_clicked();
	void on_pushButton_QTV_test_line_clicked();
	void on_pushButton_QTV_test_polygon_clicked();
	void on_pushButton_QTV_test_request_clicked();
	void on_pushButton_QTV_test_xml_clicked();
	void on_pushButton_QTV_test_resource_clicked();
	void on_pushButton_QTV_test_geo_displayMod_clicked();
	void on_pushButton_QTV_test_geo_selectionMod_clicked();
	void on_pushButton_QTV_test_geo_selected_marks_clicked();
	void on_pushButton_QTV_test_geo_clear_sel_clicked();
	void on_pushButton_QTV_test_geo_del_sel_clicked();
	void on_pushButton_QTV_default_style_clicked();
	void on_osmmap_map_event(QMap<QString, QVariant> p);

};

#endif // TESTCONTAINER_H
