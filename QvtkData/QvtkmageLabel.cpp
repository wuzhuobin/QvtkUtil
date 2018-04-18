// me
#include "QvtkImageLabel.h"
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
Q_VTK_DATACPP(QvtkImageLabel)
QvtkImageLabel::QvtkImageLabel()
{
	this->namedColors = vtkNamedColors::New();

	this->lookupTable = vtkLookupTable::New();


	this->rebuildLookupTable();

	this->imageMapToColors = vtkImageMapToColors::New();
	this->imageMapToColors->SetInputData(this->getImageData());
	this->imageMapToColors->SetLookupTable(this->getLookupTable());
}

QvtkImageLabel::~QvtkImageLabel()
{
	//this->lookupTable->Delete();
	this->imageMapToColors->Delete();
	this->lookupTable->Delete();
	this->namedColors->Delete();
}

void QvtkImageLabel::printSelf() const
{
	QvtkImage::printSelf();
}

void QvtkImageLabel::readXML(const QDomElement & xml, QString directoryPath)
{
	QvtkImage::readXML(xml, directoryPath);
	this->readLabel(xml);
}

void QvtkImageLabel::writeXML(QDomElement & xml, QString directoryPath) const
{
	QvtkImage::writeXML(xml, directoryPath);
	this->writeLabel(xml);
}

void QvtkImageLabel::addReference(QvtkAbstractProp * prop)
{
	DataSet::addReference(prop);
	QvtkImageSlice* slice = qobject_cast<QvtkImageSlice*>(prop);
	if (slice) {
		slice->getImageActor()->GetProperty()->SetInterpolationTypeToNearest();
	}
}

vtkAlgorithmOutput * QvtkImageLabel::getOutputPort() const
{
	return this->imageMapToColors->GetOutputPort();
}

vtkImageData * QvtkImageLabel::getLabelImageData() const
{
	return this->imageMapToColors->GetOutput();
}

int QvtkImageLabel::labelNameToLabelId(QString labelName) const
{
	if (labelName == "NULL") {
		return 0;
	}
	QStringList stringArray(QString::fromStdString(this->getNamedColors()->GetColorNames()).split('\n'));
	return stringArray.indexOf(labelName) + 1;
}

const double * QvtkImageLabel::getColor(int id) const 
{
	return this->getLookupTable()->GetTableValue(id);
}

void QvtkImageLabel::getColor(int id, double rgba[4]) const 
{
	this->getLookupTable()->GetTableValue(id, rgba);
}

const double * QvtkImageLabel::getColor(QString labelName) const 
{
	int id = this->labelNameToLabelId(labelName);
	if (id < 0) {
		qCritical() << "Label Name: " << labelName << "does not exist. ";
		return nullptr;
	}
	return this->getLookupTable()->GetTableValue(id);
}

void QvtkImageLabel::getColor(QString labelName, double rgba[4]) const 
{
	int id = this->labelNameToLabelId(labelName);
	if (id < 0) {
		qCritical() << "Label Name: " << labelName << "does not exist. ";
		return;
	}
	this->getColor(id, rgba);
}


void QvtkImageLabel::setColor(int id, double r, double g, double b, double a)
{
	this->getLookupTable()->SetTableValue(id, r, g, b, a);
}

void QvtkImageLabel::setColor(QString labelName, double r, double g, double b, double a)
{
	int id = this->labelNameToLabelId(labelName);
	if (id < 0) {
		qCritical() << "Label Name: " << labelName << "does not exist. ";
		return;
	}
	this->setColor(id, r, g, b, a);
}

void QvtkImageLabel::rebuildLookupTable() const
{
	this->getLookupTable()->SetTableValue(0, 0, 0, 0, 0);
	vtkSmartPointer<vtkStringArray> stringArray =
		vtkSmartPointer<vtkStringArray>::New();
	this->getNamedColors()->GetColorNames(stringArray);
	this->getLookupTable()->SetNumberOfColors(stringArray->GetNumberOfValues() + 1);
	this->getLookupTable()->SetTableRange(0, stringArray->GetNumberOfValues());
	for (vtkIdType id = 0; id < stringArray->GetNumberOfValues(); ++id)
	{
		double* rgba = this->namedColors->GetColor4d(stringArray->GetValue(id)).GetData();
		this->getLookupTable()->SetTableValue(id + 1, rgba);
		//cout << "id: " << id + 1 << '\t';
		//cout << "Name: " << stringArray->GetValue(id) << '\t';
		//cout << "RGBA: (" << rgba[0] << ", " << rgba[1] << ", " << rgba[2] << ", " << rgba[3] << ")\n";
	}
	this->getLookupTable()->Build();
}

void QvtkImageLabel::readLabel(const QDomElement & xml)
{
	QDomElement labelElem = xml.firstChildElement(K.Label);
	QDomElement labelNameElem = labelElem.firstChildElement(/*K.LabelName*/);
	while (!labelNameElem.isNull())
	{
		QString labelName = labelNameElem.tagName();
		int id = labelNameElem.attribute(K.LabelId).toInt();
		QString labelRGBA = labelNameElem.attribute(K.LabelRGBA);
		vtkColor4ub rgba_vtkColor4ub;
		rgba_vtkColor4ub = this->getNamedColors()->HTMLColorToRGBA(labelRGBA.toStdString());
		this->getLookupTable()->SetTableValue(
			id,
			rgba_vtkColor4ub.GetRed() / 255.0,
			rgba_vtkColor4ub.GetGreen() / 255.0,
			rgba_vtkColor4ub.GetBlue() / 255.0,
			rgba_vtkColor4ub.GetAlpha() / 255.0);

		labelNameElem = labelNameElem.nextSiblingElement(/*K.LabelName*/);
	}
	
	//this->rebuildLookupTable();
}

void QvtkImageLabel::writeLabel(QDomElement & xml) const
{
	vtkSmartPointer<vtkStringArray> stringArray =
		vtkSmartPointer<vtkStringArray>::New();
	this->getNamedColors()->GetColorNames(stringArray);
	QDomDocument dom = xml.ownerDocument();
	QDomElement labelElem = dom.createElement(K.Label);
	xml.appendChild(labelElem);

	for (vtkIdType id = 0; id < this->getLookupTable()->GetNumberOfTableValues(); ++id)
	{
		QString labelName;
		QString labelRGBA;
		unsigned char uchar_rgba[4];
		double double_rgba[4];
		vtkColor4ub rgba_vtkColor4ub;
		if (id == 0) {
			labelName = "NULL";
		}
		else {
			labelName = QString(stringArray->GetValue(id - 1));
		}
		// format things.
		this->getLookupTable()->GetTableValue(id, double_rgba);
		this->getLookupTable()->GetColorAsUnsignedChars(double_rgba, uchar_rgba);
		rgba_vtkColor4ub = vtkColor4ub(uchar_rgba[0], uchar_rgba[1], uchar_rgba[2], uchar_rgba[3]);
		labelRGBA = QString(this->getNamedColors()->RGBAToHTMLColor(rgba_vtkColor4ub));

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

void QvtkImageLabel::initializeLabel(QvtkImage* image, int type)
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
		vtkTemplateMacro(QvtkImageLabel::zeroImage<VTK_TT>(this->getImageData()));
	}
}

template<typename ScalarType>
void QvtkImageLabel::zeroImage(vtkImageData* image)
{
	for (vtkImageIterator<ScalarType> it(image, image->GetExtent());
		!it.IsAtEnd(); it.NextSpan()) {

		for (ScalarType* valIt = it.BeginSpan(); valIt != it.EndSpan(); ++valIt) {
			*valIt = 0;
		}
	}
}
