// me
#include "QvtkImage.h"
#include "QvtkImageSlice.h"
//vtk
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkImageReslice.h>
#include <vtkNew.h>
#include <vtkImageReader.h>
#include <vtkImageWriter.h>
#include <vtkXMLImageDataWriter.h>
#include <vtkXMLImageDataReader.h>
#include <vtkTransform.h>
#include <vtkMath.h>
//#include <vtkImageActor.h>
//#include <vtkImageProperty.h>
#include <vtkImageChangeInformation.h>
//itk
//#include <itkImage.h>
#include <itkAffineTransform.h>
//qt
#include <QDebug>
#include <QDir>
#include <QFileInfo>
namespace Q {
namespace vtk{
Q_VTK_DATA_CPP(Image)
Image::Image()
{
	vtkImageData* data = vtkImageData::New();
	setDataSet(data);
	data->Delete();

	this->window = createAttribute(K.Window, static_cast<double>(0), true);
	insertSlotFunction(this->window, &Image::setWindow);

	this->level = createAttribute(K.Level, static_cast<double>(0), true);
	insertSlotFunction(this->level, &Image::setLevel);


	this->imageChangeInformation = vtkImageChangeInformation::New();
	this->imageChangeInformation->SetInputConnection(this->getOutputPort());

	this->imageReslice = vtkImageReslice::New();
	this->imageReslice->SetInputConnection(this->imageChangeInformation->GetOutputPort());
	this->imageReslice->AutoCropOutputOn();
	this->imageReslice->TransformInputSamplingOn();
}

Image::~Image()
{
	this->imageReslice->Delete();
}

void Image::printSelf() const
{
	DataSet::printSelf();
}

bool Image::readData(QString rootDirectory)
{
	QStringList paths;
	if (rootDirectory.isEmpty())
	{
		if (this->getAbsolutePath().isEmpty()
				|| this->getAbsolutePath().first().isEmpty())
		{
			qCritical() << "getAbsolutePath() is empty. ";
			return false;
		}
		paths = this->getAbsolutePath();
	}
	else
	{
		if (this->getRelativePath().isEmpty()
				|| this->getRelativePath().first().isEmpty())
		{
			qCritical() << "getRelativePath() is empty.";
			return false;
		}

		QDir _rootDir(rootDirectory);
		for (QStringList::const_iterator cit = this->getRelativePath().cbegin();
				cit != this->getRelativePath().cend(); ++cit)
		{
			paths << _rootDir.absoluteFilePath(*cit);
		}
	}

	if (!this->getDataSet())
	{
		qCritical() << "data is a null ptr. ";
		return false;
	}
	
	bool returnValue = false;
	double orientation[3] = { 0, 0, 0 };
	double position[3] = { 0, 0, 0 };
	double scale[3] = { 1, 1, 1 };

	returnValue = readDataSuffix(paths, this->getImageData(), orientation, position, scale);
	// if using absolut path, use image's direction, origin, and spacing. 
	// if using relative path, the following will be ignored.
	if (rootDirectory.isEmpty()) {
		this->setOrientation(orientation);
		this->setPosition(position);
		this->setScale(scale);
	}
	// reset it to a best contrast if both are initialize value
	if (returnValue && 
		getWindow() == 0 && 
		getLevel() == 0) {
		resetWindowLevel();
	}
	return returnValue;
}

bool Image::writeData(QString rootDirectory) const
{
	QStringList paths;
	if (rootDirectory.isEmpty())
	{
		if (this->getAbsolutePath().isEmpty()
				|| this->getAbsolutePath().first().isEmpty())
		{
			qCritical() << "getAbsolutePath() is empty. ";
			return false;
		}
		paths = this->getAbsolutePath();
	}
	else
	{
		if (this->getRelativePath().isEmpty()
				|| this->getRelativePath().first().isEmpty())
		{
			qCritical() << "getRelativePath() is empty.";
			return false;
		}
		QDir _rootDir(rootDirectory);
		for (QStringList::const_iterator cit = this->getRelativePath().cbegin();
				cit != this->getRelativePath().cend(); ++cit)
		{
			paths << _rootDir.absoluteFilePath(*cit);
		}
	}

	if (!this->getDataSet())
	{
		qCritical() << "data is a null ptr. ";
		return false;
	}

	bool returnValue = false;
	double orientation[3] = { 0, 0, 0 };
	double position[3] = { 0, 0, 0 };
	double scale[3] = { 1, 1, 1 };

	// while in writing data, using image's direction, origin and spacing anyway. 
	// if using absolut path, use image's direction, origin, and spacing. 
	// if using relative path, do not use. 
	unsigned int type = suffixTranslate(QFileInfo(paths.first()).completeSuffix());
	if (rootDirectory.isEmpty()) {
		this->getOrientation(orientation);
		this->getPosition(position);
		this->getScale(scale);
		switch (type)
		{
		case Image::NIFTI:
		case Image::DICOM:
		{
			returnValue = writeITKImage(paths, this->getImageData(), orientation, position, scale);
		}
		break;
		case Image::VTI:
		case Image::VTK:
		{
			returnValue = writeDataSuffix(paths.first(), this->getTransformImageData());
		}
		break;
		default:
			returnValue = false;
			break;
		}
	}
	else {
		switch (type)
		{
		case Image::NIFTI:
		case Image::DICOM:
		{
			returnValue = writeITKImage(paths, this->getImageData(), orientation, position, scale);
		}
		break;
		case Image::VTI:
		case Image::VTK:
		{
			returnValue = writeDataSuffix(paths.first(), this->getImageData());
			break;
		}
		default:
			returnValue = false;
			break;
		}
	}
	return returnValue;
}

void Image::imageCoordinateToDataSetCoordinate(const int imageCoordinate[3], double dataSet[3]) const
{
	const double *spacing = this->getImageData()->GetSpacing();
	const double *origin = this->getImageData()->GetOrigin();
	dataSet[0] = imageCoordinate[0] * spacing[0] + origin[0];
	dataSet[1] = imageCoordinate[1] * spacing[1] + origin[1];
	dataSet[2] = imageCoordinate[2] * spacing[2] + origin[2];
}

void Image::dataSetCoordinateToImageCoordiante(const double dataSet[3], int imageCoordinate[3]) const
{
	const double *spacing = this->getImageData()->GetSpacing();
	const double *origin = this->getImageData()->GetOrigin();
	imageCoordinate[0] = vtkMath::Round((dataSet[0] - origin[0]) / spacing[0]);
	imageCoordinate[1] = vtkMath::Round((dataSet[1] - origin[1]) / spacing[1]);
	imageCoordinate[2] = vtkMath::Round((dataSet[2] - origin[2]) / spacing[2]);
}

void Image::imageCoordinateToWorldCoordinate(const int imageCoordinate[3], double worldCoordinate[3]) const
{
	double dataSet[3];
	this->imageCoordinateToDataSetCoordinate(imageCoordinate, dataSet);
	this->dataSetCoordinateToWorldCoordinate(dataSet, worldCoordinate);
}

void Image::worldCoordinateToImageCoordinate(const double worldCoordinate[3], int imageCoordinate[3]) const
{
	double dataSet[3];
	this->worldCoordinateToDataSetCoordinate(worldCoordinate, dataSet);
	this->dataSetCoordinateToImageCoordiante(dataSet, imageCoordinate);
}

void Image::vtkMatrix4x4ToitkAffineTransform(AffineTransformType *itkTransform, vtkMatrix4x4 *vtkMatrix)
{
	AffineTransformType::MatrixType matrix;
	AffineTransformType::OffsetType offset;
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			matrix[i][j] = vtkMatrix->GetElement(i, j);
		}
		offset[i] = vtkMatrix->GetElement(i, 3);
	}
	itkTransform->SetOffset(offset);
	itkTransform->SetMatrix(matrix);
}





vtkImageData* Image::getImageData() const
{
	return vtkImageData::SafeDownCast(this->getDataSet());
}

vtkImageData * Image::getTransformImageData() const
{
	this->getTransformOutputPort();
	return this->imageReslice->GetOutput();
}

bool Image::readDataSuffix(QStringList fileNames, vtkImageData * data, double orientation[3], double position[3], double scale[3], int suffix)
{
	if (data == nullptr) {
		qCritical() << "data is nullptr";
		return false;
	}

	if (fileNames.size() != 1) {

		return readITKImage(fileNames, data, orientation, position, scale);

	}
	else {
		QString fileName = fileNames.first();
		IMAGE_SUFFIX suffixType;
		if (suffix == -1) {
			suffixType = static_cast<IMAGE_SUFFIX>(suffixTranslate(QFileInfo(fileName).completeSuffix()));
		}
		else {
			suffixType = static_cast<IMAGE_SUFFIX>(suffix);
		}
		orientation[0] = 0;
		orientation[1] = 0;
		orientation[2] = 0;
		position[0] = 0;
		position[1] = 0;
		position[2] = 0;
		scale[0] = 1;
		scale[1] = 1;
		scale[2] = 1;
		switch (suffixType)
		{
		case Image::UNKNOWN:
		default:
			qCritical() << "Cannot read this file";
			data = nullptr;
			return false;
		case Image::VTK:
		{
			vtkNew<vtkImageReader> reader;
			reader->SetFileName(fileName.toStdString().c_str());
			reader->Update();
			reader->GetOutput()->SetSpacing(scale);
			reader->GetOutput()->SetOrigin(position);
			data->ShallowCopy(reader->GetOutput());
		}
		return true;
		case Image::VTI:
		{
			vtkNew<vtkXMLImageDataReader> reader;
			reader->SetFileName(fileName.toStdString().c_str());
			reader->Update();
			reader->GetOutput()->SetSpacing(scale);
			reader->GetOutput()->SetOrigin(position);
			data->ShallowCopy(reader->GetOutput());
		}
		return true;
		case Image::NIFTI:
		case Image::DICOM:
			return readITKImage(fileNames, data, orientation, position, scale);
		}
	}
}

bool Image::writeDataSuffix(QString fileName, vtkImageData * data, int suffix)
{
	if (data == nullptr) {
		qCritical() << "data is nullptr"; 
		return false;
	}
	IMAGE_SUFFIX suffixType;
	if (suffix == -1) {
		suffixType = static_cast<IMAGE_SUFFIX>(suffixTranslate(QFileInfo(fileName).completeSuffix()));
	}
	else {
		suffixType = static_cast<IMAGE_SUFFIX>(suffix);
	}
	switch (suffixType)
	{
	case Image::UNKNOWN:
	default:
		qCritical() << "Cannot write this file." << fileName;
		return false;
		break;
	case Image::VTK:
	{
		vtkNew<vtkImageWriter> writer;
		writer->SetInputData(data);
		writer->SetFileName(fileName.toStdString().c_str());
		writer->Write();

	}
	return true;
	case Image::VTI:
	{
		vtkNew<vtkXMLImageDataWriter> writer;
		writer->SetInputData(data);
		writer->SetFileName(fileName.toStdString().c_str());
		writer->Write();
	}
	return true;
	case Image::NIFTI:
	case Image::DICOM:
		qCritical() << "Writing image using this function is not supported. " <<
			"Please using Write ITK Image instead. ";
		return false;
	}
}

bool Image::writeDataSuffix(QStringList fileNames, vtkImageData * data, double orientation[3], double position[3], double scale[3], int suffix)
{
	if (data == nullptr) {
		qCritical() << "data is nullptr";
		return false;
	}

	if (fileNames.size() != 1) {
		qCritical() << "Write multiple DICOM format was not supported yet.";
		return false;
	}
	else {
		QString fileName = fileNames.first();
		IMAGE_SUFFIX suffixType;
		if (suffix == -1) {
			suffixType = static_cast<IMAGE_SUFFIX>(suffixTranslate(QFileInfo(fileName).completeSuffix()));
		}
		else {
			suffixType = static_cast<IMAGE_SUFFIX>(suffix);
		}
		switch (suffixType)
		{
		case Image::UNKNOWN:
		default:
			qCritical() << "Cannot write this file." << fileName;
			return false;
		case Image::VTK:
		case Image::VTI:
		{
			vtkNew<vtkImageData> _data;
			_data->DeepCopy(data);
			_data->SetSpacing(scale);
			_data->SetOrigin(position);
			return writeDataSuffix(fileName, _data.GetPointer());
		}
		case Image::NIFTI:
		{
			return writeITKImage(fileNames, data, orientation, position, scale);
		}
		case Image::DICOM:
			qCritical() << "Writing DICOM format was not supported yet. ";
		return false;
		}
	}
}

void Image::setWindow(double window)
{
	setAttribute(this->window, window);
	emit windowChanged(window);
}

double Image::getWindow() const
{
	return getAttribute(this->window).toDouble();
}

double Image::getResetWindow() const
{
	const double* range = getImageData()->GetScalarRange();
	return (range[1] - range[0]);
}

void Image::setLevel(double level)
{
	setAttribute(this->level, level);
	emit levelChanged(level);
}

double Image::getLevel() const
{
	return getAttribute(this->level).toDouble();
}

double Image::getResetLevel() const
{
	const double* range = getImageData()->GetScalarRange();
	return ((range[1] + range[0]) * 0.5);
}

vtkAlgorithmOutput * Image::getTransformOutputPort() const
{
	double spacing[3];
	this->getScale(spacing);
	this->imageChangeInformation->SetOutputSpacing(spacing);
	//this->imageChangeInformation->CenterImageOn();


	vtkTransform* transform = vtkTransform::New();
	transform->Identity();
	transform->PostMultiply();
	transform->Translate(
		-this->getOrigin()[0],
		-this->getOrigin()[1],
		-this->getOrigin()[2]);
	transform->RotateY(this->getOrientation()[1]);
	transform->RotateX(this->getOrientation()[0]);
	transform->RotateZ(this->getOrientation()[2]);
	transform->Translate(this->getPosition());
	//transform->Translate(
	//	-this->getPosition()[0],
	//	-this->getPosition()[1],
	//	-this->getPosition()[2]);
	transform->Translate(this->getOrigin());
	transform->Concatenate(this->getUserMatrix());
	//transform->Concatenate(this->getAdditionalMatrix());
	// do not know why it is inverse
	transform->Inverse();
	this->imageReslice->SetResliceTransform(transform);
	this->imageReslice->Update();
	transform->Delete();
	return this->imageReslice->GetOutputPort();
}

void Image::resetWindowLevel()
{
	//const double* range = getImageData()->GetScalarRange();
	//setWindow(range[1] - range[0]);be
	//setLevel((range[1] + range[0]) * 0.5);
	setWindow(this->getResetWindow());
	setLevel(this->getResetLevel());
}

void Image::setWindow(Data * self, QStandardItem * item)
{
	Image* _self = static_cast<Image*>(self);
	_self->setWindow(getAttribute(item).toDouble());
}

void Image::setLevel(Data * self, QStandardItem * item)
{
	Image* _self = static_cast<Image*>(self);
	_self->setLevel(getAttribute(item).toDouble());
}

unsigned int Image::suffixTranslate(QString suffix)
{
	if (suffix.contains("vtk")) {
		return VTK;
	}
	else if (suffix.contains("vti")) {
		return VTI;
	}
	else if (suffix.contains("nii") || suffix.contains("nii.gz")) {
		return NIFTI;
	}
	else if (suffix.contains("DCM", Qt::CaseInsensitive) || suffix.isEmpty()) {
		return DICOM;
	}
	else {
		return UNKNOWN;
	}
}
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<unsigned char, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<char, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<unsigned short, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<short, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<unsigned int, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<int, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<unsigned long, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<long, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<float, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::getITKImageData(itk::Image<double, 3>* itkImage) const;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<unsigned char, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<char, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<unsigned short, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<short, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<unsigned int, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<int, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<unsigned long, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<long, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<float, 3>* itkImage) ;
//template QVTKDATA_EXPORT void Image::setITKImageData(itk::Image<double, 3>* itkImage) ;
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<unsigned char, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<char, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<unsigned short, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<short, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<unsigned int, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<int, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<unsigned long, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<long, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<float, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_VTKImageToITKImage(itk::Image<double, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<unsigned char, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<char, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<unsigned short, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<short, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<unsigned int, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<int, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<unsigned long, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<long, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<float, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
//template QVTKDATA_EXPORT bool Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<double, 3>* output,  double orientation[3],  double position[3],  double scale[3]);


}
}