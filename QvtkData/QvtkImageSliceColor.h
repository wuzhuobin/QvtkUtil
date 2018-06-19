#ifndef __QVTK_IMAGE_SLICE_COLOR_H__
#define __QVTK_IMAGE_SLICE_COLOR_H__
#pragma once
// me 
#include "QvtkImageSlice.h"
#include "QvtkLabelHelper.h"
// vtk
class vtkLookupTable;
class vtkNamedColors;
namespace Q {
	namespace vtk {
		class QVTKDATA_EXPORT ImageSliceColor : 
			public ImageSlice, 
			public LabelHelper
		{
			Q_OBJECT;
			Q_VTK_DATA_H(
				ImageSliceColor,
				Q_VTK_KEY(ColorId)
				Q_VTK_KEY(ColorName)
				Q_VTK_KEY(DefaultColorFile)
				Q_VTK_KEY(ColorFile)
			);
			static const QString NAME_PREFIX;
		public:
			ImageSliceColor();
			virtual ~ImageSliceColor() override;
			virtual void printSelf() const override;
			virtual void readXML(const QDomElement &xml, QString directoryPath = QString()) override;
			virtual void writeXML(QDomElement &xml, QString directoryPath = QString()) const override;
			virtual int getDefaultColorFile() const;
			virtual QString getColorFile() const;
		public Q_SLOTS:
			virtual void setColor(int id, const double *rgba) override { this->setColor(id, rgba); }
			virtual void setColor(int id, double r, double g, double b, double a) override { this->setColor(id, r, g, b, a); }
			virtual void setRenderDataSet(DataSet *data = nullptr) override;
			virtual void setDefaultColorFile(int i);
			virtual void setColorFile(QString colorFile);
		protected:
			virtual Data *newInstance() const override { return new ImageSliceColor; }
			static void setDefaultColorFile(Data *self, QStandardItem *item);
			static void setColorFile(Data *self, QStandardItem *item);
		private:
			QStandardItem *colorFile;
			QStandardItem *defaultColorFile;
		};
	}
}

#endif // !__QVTK_IMAGE_SLICE_COLOR_H__
