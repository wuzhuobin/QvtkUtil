#ifndef __QVTK_IMAGE_LABEL2_H__
#define __QVTK_IMAGE_LABEL2_H__
#pragma once
// me
#include "QvtkImage.h"
#include "QvtkLabelHelper.h"
// vtk 
class vtkLookupTable;
class vtkImageMapToColors;
class vtkNamedColors;
namespace Q {
	namespace vtk {
		class QVTKDATA_EXPORT ImageLabel2 :
			public Image, 
			public LabelHelper
		{
			Q_OBJECT;
			Q_PROPERTY(
				int			DefaultColorFile
				READ		getDefaultColorFile
				WRITE		setDefaultColorFile	);
			Q_PROPERTY(
				QString		ColorFile
				READ		getColorFile
				WRITE		setColorFile);
			Q_VTK_DATA_H(
				ImageLabel2,
				Q_VTK_KEY(Label)
				Q_VTK_KEY(LabelId)
				Q_VTK_KEY(LabelRGBA)
				Q_VTK_KEY(DefaultColorFile)
				Q_VTK_KEY(ColorFile)
			);
			static const QString NAME_PREFIX;
		public:
			ImageLabel2();
			virtual ~ImageLabel2() override;
			virtual void printSelf() const override;
			virtual void readXML(const QDomElement& xml, QString directoryPath = QString()) override;
			virtual void writeXML(QDomElement& xml, QString directoryPath = QString()) const override;
			//virtual void addReference(Prop* prop) override;
			virtual double getResetWindow() const { return 255.0; }
			virtual double getResetLevel() const { return 127.5; }
			virtual int getDefaultColorFile() const;
			virtual QString getColorFile() const;
			virtual vtkAlgorithmOutput* getLabelOutputPort() const;
			virtual vtkImageData* getLabelImageData() const;
			virtual vtkImageMapToColors* getImageMapToColors() const { return this->imageMapToColors; }
			public Q_SLOTS:
			//virtual void resetWindowLevel() override;
			virtual void setColor(int id, const double *rgba) override { this->setColor(id, rgba); }
			virtual void setColor(int id, double r, double g, double b, double a) override { this->setColor(id, r, g, b, a); }
			virtual void setDefaultColorFile(int i);
			virtual void setColorFile(QString colorFile);
			virtual void initializeLabel(Image* image, int type = 3);
		protected:
			template<typename ScalarType>
			static void zeroImage(vtkImageData* image);
			virtual Data* newInstance() const override { return new ImageLabel2; }
			vtkImageMapToColors* imageMapToColors;
			virtual void readLabel(const QDomElement& xml);
			virtual void writeLabel(QDomElement& xml) const;
			static void setDefaultColorFile(Data *self, QStandardItem *item);
			static void setColorFile(Data *self, QStandardItem *item);
		private:
			QStandardItem *colorFile;
			QStandardItem *defaultColorFile;
		};
	}
}

#endif // !__QVTK_IMAGE_LABEL2_H__

