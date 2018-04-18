#include "QvtkPolyData.h"

// qt
#include <QDebug>
#include <QDomDocument>
#include <QStandardItemModel>
#include <QDir>

//vtk
#include <vtkXMLPolyDataReader.h>
#include <vtkXMLPolyDataWriter.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>
#include <vtkQRCFileReader.h>
//#include <vtkOBJ>
#include <vtkOBJReader.h>
#include <vtkPolyData.h>
#include <vtkNew.h>
#include <vtkTransform.h>

Q_VTK_DATACPP(QvtkPolyData);
const QString QvtkPolyData::COLOR_RGB[3]{ "R", "G", "B" };

QvtkPolyData::QvtkPolyData()
{
	vtkPolyData* data = vtkPolyData::New();
	setDataSet(data);
	data->Delete();

	this->transformPolyDataFilter = vtkTransformPolyDataFilter::New();
	this->transformPolyDataFilter->SetInputData(this->getPolyData());

	QStandardItem* colorItem = createAttribute(K.Color);


	QStringList colorKeys;
	QVariantList colorValues;


	for (int i = 0; i < 3; i++)
	{
		colorKeys << (COLOR_RGB[i]);
		colorValues << static_cast<double>(0);
	}

	this->color = createAttributesByColumns(colorKeys, colorValues, true, colorItem);
	for (int i= 0; i< 3; i++)
	{
		insertSlotFunction(this->color[i], &QvtkPolyData::setColor);
	}

}

QvtkPolyData::~QvtkPolyData()
{
	this->transformPolyDataFilter->Delete();
}

void QvtkPolyData::readXML(const QDomElement & element, QString directoryPath)
{
	DataSet::readXML(element, directoryPath);
}

void QvtkPolyData::writeXML(QDomElement & element, QString directoryPath) const
{
	DataSet::writeXML(element, directoryPath);
}

bool QvtkPolyData::readData(QString rootDirectory)
{
	QString path;
	if (rootDirectory.isEmpty())
	{
		if(this->getAbsolutePath().isEmpty() ||
			this->getAbsolutePath().first().isEmpty())
		{
			qCritical() << "getAbsolutePath() is empty. ";
			return false;
		}
		path = this->getAbsolutePath().first();
	}
	else
	{
		
		if (this->getRelativePath().isEmpty() ||
			this->getRelativePath().first().isEmpty())
		{
			qCritical() << "getRelativePath() is empty. ";
			return false;
		}
		QDir _rootDir(rootDirectory);
		path = _rootDir.absoluteFilePath(this->getRelativePath().first());

	}
	if(!this->getDataSet())
	{
		qCritical() << "data is a null ptr. ";
		return false;
	}
	// When the path starts with ":", it is in the QRC
	if (path.startsWith(":")) 
	{
		return readQRC(path, this->getPolyData());
	}
	else {
		return readDataSuffix(path, this->getPolyData());
	}
}

bool QvtkPolyData::writeData(QString rootDirectory) const
{
	QString path;
	// rootDirectory is empty, write absolute path. 
	if (rootDirectory.isEmpty())
	{
		if(this->getAbsolutePath().isEmpty() ||
			this->getAbsolutePath().first().isEmpty())
		{
			qCritical() << "getAbsolutePath() is empty. ";
			return false;
		}
		path = this->getAbsolutePath().first();
	}
	// otherwise write relative path with root directory. 
	else
	{
		QDir _rootDir(rootDirectory);
		if(this->getRelativePath().isEmpty() ||
			this->getRelativePath().first().isEmpty())
		{
			qCritical() << "getRelativePath() is empty. ";
			return false;
		}
		path = _rootDir.absoluteFilePath(this->getRelativePath().first());

	}
	if(!this->getDataSet())
	{
		qCritical() << "data is null. ";
		return false;
	}


	// QRC Data no need to write with relative path. 
	if (path.startsWith(":")) {
		return true;
	}
	// write absolute path with transform.
	else if (rootDirectory.isEmpty()) {
		return writeDataSuffix(path, this->getTransformPolyData());
	}
	// wirte relative path without transform. 
	else{
		return writeDataSuffix(path, this->getPolyData());
	}
}

void QvtkPolyData::setColor(const double color[3])
{
	setAttributes(this->color, QVariantList() << color[0]
		<< color[1] << color[2]);
	emit colorChanged(color);
}

void QvtkPolyData::getColor(double color[3]) const
{
	QVariantList list = getAttributes(this->color);
	for (int i = 0; i < 3; ++i)
	{
		color[i] = list[i].toDouble();
	}
}

vtkAlgorithmOutput * QvtkPolyData::getTransformOutputPort() const
{
	vtkTransform* transform = vtkTransform::New();
	this->getTransform(transform);
	this->transformPolyDataFilter->SetTransform(transform);
	this->transformPolyDataFilter->Update();
	transform->Delete();
	return this->transformPolyDataFilter->GetOutputPort();
}

void QvtkPolyData::setColor(Data * self, QStandardItem * item)
{
	QvtkPolyData* _self = static_cast<QvtkPolyData*>(self);
	double color[3];
	_self->getColor(color);
	_self->setColor(color);
}

bool QvtkPolyData::readDataSuffix(QString fileName, vtkPolyData * data, int suffix)
{
	if (data == nullptr) {
		qCritical() << "data is nullptr"; 
		return false;
	}
	POLY_DATA_SUFFIX suffixType;
	if (suffix == -1) {
		suffixType = static_cast<POLY_DATA_SUFFIX>(suffixTranslate(QFileInfo(fileName).completeSuffix()));
	}
	else {
		suffixType = static_cast<POLY_DATA_SUFFIX>(suffix);
	}
	switch (suffixType)
	{
	case QvtkPolyData::UNKNOWN:
	default:
		qCritical() << "Cannot not read this file" << fileName;
		data = nullptr;
		return false;
	case QvtkPolyData::VTK:
		{
		vtkNew<vtkPolyDataReader> reader;
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		data->ShallowCopy(reader->GetOutput());
		}
		return true;
	case QvtkPolyData::VTP:
		{
		vtkNew<vtkXMLPolyDataReader> reader;
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		data->ShallowCopy(reader->GetOutput());
		}
		return true;
	case QvtkPolyData::STL:
		{
		vtkNew<vtkSTLReader> reader;
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		data->ShallowCopy(reader->GetOutput());
		}
		return true;
	case QvtkPolyData::OBJ:
		{
		vtkNew<vtkOBJReader> reader;
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		data->ShallowCopy(reader->GetOutput());
		}
		return true;
	}
}

bool QvtkPolyData::readQRC(QString fileName, vtkPolyData * data, int suffix)
{
	POLY_DATA_SUFFIX suffixType;
	if (suffix == -1) {
		suffixType = static_cast<POLY_DATA_SUFFIX>(suffixTranslate(QFileInfo(fileName).completeSuffix()));
	}
	else {
		suffixType = static_cast<POLY_DATA_SUFFIX>(suffix);
	}
	switch (suffixType)
	{
	case QvtkPolyData::UNKNOWN:
	default:
		qCritical() << "Cannot read this file" << fileName;
		data = nullptr;
		return false;
	case QvtkPolyData::VTK:
	{
		vtkNew<vtkQRCFileReader<vtkPolyDataReader>> reader;
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		data->ShallowCopy(reader->GetOutput());
	}
	return true;
	case QvtkPolyData::VTP:
	{
		vtkNew<vtkQRCFileReader<vtkXMLPolyDataReader>> reader;
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		data->ShallowCopy(reader->GetOutput());
	}
	return true;
	case QvtkPolyData::STL:
	{
		vtkNew<vtkQRCFileReader<vtkSTLReader>> reader;
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		data->ShallowCopy(reader->GetOutput());
	}
	return true;
	case QvtkPolyData::OBJ:
	{
		vtkNew<vtkQRCFileReader<vtkOBJReader>> reader;
		reader->SetFileName(fileName.toStdString().c_str());
		reader->Update();
		data->ShallowCopy(reader->GetOutput());
	}
	return true;
	}
}

bool QvtkPolyData::writeDataSuffix(QString fileName, vtkPolyData * data, int suffix)
{
	POLY_DATA_SUFFIX suffixType;
	if (suffix == -1) {
		suffixType = static_cast<POLY_DATA_SUFFIX>(suffixTranslate(QFileInfo(fileName).completeSuffix()));
	}
	else {
		suffixType = static_cast<POLY_DATA_SUFFIX>(suffix);
	}
	switch (suffixType)
	{
	case QvtkPolyData::UNKNOWN:
	case QvtkPolyData::OBJ:
	default:
		qCritical() << "Cannot write this file" << fileName;
		data = nullptr;
		return false;
	case QvtkPolyData::VTK:
	{
		vtkNew<vtkPolyDataWriter> writer;
		writer->SetInputData(data);
		writer->SetFileName(fileName.toStdString().c_str());
		writer->Write();
	}
	return true;
	case QvtkPolyData::VTP:
	{
		vtkNew<vtkXMLPolyDataWriter> writer;
		writer->SetInputData(data);
		writer->SetFileName(fileName.toStdString().c_str());
		writer->Write();
	}
	return true;
	case QvtkPolyData::STL:
	{
		vtkNew<vtkSTLWriter> writer;
		writer->SetInputData(data);
		writer->SetFileName(fileName.toStdString().c_str());
		writer->Write();
	}
	return true;
	}
}

unsigned int QvtkPolyData::suffixTranslate(QString suffix)
{
	if (suffix.contains("vtk")) {
		return VTK;
	}
	else if (suffix.contains("vtp")) {
		return VTP;
	}
	else if (suffix.contains("stl")) {
		return STL;
	}
	else if (suffix.contains("obj")) {
		return OBJ;
	}
	else {
		return UNKNOWN;
	}
}

vtkPolyData* QvtkPolyData::getPolyData() const
{
	return vtkPolyData::SafeDownCast(this->getDataSet());
}

vtkPolyData * QvtkPolyData::getTransformPolyData() const
{
	this->getTransformOutputPort();
	return this->transformPolyDataFilter->GetOutput();
}


void QvtkPolyData::printSelf() const
{
	DataSet::printSelf();

}

void QvtkPolyData::reset()
{
	DataSet::reset();
	this->setColor(0, 0, 0);

}
