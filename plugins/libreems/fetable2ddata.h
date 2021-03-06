/************************************************************************************
 * EMStudio - Open Source ECU tuning software                                       *
 * Copyright (C) 2013  Michael Carpenter (malcom2073@gmail.com)                     *
 *                                                                                  *
 * This file is a part of EMStudio                                                  *
 *                                                                                  *
 * EMStudio is free software; you can redistribute it and/or                        *
 * modify it under the terms of the GNU Lesser General Public                       *
 * License as published by the Free Software Foundation, version                    *
 * 2.1 of the License.                                                              *
 *                                                                                  *
 * EMStudio is distributed in the hope that it will be useful,                      *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                *
 * Lesser General Public License for more details.                                  *
 *                                                                                  *
 * You should have received a copy of the GNU Lesser General Public                 *
 * License along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   *
 ************************************************************************************/

#ifndef FETABLE2DDATA_H
#define FETABLE2DDATA_H
#include <QObject>
#include <QString>
#include <QList>
#include <QByteArray>
#include <QMutex>
#include "table2ddata.h"
#include "table2dmetadata.h"

class FETable2DData : public Table2DData
{
	Q_OBJECT
public:
	FETable2DData(bool is32bit);
	void setData(unsigned short locationid,bool isflashonly, QByteArray payload);
	void setMetaData(TableMeta metadata,FieldMeta xMeta,FieldMeta yMeta);
	QByteArray data();
	double maxActualXAxis();
	double maxActualYAxis();
	double minActualXAxis();
	double minActualYAxis();
	double maxCalcedXAxis();
	double maxCalcedYAxis();
	double minCalcedXAxis();
	double minCalcedYAxis();
	QString axisLabel() { return ""; }
	QString valueLabel() { return ""; }

	QList<double> axis();
	QList<double> values();
	int columns();
	int rows();
	void setCell(int row, int column,double newval);
	void setWritesEnabled(bool enabled);
	void writeWholeLocation(bool ram);
	bool isRam() { return !m_isFlashOnly; }
	double calcAxis(qint64 val,FieldMeta metadata);
	quint64 backConvertAxis(double val,FieldMeta metadata);
private:
	bool m_is32Bit;
	QMutex *m_acccessMutex;
	unsigned short m_dataSize;
	bool m_isSignedData;
	bool m_isFlashOnly;
	bool m_writesEnabled;
	double m_maxActualXAxis;
	double m_maxActualYAxis;
	double m_minActualXAxis;
	double m_minActualYAxis;
	double m_maxCalcedXAxis;
	double m_maxCalcedYAxis;
	double m_minCalcedXAxis;
	double m_minCalcedYAxis;

	//Table2DMetaData m_metaData;
	TableMeta m_tableMeta;
	FieldMeta x_metaData;
	FieldMeta y_metaData;
	unsigned short m_locationId;
	QList<double> m_axis;
	QList<double> m_values;
	QString m_axisLabel;
	QString m_valuesLabel;
	void reCalcAxisData();
signals:
	void saveSingleDataToFlash(unsigned short locationid,unsigned short offset, unsigned short size,QByteArray data);
	void saveSingleDataToRam(unsigned short locationid,unsigned short offset, unsigned short size,QByteArray data);
	void requestBlockFromRam(unsigned short locationid,unsigned short offset,unsigned short size);
	void requestBlockFromFlash(unsigned short locationid,unsigned short offset,unsigned short size);
	void update();
	void requestRamUpdateFromFlash(unsigned short locationid);
	void requestFlashUpdateFromRam(unsigned short locationid);
public slots:
	void updateFromFlash(); //Trigger a flash update, if table is in ram, it copies from flash to ram, and sends a request to the ECU
	void updateFromRam(); //Trigger a ram udpate. Requests table from the ECU from ram
	void saveRamToFlash();
};

#endif // FETABLE2DDATA_H
