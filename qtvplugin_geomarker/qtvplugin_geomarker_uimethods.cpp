#include "qtvplugin_geomarker.h"
#include "ui_qtvplugin_geomarker.h"
#include <QSettings>
#include <QColorDialog>
#include <QSet>
#include <QMap>
#include <QRegExp>
#include <QDebug>
#include "geographicsellipseitem.h"
#include "geographicsrectitem.h"
#include "geographicslineitem.h"
#include "geographicspolygonitem.h"

void		qtvplugin_geomarker::timerEvent(QTimerEvent * e)
{
	if (e->timerId()==m_nTimerID_refreshUI && m_bNeedRefresh)
	{
		m_bNeedRefresh = false;
		killTimer(m_nTimerID_refreshUI);
		m_nTimerID_refreshUI = -1;

		//refersh
		int rowCount = m_pGeoItemModel->rowCount();
		QList<QTVP_GEOMARKER::geoItemBase *> items = m_pScene->geo_items();
		int c = 0;
		foreach (QTVP_GEOMARKER::geoItemBase * item, items)
		{
			if (c < rowCount)
			{
				m_pGeoItemModel->setData(
							m_pGeoItemModel->index(c,0),
							item->item_name()
							);

				m_pGeoItemModel->setData(
							m_pGeoItemModel->index(c,1),
							QTVP_GEOMARKER::item_name_by_enum(item->item_type())
							);
				m_pGeoItemModel->setData(
							m_pGeoItemModel->index(c,2),
							item->prop_counts()
							);
			}
			else
			{
				m_pGeoItemModel->appendRow(new QStandardItem(item->item_name()));
				m_pGeoItemModel->setData(
							m_pGeoItemModel->index(c,1),
							QTVP_GEOMARKER::item_name_by_enum(item->item_type())
							);
				m_pGeoItemModel->setData(
							m_pGeoItemModel->index(c,2),
							item->prop_counts()
							);

			}
			++c;
		}
		if (c< rowCount)
			m_pGeoItemModel->removeRows(c, rowCount - c);
		m_nTimerID_refreshUI = startTimer(2000);
	}
}




void qtvplugin_geomarker::on_radioButton_tool_point_toggled(bool b)
{
	if (b) ui->stackedWidget_marks->setCurrentIndex(0);
}

void qtvplugin_geomarker::on_radioButton_tool_line_toggled(bool b )
{
	if (b) ui->stackedWidget_marks->setCurrentIndex(1);
}

void qtvplugin_geomarker::on_radioButton_tool_polygon_toggled(bool b)
{
	if (b) ui->stackedWidget_marks->setCurrentIndex(2);
}

void qtvplugin_geomarker::on_toolButton_selColor_pointEdge_clicked()
{
	QString str = ui->lineEdit_PointColorEdge->text();

	QColor col = QColorDialog::getColor(string2color(str),this,tr("Select Color"),QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
	if (col.isValid())
		ui->lineEdit_PointColorEdge->setText(color2string(col));
}
void qtvplugin_geomarker::on_toolButton_selColor_pointcolorFill_clicked()
{
	QString str = ui->lineEdit_PointColorFill->text();

	QColor col = QColorDialog::getColor(string2color(str),this,tr("Select Color"),QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
	if (col.isValid())
		ui->lineEdit_PointColorFill->setText(color2string(col));
}
void qtvplugin_geomarker::on_toolButton_selColor_regionEdge_clicked()
{
	QString str = ui->lineEdit_RegionColorEdge->text();
	QColor col = QColorDialog::getColor(string2color(str),this,tr("Select Color"),QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
	if (col.isValid())
		ui->lineEdit_RegionColorEdge->setText(color2string(col));

}

void qtvplugin_geomarker::on_toolButton_selColor_regionFill_clicked()
{
	QString str = ui->lineEdit_RegionColorFill->text();
	QColor col = QColorDialog::getColor(string2color(str),this,tr("Select Color"),QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
	if (col.isValid())
		ui->lineEdit_RegionColorFill->setText(color2string(col));
}

void qtvplugin_geomarker::on_pushButton_pickToLine1_clicked()
{
	ui->lineEdit_lineLat1->setText(ui->lineEdit_point_lat->text());
	ui->lineEdit_lineLon1->setText(ui->lineEdit_point_lon->text());
}

void qtvplugin_geomarker::on_pushButton_pickToLine2_clicked()
{
	ui->lineEdit_lineLat2->setText(ui->lineEdit_point_lat->text());
	ui->lineEdit_lineLon2->setText(ui->lineEdit_point_lon->text());
}

void qtvplugin_geomarker::on_toolButton_selColor_clicked()
{
	QString str = ui->lineEdit_lineColor->text();

	QColor col = QColorDialog::getColor(string2color(str),this,tr("Select Color"),QColorDialog::ShowAlphaChannel|QColorDialog::DontUseNativeDialog);
	if (col.isValid())
		ui->lineEdit_lineColor->setText(color2string(col));
}

void qtvplugin_geomarker::SaveSettingsToIni()
{
	QSettings settings(inifile(),QSettings::IniFormat);
	int radioButton_tool_point = 0;
	if (ui->radioButton_tool_line->isChecked()==true) radioButton_tool_point = 1;
	else if (ui->radioButton_tool_polygon->isChecked()==true) radioButton_tool_point = 2;
	else radioButton_tool_point = 0;
	settings.setValue("ui/radioButton_tool_point",radioButton_tool_point);

	settings.setValue("ui/lineEdit_currentID",ui->lineEdit_currentID->text());

	settings.setValue("ui/lineEdit_point_lat",ui->lineEdit_point_lat->text());
	settings.setValue("ui/lineEdit_point_lon",ui->lineEdit_point_lon->text());
	settings.setValue("ui/spinBox_point_width",ui->spinBox_point_width->value());
	settings.setValue("ui/spinBox_point_size_height",ui->spinBox_point_size_height->value());
	settings.setValue("ui/lineEdit_PointColorEdge",ui->lineEdit_PointColorEdge->text());
	settings.setValue("ui/lineEdit_PointColorFill",ui->lineEdit_PointColorFill->text());

	int radioButton_PointRect = 0;
	if (ui->radioButton_PointRound->isChecked()==true) radioButton_PointRect = 1;
	else radioButton_PointRect = 0;
	settings.setValue("ui/radioButton_PointRect",radioButton_PointRect);

	settings.setValue("ui/lineEdit_prop_name",ui->lineEdit_prop_name->text());
	settings.setValue("ui/lineEdit_prop_string",ui->lineEdit_prop_string->text());
	settings.setValue("ui/lineEdit_lineLat1",ui->lineEdit_lineLat1->text());
	settings.setValue("ui/lineEdit_lineLat2",ui->lineEdit_lineLat2->text());
	settings.setValue("ui/lineEdit_lineLon1",ui->lineEdit_lineLon1->text());
	settings.setValue("ui/lineEdit_lineLon2",ui->lineEdit_lineLon2->text());
	settings.setValue("ui/spinBox_lineWidth",ui->spinBox_lineWidth->value());
	settings.setValue("ui/lineEdit_lineColor",ui->lineEdit_lineColor->text());

	settings.setValue("ui/comboBox_linePad",ui->comboBox_linePad->currentIndex());

	settings.setValue("ui/lineEdit_RegionColorEdge",ui->lineEdit_RegionColorEdge->text());
	settings.setValue("ui/lineEdit_RegionColorFill",ui->lineEdit_RegionColorFill->text());
	settings.setValue("ui/plainTextEdit_corners",ui->plainTextEdit_corners->toPlainText());
}

void qtvplugin_geomarker::loadSettingsFromIni()
{
	QSettings settings(inifile(),QSettings::IniFormat);
	int radioButton_tool_point =  settings.value("ui/radioButton_tool_point",0).toInt();
	switch (radioButton_tool_point)
	{
	case 0:
		ui->radioButton_tool_point->setChecked(true);
		break;
	case 1:
		ui->radioButton_tool_line->setChecked(true);
		break;
	case 2:
		ui->radioButton_tool_polygon->setChecked(true);
		break;
	default:
		ui->radioButton_tool_point->setChecked(true);
		break;
	}
	QString lineEdit_currentID = settings.value("ui/lineEdit_currentID","PT0").toString();
	ui->lineEdit_currentID->setText(lineEdit_currentID);

	QString lineEdit_point_lat = settings.value("ui/lineEdit_point_lat","0").toString();
	ui->lineEdit_point_lat->setText(lineEdit_point_lat);
	QString lineEdit_point_lon = settings.value("ui/lineEdit_point_lon","0").toString();
	ui->lineEdit_point_lon->setText(lineEdit_point_lon);
	int spinBox_point_width = settings.value("ui/spinBox_point_width",8).toInt();
	ui->spinBox_point_width->setValue(spinBox_point_width);
	int spinBox_point_size_height = settings.value("ui/spinBox_point_size_height",8).toInt();
	ui->spinBox_point_size_height->setValue(spinBox_point_size_height);
	QString lineEdit_PointColorEdge = settings.value("ui/lineEdit_PointColorEdge",color2string(QColor(0,0,0,128))).toString();
	ui->lineEdit_PointColorEdge->setText(lineEdit_PointColorEdge);
	QString lineEdit_PointColorFill = settings.value("ui/lineEdit_PointColorFill",color2string(QColor(255,255,255,128))).toString();
	ui->lineEdit_PointColorFill->setText(lineEdit_PointColorFill);
	int radioButton_PointRect =  settings.value("ui/radioButton_PointRect",0).toInt();
	switch (radioButton_PointRect)
	{
	case 0:
		ui->radioButton_PointRect->setChecked(true);
		break;
	case 1:
		ui->radioButton_PointRound->setChecked(true);
		break;
	default:
		ui->radioButton_PointRect->setChecked(true);
		break;
	}

	QString lineEdit_prop_name = settings.value("ui/lineEdit_prop_name","LABEL").toString();
	ui->lineEdit_prop_name->setText(lineEdit_prop_name);
	QString lineEdit_prop_string = settings.value("ui/lineEdit_prop_string","please input label here").toString();
	ui->lineEdit_prop_string->setText(lineEdit_prop_string);
	QString lineEdit_lineLat1 = settings.value("ui/lineEdit_lineLat1","0").toString();
	ui->lineEdit_lineLat1->setText(lineEdit_lineLat1);
	QString lineEdit_lineLat2 = settings.value("ui/lineEdit_lineLat2","0").toString();
	ui->lineEdit_lineLat2->setText(lineEdit_lineLat2);
	QString lineEdit_lineLon1 = settings.value("ui/lineEdit_lineLon1","0").toString();
	ui->lineEdit_lineLon1->setText(lineEdit_lineLon1);
	QString lineEdit_lineLon2 = settings.value("ui/lineEdit_lineLon2","0").toString();
	ui->lineEdit_lineLon2->setText(lineEdit_lineLon2);
	int spinBox_lineWidth = settings.value("ui/spinBox_lineWidth",3).toInt();
	ui->spinBox_lineWidth->setValue(spinBox_lineWidth);
	QString lineEdit_lineColor = settings.value("ui/lineEdit_lineColor","0").toString();
	ui->lineEdit_lineColor->setText(lineEdit_lineColor);
	int comboBox_linePad = settings.value("ui/comboBox_linePad",1).toInt();
	ui->comboBox_linePad->setCurrentIndex(comboBox_linePad);

	QString lineEdit_RegionColorEdge = settings.value("ui/lineEdit_RegionColorEdge","0").toString();
	ui->lineEdit_RegionColorEdge->setText(lineEdit_RegionColorEdge);
	QString lineEdit_RegionColorFill = settings.value("ui/lineEdit_RegionColorFill","0").toString();
	ui->lineEdit_RegionColorFill->setText(lineEdit_RegionColorFill);
	QString plainTextEdit_corners = settings.value("ui/plainTextEdit_corners","0").toString();
	ui->plainTextEdit_corners->setPlainText(plainTextEdit_corners);

}
void qtvplugin_geomarker::on_pushButton_update_clicked()
{
	if (m_pVi==0 || !m_pScene)
		return;
	QString name = ui->lineEdit_currentID->text();
	SaveSettingsToIni();
	if (ui->radioButton_tool_point->isChecked())
	{
		double lat = ui->lineEdit_point_lat->text().toDouble();
		double lon = ui->lineEdit_point_lon->text().toDouble();
		int tp = ui->radioButton_PointRect->isChecked()?0:1;
		QColor colorEdge(string2color(ui->lineEdit_PointColorEdge->text()));
		QColor colorFill(string2color(ui->lineEdit_PointColorFill->text()));
		int width = ui->spinBox_point_width->value();
		int height = ui->spinBox_point_size_height->value();
		if (tp==0)
			update_point<QTVP_GEOMARKER::geoGraphicsRectItem>(name,lat,lon,colorEdge,colorFill,width,height);
		else
			update_point<QTVP_GEOMARKER::geoGraphicsEllipseItem>(name,lat,lon,colorEdge,colorFill,width,height);
	}
	else if (ui->radioButton_tool_line->isChecked())
	{
		double lat1 = ui->lineEdit_lineLat1->text().toDouble();
		double lat2 = ui->lineEdit_lineLat2->text().toDouble();
		double lon1 = ui->lineEdit_lineLon1->text().toDouble();
		double lon2 = ui->lineEdit_lineLon2->text().toDouble();
		int lineWidth = ui->spinBox_lineWidth->value();
		QColor lineColor = string2color( ui->lineEdit_lineColor->text());
		update_line(name,lat1,lon1,lat2,lon2,lineColor,lineWidth);
	}
	else if (ui->radioButton_tool_polygon->isChecked())
	{
		QColor colorEdge(string2color(ui->lineEdit_RegionColorEdge->text()));
		QColor colorFill(string2color(ui->lineEdit_RegionColorFill->text()));
		int width = ui->spinBox_lineWidth->value();
		QPolygonF latlons;
		QString strPlainTexts = ui->plainTextEdit_corners->toPlainText();
		strPlainTexts.remove(' ');
		strPlainTexts.remove('\n');
		strPlainTexts.remove('\r');
		strPlainTexts.remove('\015');
		strPlainTexts.remove('\012');
		QStringList lst = strPlainTexts.split(QRegExp("[,;]"),QString::SkipEmptyParts);
		//qDebug()<<lst;
		int c = 0;
		QPointF ll;
		foreach (QString s,lst)
		{
			if (c%2==0)
				ll.setY(s.toDouble());
			else
				ll.setX(s.toDouble());
			if ((++c) % 2==0)
				latlons.push_back(ll);
		}
		if (latlons.size())
			update_region(name,latlons,colorEdge,colorFill,width);

	}
	else
		return;
	refreshMarks();
}

void qtvplugin_geomarker::on_pushButton_del_clicked()
{
	QSet<int> rows;
	QModelIndexList lst =  ui->tableView_marks->selectionModel()->selectedIndexes();
	foreach (QModelIndex idd, lst)
		rows.insert(idd.row());
	foreach (int row, rows)
	{
		QString namep = m_pGeoItemModel->data(m_pGeoItemModel->index(row,0)).toString();
		QTVP_GEOMARKER::geoItemBase * b = m_pScene->geoitem_by_name(namep);
		if (b)
			m_pScene->removeItem(b,0);
	}
	refreshMarks();

}

void qtvplugin_geomarker::on_pushButton_prop_update_clicked()
{
	QString name = ui->lineEdit_currentID->text();
	//Fill in the pages
	QTVP_GEOMARKER::geoItemBase * item = m_pScene->geoitem_by_name(name);
	if (item)
	{
		item->set_prop_data(ui->lineEdit_prop_name->text(),ui->lineEdit_prop_string->text());
		this->refreshProps(item);
	}
}
void qtvplugin_geomarker::on_pushButton_prop_delete_clicked()
{
	QString name = ui->lineEdit_currentID->text();
	//Fill in the pages
	QTVP_GEOMARKER::geoItemBase * item = m_pScene->geoitem_by_name(name);
	if (item)
	{
		QSet<int> rows;
		QModelIndexList lst =  ui->tableView_props->selectionModel()->selectedIndexes();
		foreach (QModelIndex idd, lst)
			rows.insert(idd.row());
		foreach (int row, rows)
		{
			QString namep = m_pGeoPropModel->data(m_pGeoPropModel->index(row,0)).toString();
			item->del_prop(namep);
		}
	}
	this->refreshProps(item);
}


void qtvplugin_geomarker::on_tableView_marks_doubleClicked(const QModelIndex & index)
{
	int row = index.row();
	if (row >=0 && row < m_pGeoItemModel->rowCount() )
	{
		QString name = m_pGeoItemModel->data(
					m_pGeoItemModel->index(row,0)
					).toString();

		refreshItemUI(name);
	}
}

void qtvplugin_geomarker::on_pushButton_getRegion_clicked()
{
	if (!m_pVi)	return;
	QString strGridName = QString("grid%1").arg(m_nInstance);
	layer_interface * pif =  m_pVi->layer(strGridName);
	if (pif)
	{
		QMap<QString, QVariant> inPara, outPara;
		inPara["function"] = "get_region";
		outPara = pif->call_func(inPara);
		QString strPlainText = "";
		if (outPara.contains("size"))
		{
			int nsz = outPara["size"].toInt();
			for (int i=0;i<nsz;++i)
			{
				QString latkey = QString("lat%1").arg(i);
				QString lonkey = QString("lon%1").arg(i);
				strPlainText += QString("%1,%2;\n").arg(outPara[latkey].toDouble(),0,'f',14).arg(outPara[lonkey].toDouble(),0,'f',14);
			}
		}
		ui->plainTextEdit_corners->setPlainText(strPlainText);
	}
}
void qtvplugin_geomarker::refreshItemUI(QString markname)
{
	QString name = markname;
	//Fill in the pages
	QTVP_GEOMARKER::geoItemBase * item = m_pScene->geoitem_by_name(name);
	if (item)
	{
		ui->lineEdit_currentID->setText(item->item_name());
		switch (item->item_type())
		{
		case QTVP_GEOMARKER::ITEAMTYPE_RECT_POINT:
		{
			QTVP_GEOMARKER::geoGraphicsRectItem * pitem = dynamic_cast<QTVP_GEOMARKER::geoGraphicsRectItem *>(item);
			if (!pitem)
				break;
			ui->lineEdit_point_lat->setText(QString("%1").arg(pitem->lat(),0,'f',14));
			ui->lineEdit_point_lon->setText(QString("%1").arg(pitem->lon(),0,'f',14));
			ui->radioButton_PointRect->setChecked(true);
			QColor colorEdge = pitem->pen().color();
			ui->lineEdit_PointColorEdge->setText(color2string(colorEdge));
			QColor colorFill = pitem->brush().color();
			ui->lineEdit_PointColorFill->setText(color2string(colorFill));
			ui->spinBox_point_width->setValue(pitem->width());
			ui->spinBox_point_size_height->setValue(pitem->height());
		}
			break;
		case QTVP_GEOMARKER::ITEAMTYPE_ELLIPSE_POINT:
		{
			QTVP_GEOMARKER::geoGraphicsEllipseItem * pitem = dynamic_cast<QTVP_GEOMARKER::geoGraphicsEllipseItem *>(item);
			if (!pitem)
				break;
			ui->lineEdit_point_lat->setText(QString("%1").arg(pitem->lat(),0,'f',14));
			ui->lineEdit_point_lon->setText(QString("%1").arg(pitem->lon(),0,'f',14));
			ui->radioButton_PointRound->setChecked(true);
			QColor colorEdge = pitem->pen().color();
			ui->lineEdit_PointColorEdge->setText(color2string(colorEdge));
			QColor colorFill = pitem->brush().color();
			ui->lineEdit_PointColorFill->setText(color2string(colorFill));
			ui->spinBox_point_width->setValue(pitem->width());
			ui->spinBox_point_size_height->setValue(pitem->height());
		}
			break;
		case QTVP_GEOMARKER::ITEAMTYPE_LINE:
		{
			QTVP_GEOMARKER::geoGraphicsLineItem * pitem = dynamic_cast<QTVP_GEOMARKER::geoGraphicsLineItem *>(item);
			if (!pitem)
				break;
			ui->lineEdit_lineLat1->setText(QString("%1").arg(pitem->lat1(),0,'f',14));
			ui->lineEdit_lineLat2->setText(QString("%1").arg(pitem->lat2(),0,'f',14));
			ui->lineEdit_lineLon1->setText(QString("%1").arg(pitem->lon1(),0,'f',14));
			ui->lineEdit_lineLon2->setText(QString("%1").arg(pitem->lon2(),0,'f',14));
			QColor color = pitem->pen().color();
			ui->lineEdit_lineColor->setText(color2string(color));
			int width = pitem->pen().width();
			ui->spinBox_point_width->setValue(width);
			Qt::PenStyle st = pitem->pen().style();
			ui->comboBox_linePad->setCurrentIndex((int)st);
		}
			break;
		case QTVP_GEOMARKER::ITEAMTYPE_REGION:
		{
			QTVP_GEOMARKER::geoGraphicsPolygonItem * pitem = dynamic_cast<QTVP_GEOMARKER::geoGraphicsPolygonItem *>(item);
			if (!pitem)
				break;
			QPolygonF pol = pitem->llas();
			QString strPlainText;
			foreach (QPointF p, pol)
				strPlainText += QString("%1,%2\n").arg(p.y(),0,'f',14).arg(p.x(),0,'f',14);
			ui->plainTextEdit_corners->setPlainText(strPlainText);
			QColor color = pitem->pen().color();
			ui->lineEdit_RegionColorEdge->setText(color2string(color));
			int width = pitem->pen().width();
			ui->spinBox_point_width->setValue(width);
			Qt::PenStyle st = pitem->pen().style();
			ui->comboBox_linePad->setCurrentIndex((int)st);
			QColor colorFill = pitem->brush().color();
			ui->lineEdit_RegionColorFill->setText(color2string(colorFill));
		}
			break;
		default:
			break;

		}//end switch
		refreshProps(item);
	}//end if item

}

void qtvplugin_geomarker::refreshProps(QTVP_GEOMARKER::geoItemBase * itm)
{
	m_pGeoPropModel->removeRows(0,m_pGeoPropModel->rowCount());
	int ct = itm->prop_counts();
	QStringList lstNames = itm->prop_names();
	QVariantList lstValues = itm->prop_values();
	for (int i=0;i<ct;++i)
	{
		m_pGeoPropModel->appendRow(new QStandardItem(lstNames.first()));
		m_pGeoPropModel->setData(
					m_pGeoPropModel->index(i,1),
					lstValues.first());
		lstNames.pop_front();
		lstValues.pop_front();
	}
}
