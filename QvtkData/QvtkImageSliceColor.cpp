// me 
#include "QvtkImageSliceColor.h"
#include "QvtkImage.h"
// vtk
#include <vtkLookupTable.h>
#include <vtkImageActor.h>
#include <vtkImageProperty.h>
#include <vtkExtractVOI.h>
//#include <vtkImageMapper3D.h>
// qt
#include <QDebug>
namespace Q {
	namespace vtk {
		Q_VTK_DATA_CPP(ImageSliceColor);
		ImageSliceColor::ImageSliceColor()
		{
			this->getImageActor()->GetProperty()->SetLookupTable(this->lookupTable);
			this->getImageActor()->GetProperty()->SetUseLookupTableScalarRange(true);
			this->getImageActor()->GetProperty()->SetInterpolationTypeToLinear();
			this->defaultColorFile = createAttribute(K.DefaultColorFile, static_cast<int>(0), true);
			this->colorFile = createAttribute(K.ColorFile, "", true);
		}

		ImageSliceColor::~ImageSliceColor()
		{
		}

		void ImageSliceColor::printSelf() const
		{
		}
		void ImageSliceColor::readXML(const QDomElement & xml, QString directoryPath)
		{
			ImageSlice::readXML(xml, directoryPath);
			ImageSliceColor::readLabel(this, xml, K.ColorId, K.ColorName);
		}
		void ImageSliceColor::writeXML(QDomElement & xml, QString directoryPath) const
		{
			ImageSlice::writeXML(xml, directoryPath);
			ImageSliceColor::writeLabel(this, xml, K.ColorId, K.ColorName);
		}
		int ImageSliceColor::getDefaultColorFile() const
		{
			return Data::getAttribute(this->defaultColorFile).toInt();
		}
		QString ImageSliceColor::getColorFile() const
		{
			return this->Data::getAttribute(this->colorFile).toString();
		}
		void ImageSliceColor::setRenderDataSet(DataSet * data)
		{
			if (this->getRenderDataSet() == data) {
				return;
			}
			// nullptr to remove connection
			if (this->getRenderDataSet()) {
				this->extractVOI->SetInputConnection(nullptr);
				Image* image = qobject_cast<Image*>(this->getRenderDataSet());
				if (image) {
					disconnect(image, &Image::windowChanged,
						this, &ImageSlice::setWindow);
					disconnect(image, &Image::levelChanged,
						this, &ImageSlice::setLevel);
				}
			}
			PlanarProp::setRenderDataSet(data);
			if (this->getRenderDataSet())
			{
				Image* image = qobject_cast<Image*>(data);
				if (!image) {
					qCritical() << "data is not Image.";
				}
				else {
					// Always using linear interpolation, setting again is unnecessary. 
					//this->getImageActor()->GetProperty()->SetInterpolationTypeToLinear();
					this->extractVOI->SetInputConnection(data->getOutputPort());
					connect(image, &Image::windowChanged,
						this, &ImageSlice::setWindow);
					connect(image, &Image::levelChanged,
						this, &ImageSlice::setLevel);
					setWindow(image->getWindow());
					setLevel(image->getLevel());
				}
			}
		}
		void ImageSliceColor::setDefaultColorFile(int i)
		{
			this->setAttribute(this->defaultColorFile, i);
			if (i > -1 && i < DEFAULT_COLOR_FILES.size()) {
				this->setColorFile(DEFAULT_COLOR_FILES[i]);
			}
		}
		void ImageSliceColor::setColorFile(QString colorFile)
		{
			Data::setAttribute(this->colorFile, colorFile);
			if (!DEFAULT_COLOR_FILES.contains(colorFile)) {
				this->setDefaultColorFile(-1);
			}
			if (!this->readLabelFile(colorFile)) {
				qCritical() << "Cannot find color file: " << colorFile;
				return;
			}
			this->namedColorsToLookupTable();
			this->namedColosrToTransferFunction();
		}

		void ImageSliceColor::setDefaultColorFile(Data *self, QStandardItem *item) {

		}

		void ImageSliceColor::setColorFile(Data *self, QStandardItem *item) {

		}

	}
}
