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
#include <vtkPassThroughFilter.h>
#include <vtkMatrix4x4.h>

// qt
#include <QDomElement>
#include <QMap>
#include <QDebug>

namespace Q {
namespace vtk{
Q_VTK_DATA_CPP(ImageLabel2)
ImageLabel2::ImageLabel2()
{
	this->lookupTable = vtkLookupTable::New();
	this->outputfilter = vtkSmartPointer<vtkPassThroughFilter>::New();
	//this->outputfilter->SetReferenceCount(this->outputfilter->GetReferenceCount() + 1);
	this->outputfilter->SetInputData(this->getImageData());

	// Initialize this
	this->nameToValueMap["None"] = 0;
	double* d = new double[4];
	d[0] = 0.;
	d[1] = 0.;
	d[2] = 0.;
	d[3] = 0.;
	this->valueToColorMap[0] = d;
}

ImageLabel2::~ImageLabel2()
{
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

void ImageLabel2::addReference(Prop * prop)
{
	DataSet::addReference(prop);
	ImageSlice* slice = qobject_cast<ImageSlice*>(prop);
	if (slice) {
		slice->getImageActor()->GetProperty()->SetInterpolationTypeToNearest();
		slice->getImageActor()->GetProperty()->SetLookupTable(this->lookupTable);
		slice->getImageActor()->GetProperty()->SetUseLookupTableScalarRange(true);
	}
}

vtkAlgorithmOutput * ImageLabel2::getOutputPort() const
{
	return this->outputfilter->GetOutputPort();
}


int ImageLabel2::getLabelIdFromName(QString labelName) const
{
	return this->nameToValueMap[labelName];
}

const double * ImageLabel2::getColor(int id) const 
{
	return this->getLookupTable()->GetTableValue(id);
}

void ImageLabel2::getColor(int id, double rgba[4]) const 
{
	this->getLookupTable()->GetTableValue(id, rgba);
}

const double * ImageLabel2::getColor(QString labelName) const 
{
	int id = this->getLabelIdFromName(labelName);
	if (id < 0) {
		qCritical() << "Label Name: " << labelName << "does not exist. ";
		return nullptr;
	}
	return this->getLookupTable()->GetTableValue(id);
}

void ImageLabel2::getColor(QString labelName, double rgba[4]) const 
{
	int id = this->getLabelIdFromName(labelName);
	if (id < 0) {
		qCritical() << "Label Name: " << labelName << "does not exist. ";
		return;
	}
	this->getColor(id, rgba);
}


const QMap<QString, int> ImageLabel2::getNameToValueMap()
{
	return this->nameToValueMap;
}

QString ImageLabel2::getColorName(int val) const
{
	for (QMap<QString, int>::const_iterator iter = this->nameToValueMap.cbegin();
		iter != this->nameToValueMap.cend();iter++)
	{
		if (iter.value() == val)
		{
			return iter.key();
		}
	}

	qWarning() << "Cannot find a label name for this value!";
	return QString("");
}

/*=================================================================================================*/
/*=================================================================================================*/
/* Depict these */
void ImageLabel2::setColor(int id, double r, double g, double b, double a)
{
	this->setColor(this->getColorName(id), id, r, g, b, a);
}

void ImageLabel2::setColor(QString labelName, double r, double g, double b, double a)
{
	if (this->getLabelIdFromName(labelName) >= 0)
	{
		this->setColor(labelName, this->getLabelIdFromName(labelName), r, g, b, a);
	}
}
/*=================================================================================================*/
/*=================================================================================================*/


void ImageLabel2::setColor(QString labelName, int value, double r, double g, double b, double a)
{
	/* Ensure unique key */
	if (this->nameToValueMap.keys().contains(labelName) && value != this->nameToValueMap[labelName])
	{
		qCritical() << labelName << " already exist in the list with different value!";
		return;
	}

	double *d = new double[4];
	d[0] = r;
	d[1] = g;
	d[2] = b;
	d[3] = a;
	this->nameToValueMap[labelName] = value;
	this->valueToColorMap[value] = d;
	this->rebuildLookupTable();
}


void ImageLabel2::setColor(QString labelName, int value, double d[4])
{
	this->setColor(labelName, value, d[0], d[1], d[2], d[3]);
}

void ImageLabel2::rebuildLookupTable()
{
	/* Alwasy has zero label */
	if (!this->nameToValueMap.keys().contains("None") || this->nameToValueMap["None"] != 0)
	{
		qWarning() << "Do not manually set label 'None' to values other than 0";
		this->nameToValueMap["None"] = 0; // force this
	}

	/* Obtain max value in list */
	QList<int> templist = this->nameToValueMap.values();
	int min=255, max=-255;
	for (QList<int>::const_iterator iter = templist.cbegin();
		iter != templist.cend();iter++)
	{
		if (*iter < min)
			min = *iter;
		if (*iter > max)
			max = *iter;
	}

	this->getLookupTable()->SetTableRange(min, max);
	this->getLookupTable()->SetNumberOfColors(max - min + 1);
	this->getLookupTable()->Build();
	for (QMap<QString, int>::const_iterator iter = this->nameToValueMap.cbegin();
		iter != this->nameToValueMap.cend();iter++)
	{
		this->getLookupTable()->SetTableValue(iter.value(), this->valueToColorMap[iter.value()]);
	}
}

void ImageLabel2::readLabel(const QDomElement & xml)
{
	QDomElement labelElem = xml.firstChildElement(K.Label);
	QDomElement labelNameElem = labelElem.firstChildElement(/*K.LabelName*/);
	while (!labelNameElem.isNull())
	{
		QString labelName = labelNameElem.tagName();
		int id = labelNameElem.attribute(K.LabelId).toInt();
		QString labelRGBA = labelNameElem.attribute(K.LabelRGBA);
		this->getLookupTable()->SetTableValue(
			id,
			labelRGBA.split(',')[0].toDouble(),
			labelRGBA.split(',')[1].toDouble(),
			labelRGBA.split(',')[2].toDouble(),
			labelRGBA.split(',')[3].toDouble()
			);

		labelNameElem = labelNameElem.nextSiblingElement(/*K.LabelName*/);
	}
	
	//this->rebuildLookupTable();
}

void ImageLabel2::writeLabel(QDomElement & xml) const
{
	QDomDocument dom = xml.ownerDocument();
	QDomElement labelElem = dom.createElement(K.Label);
	xml.appendChild(labelElem);

	for (vtkIdType id = 0; id < this->getLookupTable()->GetNumberOfTableValues(); ++id)
	{
		QString labelName;
		QString labelRGBA;
		double double_rgba[4];
		vtkColor4ub rgba_vtkColor4ub;
		if (id == 0) {
			labelName = "NULL";
		}
		else {
			labelName = this->getColorName(id);
		}
		// format things.
		this->getLookupTable()->GetTableValue(id, double_rgba);
		labelRGBA = QString("").sprintf("%.05f,%.05f,%.05f,%.05f", 
			double_rgba[0], double_rgba[1], double_rgba[2], double_rgba[3]);

		QDomElement labelNameElem = dom.createElement(labelName);
		labelElem.appendChild(labelNameElem);
		
		labelNameElem.setAttribute(K.LabelId, id);
		labelNameElem.setAttribute(K.LabelRGBA, labelRGBA);
		//label.setAttribute(K.LabelRGBA, )
		//cout << "id: " << id + 1 << '\t';;
		//cout << "Name: " << stringArray->GetValue(id) << '\t';
		//cout << "RGBA: (" << rgba[0] << ", " << rgba[1] << ", " << rgba[2] << ", " << rgba[3] << ")\n";
	}
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