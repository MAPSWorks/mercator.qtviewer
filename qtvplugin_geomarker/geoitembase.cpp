#include "geoitembase.h"
#include "geographicsscene.h"
#include "../qtviewer_planetosm/osmtiles/viewer_interface.h"
#include <QTextEdit>
#include <QCoreApplication>
namespace QTVP_GEOMARKER{
	geoItemBase::geoItemBase(QString name,geo_item_type tp,QTVOSM::viewer_interface * pVi)
		:m_pVi(pVi)
		,m_nCurrentLevel(0)
		,m_type(tp)
		,m_name(name)
		,m_pLabelItem(0)
		,m_pSelectionBound(0)
		,m_bPropVisible(false)
	{
		if (pVi)
			m_nCurrentLevel = pVi->level();
	}

	geoItemBase::~geoItemBase()
	{
		if (m_pLabelItem)
		{
			if (m_pLabelItem->scene()==0)
			{
				delete m_pLabelItem;
				m_pLabelItem = 0;
			}
		}
		if (m_pSelectionBound)
		{
			if (m_pSelectionBound->scene()==0)
			{
				delete m_pSelectionBound;
				m_pSelectionBound = 0;
			}
		}
	}

	bool geoItemBase::is_selected	()
	{
		return m_bSeleted;
	}

	void geoItemBase::set_selected	(bool bsel)
	{
		m_bSeleted = bsel;
		QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
		if (pc)
		{
			if (m_bSeleted == true)
			{
				if (!m_pSelectionBound)
				{
					m_pSelectionBound = new QGraphicsRectItem(pc);
					m_pSelectionBound->setRect(pc->boundingRect());
					m_pSelectionBound->setPen(QPen(QBrush(QColor(255,64,0)),3,Qt::DashDotDotLine));
				}

			}
			else
			{
				if (m_pSelectionBound)
				{
					QGraphicsScene * sc =  pc->scene();
					if (sc)
					{
						sc->removeItem(m_pSelectionBound);
					}
					delete m_pSelectionBound;
					m_pSelectionBound = 0;
				}
			}
		}

	}

	void geoItemBase::setLevel(int newLevel)
	{
		if (m_pLabelItem && newLevel != level())
			m_pLabelItem->setPos(label_pos());
		if (m_pSelectionBound && newLevel != level())
		{
			QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
			if (pc)
				m_pSelectionBound->setRect(pc->boundingRect());
		}
		m_nCurrentLevel = newLevel;
	}

	void geoItemBase::setViewInterface(QTVOSM::viewer_interface * pVi)
	{
		m_pVi = pVi;
		m_nCurrentLevel = pVi->level();
	}

	QStringList geoItemBase::prop_names()
	{
		return /*std::move*/(m_props.keys());
	}
	int geoItemBase::prop_counts()
	{
		return m_props.size();
	}
	QVariantList geoItemBase::prop_values()
	{
		return /*std::move*/(m_props.values());
	}

	const QVariant & geoItemBase::prop_data(const QString & k)
	{
		static QVariant invalid_item;
		auto it = m_props.find(k);
		if (it==m_props.end())
			return invalid_item;
		return it.value();
	}

	void geoItemBase::set_prop_data(const QString &k,const  QVariant & v)
	{
		QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
		m_props[k] = v;
		if (k=="LABEL" && pc)
		{
			if (!m_pLabelItem)
			{
				m_pLabelItem = new QGraphicsTextItem(v.toString(),pc);
				m_pLabelItem->setDefaultTextColor(m_LabelTextColor);
				m_pLabelItem->setFont(m_LabelTextFont);
				if (pc->scale()>0)
				{
					qreal sc = pc->scale();
					if (sc)
						m_pLabelItem->setScale(1.0/sc);
				}
				m_pLabelItem->setRotation(-pc->rotation());
			}
			else if (m_bPropVisible==false)
				m_pLabelItem->setPlainText(v.toString());
			m_pLabelItem->setPos(this->label_pos());
		}
	}
	void geoItemBase::del_prop (const QString & k)
	{
		QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
		m_props.remove(k);
		if (k=="LABEL" && pc)
		{
			if (m_pLabelItem && m_bPropVisible==false)
			{
				if (pc->scene())
					pc->scene()->removeItem(m_pLabelItem);
				m_pLabelItem->deleteLater();
				m_pLabelItem = 0;
			}
		}

	}
	void geoItemBase::show_props(bool bShow)
	{
		QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
		if (!pc)
			return;
		if (bShow)
		{
			m_bPropVisible = true;
			QString str;
			QStringList propNames = prop_names();
			QVariantList propValues = prop_values();
			int cs = propNames.size();
			for (int i=0;i<cs ;++i)
			{
				str += QString("%1 : %2\n").arg(propNames.first()).arg(propValues.first().toString());
				propNames.pop_front();
				propValues.pop_front();
			}
			if (!m_pLabelItem)
			{
				m_pLabelItem = new QGraphicsTextItem(str,pc);
				m_pLabelItem->setDefaultTextColor(m_LabelTextColor);
				m_pLabelItem->setFont(m_LabelTextFont);
				if (pc->scale()>0)
				{
					qreal sc = pc->scale();
					if (sc)
						m_pLabelItem->setScale(1.0/sc);
				}
				m_pLabelItem->setRotation(-pc->rotation());

			}
			else
				m_pLabelItem->setPlainText(str);
			m_pLabelItem->setPos(this->label_pos());

		}
		else
		{
			m_bPropVisible = false;
			QVariant v = prop_data("LABEL");
			if (v.isValid()==false)
			{
				if (m_pLabelItem)
				{
					if (pc->scene())
						pc->scene()->removeItem(m_pLabelItem);
					m_pLabelItem->deleteLater();
					m_pLabelItem = 0;
				}
			}
			else
			{
				if (!m_pLabelItem)
				{
					m_pLabelItem = new QGraphicsTextItem(v.toString(),pc);
					m_pLabelItem->setDefaultTextColor(m_LabelTextColor);
					m_pLabelItem->setFont(m_LabelTextFont);
					if (pc->scale()>0)
					{
						qreal sc = pc->scale();
						if (sc)
							m_pLabelItem->setScale(1.0/sc);
					}
					m_pLabelItem->setRotation(-pc->rotation());

				}
				m_pLabelItem->setPlainText(v.toString());
				m_pLabelItem->setPos(this->label_pos());
			}
		}
	}
	bool geoItemBase::props_visible	()
	{
		return m_bPropVisible;
	}
	void geoItemBase::adjustLabelPos()
	{
		if (m_pLabelItem)
		{
			QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
			if (pc)
			{
				if (pc->scale()>0)
				{
					qreal sc = pc->scale();
					if (sc)
						m_pLabelItem->setScale(1.0/sc);
				}
				m_pLabelItem->setRotation(-pc->rotation());
			}
			m_pLabelItem->setPos(this->label_pos());
		}
		if (m_pSelectionBound)
		{
			QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
			if (pc)
				m_pSelectionBound->setRect(pc->boundingRect());
		}
	}

	bool geoItemBase::wantMouseHoverEvent()
	{
		QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
		if (pc)
		{
			return pc->acceptHoverEvents();
		}
		return false;
	}

	void geoItemBase::setWantMouseHoverEvent(bool w)
	{
		QGraphicsItem * pc = dynamic_cast<QGraphicsItem *> (this);
		if (pc)
		{
			pc->setAcceptHoverEvents(w);
		}

	}

}
