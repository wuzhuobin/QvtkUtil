// me
#include "QvtkImageLabel2.h"
#include "QvtkImageSlice.h"

// vtk
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkImageProperty.h>
#include <vtkNamedColors.h>
#include <vtkStringArray.h>
#include <vtkImageIterator.h>
#include <vtkImageCast.h>
#include <vtkMatrix4x4.h>
// qt
#include <QDomElement>
#include <QDebug>
#include <QStandardItem>
namespace Q {
	namespace vtk {
		Q_VTK_DATA_CPP(ImageLabel2);
		const QString ImageLabel2::NAME_PREFIX("Label_");
		ImageLabel2::ImageLabel2()
		{
			this->imageMapToColors = vtkImageMapToColors::New();
			this->imageMapToColors->SetInputData(this->getImageData());
			this->imageMapToColors->SetLookupTable(this->lookupTable);
			this->defaultColorFile = createAttribute(K.DefaultColorFile, static_cast<int>(0), true);
			this->insertSlotFunction(this->defaultColorFile, &ImageLabel2::setDefaultColorFile);
			this->colorFile = createAttribute(K.ColorFile, "", true);
			this->insertSlotFunction(this->colorFile, &ImageLabel2::setColorFile);
			this->setDefaultColorFile(0);
		}

		ImageLabel2::~ImageLabel2()
		{
			this->imageMapToColors->Delete();
		}

		void ImageLabel2::printSelf() const
		{
			Image::printSelf();
		}

		void ImageLabel2::readXML(const QDomElement & xml, QString directoryPath)
		{
			Image::readXML(xml, directoryPath);
			this->readLabel(xml);
		}

		void ImageLabel2::writeXML(QDomElement & xml, QString directoryPath) const
		{
			Image::writeXML(xml, directoryPath);
			this->writeLabel(xml);
		}

		//void ImageLabel2::addReference(Prop * prop)
		//{
		//	DataSet::addReference(prop);
		//	ImageSlice* slice = qobject_cast<ImageSlice*>(prop);
		//	if (slice) {
		//		slice->getImageActor()->GetProperty()->SetInterpolationTypeToNearest();
		//	}
		//}

		int ImageLabel2::getDefaultColorFile() const
		{
			return this->getAttribute(this->defaultColorFile).toInt();
		}

		QString ImageLabel2::getColorFile() const
		{
			return  Data::getAttribute(this->colorFile).toString();
		}

		vtkAlgorithmOutput * ImageLabel2::getOutputPort() const
		{
			return this->imageMapToColors->GetOutputPort();
		}

		vtkImageData * ImageLabel2::getLabelImageData() const
		{
			return this->imageMapToColors->GetOutput();
		}

		void ImageLabel2::readLabel(const QDomElement & xml)
		{
			QDomElement labelElem = xml.firstChildElement(K.Label);
			QDomElement labelNameElem = labelElem.firstChildElement(/*K.LabelName*/);
			this->labelIdToLabelName.clear();
			while (!labelNameElem.isNull())
			{
				QString labelName = labelNameElem.tagName().remove(NAME_PREFIX);
				int id = labelNameElem.attribute(K.LabelId).toInt();
				QString htmlColor = labelNameElem.attribute(K.LabelRGBA);
				this->labelIdToLabelName.insert(id, labelName);
				this->namedColors->SetColor(labelName.toStdString(),
					this->namedColors->HTMLColorToRGBA(htmlColor.toStdString()));
				QString labelRGBA = labelNameElem.attribute(K.LabelRGBA);
				labelNameElem = labelNameElem.nextSiblingElement();
			}
			this->namedColorsToLookupTable();
			this->namedColosrToTransferFunction();
		}

		void ImageLabel2::writeLabel(QDomElement & xml) const
		{
			QDomDocument dom = xml.ownerDocument();
			QDomElement labelElem = dom.createElement(K.Label);
			xml.appendChild(labelElem);
			for (LabelIdToLabelName::const_iterator cit = this->labelIdToLabelName.cbegin();
				cit != this->labelIdToLabelName.cend(); ++cit) {
				vtkColor4ub rgba = this->namedColors->GetColor4ub(cit.value().toStdString());
				QDomElement labelNameElem = dom.createElement(NAME_PREFIX + cit.value());
				labelNameElem.setAttribute(K.LabelId, cit.key());
				labelNameElem.setAttribute(K.LabelRGBA, QString::fromStdString(this->namedColors->RGBAToHTMLColor(rgba)));
				labelElem.appendChild(labelNameElem);
			}
		}

		void ImageLabel2::setColorFile(Data * self, QStandardItem * item)
		{
			ImageLabel2 *_self = static_cast<ImageLabel2*>(self);
			_self->setColorFile(getAttribute(item).toString());
		}

		void ImageLabel2::setDefaultColorFile(Data * self, QStandardItem * item)
		{
			ImageLabel2 *_self = static_cast<ImageLabel2*>(self);
			_self->setDefaultColorFile(getAttribute(item).toInt());
		}

		void ImageLabel2::setDefaultColorFile(int i)
		{
			this->setAttribute(this->defaultColorFile, i);
			if (i > -1 && i < DEFAULT_COLOR_FILES.size()) {
				this->setColorFile(DEFAULT_COLOR_FILES[i]);
			}
		}

		void ImageLabel2::setColorFile(QString colorFile)
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

		void ImageLabel2::initializeLabel(Image* image, int type)
		{
			vtkSmartPointer<vtkImageCast> imageCast = vtkSmartPointer<vtkImageCast>::New();
			imageCast->SetInputConnection(image->getOutputPort());
			imageCast->SetOutputScalarType(type);
			imageCast->Update();

			this->getImageData()->ShallowCopy(imageCast->GetOutput());
			this->setOrigin(image->getOrigin());
			this->setPosition(image->getPosition());
			this->setOrientation(image->getOrientation());
			this->setScale(image->getScale());
			this->getUserMatrix()->DeepCopy(image->getUserMatrix());
			switch (type)
			{
				vtkTemplateMacro(ImageLabel2::zeroImage<VTK_TT>(this->getImageData()));
			}
		}

		template<typename ScalarType>
		void ImageLabel2::zeroImage(vtkImageData* image)
		{
			for (vtkImageIterator<ScalarType> it(image, image->GetExtent());
				!it.IsAtEnd(); it.NextSpan()) {

				for (ScalarType* valIt = it.BeginSpan(); valIt != it.EndSpan(); ++valIt) {
					*valIt = 0;
				}
			}
		}
	}
}