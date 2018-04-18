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
#include <vtkImageChangeInformation.h>
#include <vtkImageActor.h>
#include <vtkImageProperty.h>

//itk
#include <itkImage.h>
#include <itkImageIOBase.h>
#include <itkImageIOFactory.h>
#include <itkImageFileWriter.h>
#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkImageToVTKImageFilter.h>
#include <itkVTKImageToImageFilter.h>
#include <itkOrientImageFilter.h>
#include <itkChangeInformationImageFilter.h>
#include <itkCastImageFilter.h>

//qt
#include <QDebug>
#include <QDir>
#include <QFileInfo>

Q_VTK_DATACPP(QvtkImage)
QvtkImage::QvtkImage()
{
	vtkImageData* data = vtkImageData::New();
	setDataSet(data);
	data->Delete();

	this->window = createAttribute(K.Window, static_cast<double>(0), true);
	insertSlotFunction(this->window, &QvtkImage::setWindow);

	this->level = createAttribute(K.Level, static_cast<double>(0), true);
	insertSlotFunction(this->level, &QvtkImage::setLevel);


	this->imageChangeInformation = vtkImageChangeInformation::New();
	this->imageChangeInformation->SetInputConnection(this->getOutputPort());

	this->imageReslice = vtkImageReslice::New();
	this->imageReslice->SetInputConnection(this->imageChangeInformation->GetOutputPort());
	this->imageReslice->AutoCropOutputOn();
	this->imageReslice->TransformInputSamplingOn();
}

QvtkImage::~QvtkImage()
{
	this->imageReslice->Delete();
}

void QvtkImage::printSelf() const
{
	DataSet::printSelf();

}


bool QvtkImage::readData(QString rootDirectory)
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

bool QvtkImage::writeData(QString rootDirectory) const
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
		case QvtkImage::NIFTI:
		case QvtkImage::DICOM:
		{
			returnValue = writeITKImage(paths, this->getImageData(), orientation, position, scale);
		}
		break;
		case QvtkImage::VTI:
		case QvtkImage::VTK:
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
		case QvtkImage::NIFTI:
		case QvtkImage::DICOM:
		{
			returnValue = writeITKImage(paths, this->getImageData(), orientation, position, scale);
		}
		break;
		case QvtkImage::VTI:
		case QvtkImage::VTK:
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

void QvtkImage::addReference(QvtkAbstractProp * prop)
{
	DataSet::addReference(prop);
	QvtkImageSlice* slice = qobject_cast<QvtkImageSlice*>(prop);
	if (slice) {
		slice->getImageActor()->GetProperty()->SetInterpolationTypeToLinear();
	}
}

bool QvtkImage::readITKImage(QStringList paths, vtkImageData * image, double orientation[3], double position[3], double scale[3])
{
	itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO(
		paths.first().toStdString().c_str(), itk::ImageIOFactory::ReadMode);

	if (!imageIO)
	{
		qCritical() << "Could not Create ImageIO for:" << paths.first();
		return false;
	}

	imageIO->SetFileName(paths.first().toStdString());
	imageIO->ReadImageInformation();

	typedef itk::ImageIOBase::IOComponentType IOComponentType;

	bool returnValue = false;

	switch (imageIO->GetComponentType())
	{
		Qvtk_ITK_TEMPLATE_MACRO(return readImage<ITK_IMAGE_IO_BASE_IO_COMPONENT_TYPE>(
			paths, image, orientation, position, scale));
	}
	return false;

}

template<typename PixelType>
bool QvtkImage::_VTKImageToITKImage(itk::Image<PixelType, 3> * output, vtkImageData * input, const double orientation[3], const double position[3], const double scale[3])
{
	using namespace itk;
	if (!input) {
		qCritical() << "input is an nullptr.";
		return false;
	}

	if (!output) {
		qCritical() << "output is an nullptr.";
		return false;
	}
	//typedef Image<VTK_TT, 3> vtkImageType;
	typedef Image<PixelType, 3> TImageType;
	typename TImageType::Pointer itkImage = TImageType::New();
	switch (input->GetScalarType())
	{
		vtkTemplateMacro((_ITKImageCasting<PixelType, VTK_TT>(itkImage, input)));
	}
	typename TImageType::PointType origin;
	typename TImageType::DirectionType direction;
	typename TImageType::SpacingType spacing;

	vtkTransform* trans = vtkTransform::New();
	trans->Identity();
	trans->RotateY(orientation[1]);
	trans->RotateX(orientation[0]);
	trans->RotateZ(orientation[2]);
	trans->Update();

	origin[0] = position[0];
	origin[1] = position[1];
	origin[2] = position[2];

	spacing[0] = scale[0];
	spacing[1] = scale[1];
	spacing[2] = scale[2];

	// Setting direction and origin
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{

			direction[i][j] = trans->GetMatrix()->GetElement(i, j);
		}
	}
	trans->Delete();

	typedef itk::ChangeInformationImageFilter<TImageType> ChangeInformationImageFilter;
	typename ChangeInformationImageFilter::Pointer changeInformationImageFilter =
		ChangeInformationImageFilter::New();
	changeInformationImageFilter->SetInput(itkImage);
	changeInformationImageFilter->SetOutputOrigin(origin);
	changeInformationImageFilter->SetOutputSpacing(spacing);
	changeInformationImageFilter->SetOutputDirection(direction);
	changeInformationImageFilter->ChangeOriginOn();
	changeInformationImageFilter->ChangeSpacingOn();
	changeInformationImageFilter->ChangeDirectionOn();
	changeInformationImageFilter->Update();
	output->Graft(changeInformationImageFilter->GetOutput());
	return true;



}

template<typename PixelType>
bool QvtkImage::_ITKImageToVTKImage(vtkImageData * output, itk::Image<PixelType, 3>* input, double orientation[3], double position[3], double scale[3])
{
	using namespace itk;
	typedef itk::Image<PixelType, 3> TImageType;

	const typename TImageType::DirectionType& direction = input->GetDirection();
	const typename TImageType::PointType& origin = input->GetOrigin();
	const typename TImageType::SpacingType& spacing = input->GetSpacing();

	vtkMatrix4x4* matrix = vtkMatrix4x4::New();
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			matrix->SetElement(i, j, direction[i][j]);
		}
	}

	vtkTransform::GetOrientation(orientation, matrix);
	matrix->Delete();
	position[0] = origin[0];
	position[1] = origin[1];
	position[2] = origin[2];

	scale[0] = spacing[0];
	scale[1] = spacing[1];
	scale[2] = spacing[2];

	typedef ImageToVTKImageFilter<TImageType> ImageToVTKImageFilter;
	typename ImageToVTKImageFilter::Pointer toVTKImage =
			ImageToVTKImageFilter::New();
	toVTKImage->SetInput(input);
	toVTKImage->Update();

	vtkSmartPointer<vtkImageChangeInformation> imageChangeInformation =
		vtkSmartPointer<vtkImageChangeInformation>::New();
	imageChangeInformation->SetInputData(toVTKImage->GetOutput());
	imageChangeInformation->SetOutputOrigin(0, 0, 0);
	imageChangeInformation->SetOutputSpacing(1, 1, 1);
	imageChangeInformation->Update();

	// do not use ShallowCopy
	output->DeepCopy(imageChangeInformation->GetOutput());
	return true;
}

template<typename PixelType>
bool QvtkImage::readImage(QStringList paths, vtkImageData * image, double orientation[3], double position[3], double scale[3])
{
	using namespace itk;
	//typedef float PixelType;
	typedef Image< PixelType, 3> TImageType;
	typename TImageType::Pointer itkImage = nullptr;

	if (paths.isEmpty() && paths.first().isEmpty())
	{
		qCritical() << "The paths are empty. ";
		return false;
	}
	else if (paths.size() == 1)
	{

		try
		{
			typedef ImageFileReader<TImageType> ImageFileReader;
			typename ImageFileReader::Pointer reader =
					ImageFileReader::New();
			reader->SetFileName(paths.first().toStdString());
			reader->Update();
			itkImage = reader->GetOutput();
		} catch (ImageFileReaderException& e)
		{
			qCritical() << "What:" << e.what();
			qCritical() << "Description: " << e.GetDescription();
			return false;
		}

	}
	else
	{

		try
		{
			typedef ImageSeriesReader<TImageType> ImageSeriesReader;
			std::vector<std::string> _fileNames;
			for (QStringList::const_iterator constIterator = paths.constBegin();
					constIterator != paths.constEnd(); ++constIterator)
			{
				_fileNames.push_back(constIterator->toStdString());
			}
			typename ImageSeriesReader::Pointer reader =
					ImageSeriesReader::New();
			reader->SetFileNames(_fileNames);
			reader->Update();
			itkImage = reader->GetOutput();

		} catch (ExceptionObject& e)
		{
			qCritical() << "What: " << e.what();
			qCritical() << "Description" << e.GetDescription();
			return false;
		}

	}

	typedef itk::OrientImageFilter< TImageType, TImageType > OrientImageFilter;
	typename OrientImageFilter::Pointer orientImageFilter =
		OrientImageFilter::New();
	orientImageFilter->SetInput(itkImage);
	orientImageFilter->UseImageDirectionOn();
	orientImageFilter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orientImageFilter->Update();

	bool returnValue = _ITKImageToVTKImage<PixelType>(image, orientImageFilter->GetOutput(), orientation, position, scale);
	return returnValue;

}

template<typename OPixelType, typename IPixelType>
void QvtkImage::_ITKImageCasting(itk::Image<OPixelType, 3>* output, vtkImageData* input)
{
	using namespace itk;
	typedef Image<OPixelType, 3> OutputImageType;
	typedef Image<IPixelType, 3> InputImageType;

	typedef VTKImageToImageFilter<InputImageType> VTKImageToImageFilter;
	typename VTKImageToImageFilter::Pointer vtkImageToImageFilter =
		VTKImageToImageFilter::New();
	vtkImageToImageFilter->SetInput(input);
	vtkImageToImageFilter->Update();


	typedef itk::CastImageFilter<InputImageType, OutputImageType> CastImageFilter;
	typename CastImageFilter::Pointer castImageFilter =
		CastImageFilter::New();
	castImageFilter->SetInput(vtkImageToImageFilter->GetOutput());
	castImageFilter->Update();
	output->Graft(castImageFilter->GetOutput());

}
template void QvtkImage::_ITKImageCasting<double, double>(itk::Image<double, 3>* output, vtkImageData* image);

//
//template<typename PixelType>
//bool QvtkImage::writeImage(QStringList paths) const
//{
//	using namespace itk;
////	typedef float PixelType;
//	typedef Image< PixelType, 3> TImageType;
//
//	typename TImageType::Pointer itkImage = nullptr;
//	typedef VTKImageToImageFilter<TImageType> VTKImageToImageFilter;
//	typename VTKImageToImageFilter::Pointer toImageFilter =
//			VTKImageToImageFilter::New();
//	toImageFilter->SetInput(getImageData());
//	toImageFilter->Update();
//	//itkImage = toImageFilter->GetOutput();
//
//	typename TImageType::PointType origin;
//	typename TImageType::DirectionType direction;
//	typename TImageType::SpacingType spacing;
//
//	vtkTransform* trans = vtkTransform::New();
//	trans->Identity();
//	trans->RotateY(getOrientation()[1]);
//	trans->RotateX(getOrientation()[0]);
//	trans->RotateZ(getOrientation()[2]);
//	trans->Update();
//
//	origin[0] = getPosition()[0];
//	origin[1] = getPosition()[1];
//	origin[2] = getPosition()[2];
//
//	spacing[0] = getScale()[0];
//	spacing[1] = getScale()[1];
//	spacing[2] = getScale()[2];
//
//	// Setting direction and origin
//	for (int i = 0; i < 3; ++i)
//	{
//		for (int j = 0; j < 3; ++j)
//		{
//			
//			direction[i][j] =  trans->GetMatrix()->GetElement(i, j);
//		}
//		//origin[i] = this->userMatrix->GetElement(i, 3);
//	}
//	trans->Delete();
//	//itkImage->SetOrigin(origin);
//	//itkImage->SetDirection(direction);
//	toImageFilter->GetOutput()->SetOrigin(origin);
//	toImageFilter->GetOutput()->SetDirection(direction);
//	toImageFilter->GetOutput()->SetSpacing(spacing);
//
//	if (paths.isEmpty() && paths.first().isEmpty())
//	{
//		return false;
//	}
//	else if (paths.size() == 1)
//	{
//
//		try {
//			typedef ImageFileWriter<TImageType> ImageFileWriter;
//
//			typename ImageFileWriter::Pointer writer = ImageFileWriter::New();
//			//writer->SetInput(itkImage);
//			writer->SetInput(toImageFilter->GetOutput());
//			writer->SetFileName(paths.first().toStdString());
//			writer->Write();
//		}
//		catch (ImageFileWriterException& e) {
//			qCritical() << "What: " << e.what();
//			qCritical() << "Description: " << e.GetDescription();
//			return false;
//		}
//
//	}
//	else
//	{
//		qWarning() << "NOTHING";
//	}
//	return true;
//}

bool QvtkImage::writeITKImage(QStringList paths, vtkImageData* image, const double orientation[3], const double position[3], const double scale[3])
{
	switch (image->GetScalarType())
	{
		vtkTemplateMacro(return writeImage<VTK_TT>(paths, image, orientation, position, scale));
	}
	return false;
}

template<typename PixelType>
bool QvtkImage::writeImage(QStringList paths, vtkImageData* image, const double orientation[3], const double position[3], const double scale[3])
{
	using namespace itk;
//	typedef float PixelType;
	typedef Image< PixelType, 3> TImageType;

	typename TImageType::Pointer itkImage = TImageType::New();
	bool returnValue = _VTKImageToITKImage<PixelType>(itkImage, image, orientation, position, scale);

	if (!returnValue) {
		qCritical() << "vtkImageTo_itkImage failed. ";
		return false;
	}
	else if (paths.isEmpty() && paths.first().isEmpty())
	{
		qCritical() << "The paths are empty. ";
		return false;
	}
	else if (paths.size() == 1)
	{

		try {
			typedef ImageFileWriter<TImageType> ImageFileWriter;

			typename ImageFileWriter::Pointer writer = ImageFileWriter::New();
			//writer->SetInput(itkImage);
			writer->SetInput(itkImage);
			writer->SetFileName(paths.first().toStdString());
			writer->Write();
		}
		catch (ImageFileWriterException& e) {
			qCritical() << "What: " << e.what();
			qCritical() << "Description: " << e.GetDescription();
			return false;
		}

	}
	else
	{
		qWarning() << "NOTHING";
	}
	return true;
}

template<typename PixelType>
void QvtkImage::getITKImageData(itk::Image<PixelType, 3>* itkImage) const
{
	_VTKImageToITKImage(itkImage, this->getImageData(), this->getOrientation(), this->getPosition(), this->getScale());
}
template void QvtkImage::getITKImageData(itk::Image<unsigned char, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<char, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<unsigned short, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<short, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<unsigned int, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<int, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<unsigned long, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<long, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<float, 3>* itkImage) const;
template void QvtkImage::getITKImageData(itk::Image<double, 3>* itkImage) const;

vtkImageData* QvtkImage::getImageData() const
{
	return vtkImageData::SafeDownCast(this->getDataSet());
}

vtkImageData * QvtkImage::getTransformImageData() const
{
	this->getTransformOutputPort();
	return this->imageReslice->GetOutput();
}

bool QvtkImage::readDataSuffix(QStringList fileNames, vtkImageData * data, double orientation[3], double position[3], double scale[3], int suffix)
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
		case QvtkImage::UNKNOWN:
		default:
			qCritical() << "Cannot read this file";
			data = nullptr;
			return false;
		case QvtkImage::VTK:
		{
			vtkNew<vtkImageReader> reader;
			reader->SetFileName(fileName.toStdString().c_str());
			reader->Update();
			reader->GetOutput()->SetSpacing(scale);
			reader->GetOutput()->SetOrigin(position);
			data->ShallowCopy(reader->GetOutput());
		}
		return true;
		case QvtkImage::VTI:
		{
			vtkNew<vtkXMLImageDataReader> reader;
			reader->SetFileName(fileName.toStdString().c_str());
			reader->Update();
			reader->GetOutput()->SetSpacing(scale);
			reader->GetOutput()->SetOrigin(position);
			data->ShallowCopy(reader->GetOutput());
		}
		return true;
		case QvtkImage::NIFTI:
		case QvtkImage::DICOM:
			return readITKImage(fileNames, data, orientation, position, scale);
		}
	}
}

bool QvtkImage::writeDataSuffix(QString fileName, vtkImageData * data, int suffix)
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
	case QvtkImage::UNKNOWN:
	default:
		qCritical() << "Cannot write this file." << fileName;
		return false;
		break;
	case QvtkImage::VTK:
	{
		vtkNew<vtkImageWriter> writer;
		writer->SetInputData(data);
		writer->SetFileName(fileName.toStdString().c_str());
		writer->Write();

	}
	return true;
	case QvtkImage::VTI:
	{
		vtkNew<vtkXMLImageDataWriter> writer;
		writer->SetInputData(data);
		writer->SetFileName(fileName.toStdString().c_str());
		writer->Write();
	}
	return true;
	case QvtkImage::NIFTI:
	case QvtkImage::DICOM:
		qCritical() << "Writing image using this function is not supported. " <<
			"Please using Write ITK Image instead. ";
		return false;
	}
}

bool QvtkImage::writeDataSuffix(QStringList fileNames, vtkImageData * data, double orientation[3], double position[3], double scale[3], int suffix)
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
		case QvtkImage::UNKNOWN:
		default:
			qCritical() << "Cannot write this file." << fileName;
			return false;
		case QvtkImage::VTK:
		case QvtkImage::VTI:
		{
			vtkNew<vtkImageData> _data;
			_data->DeepCopy(data);
			_data->SetSpacing(scale);
			_data->SetOrigin(position);
			return writeDataSuffix(fileName, _data.GetPointer());
		}
		case QvtkImage::NIFTI:
		{
			return writeITKImage(fileNames, data, orientation, position, scale);
		}
		case QvtkImage::DICOM:
			qCritical() << "Writing DICOM format was not supported yet. ";
		return false;
		}
	}
}

void QvtkImage::setWindow(double window)
{
	setAttribute(this->window, window);
	emit windowChanged(window);
}

double QvtkImage::getWindow() const
{
	return getAttribute(this->window).toDouble();
}

void QvtkImage::setLevel(double level)
{
	setAttribute(this->level, level);
	emit levelChanged(level);
}

double QvtkImage::getLevel() const
{
	return getAttribute(this->level).toDouble();
}

vtkAlgorithmOutput * QvtkImage::getTransformOutputPort() const
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
	transform->Concatenate(this->getAdditionalMatrix());
	// do not know why it is inverse
	transform->Inverse();
	this->imageReslice->SetResliceTransform(transform);
	this->imageReslice->Update();
	transform->Delete();
	return this->imageReslice->GetOutputPort();
}

void QvtkImage::resetWindowLevel()
{
	const double* range = getImageData()->GetScalarRange();
	setWindow(range[1] - range[0]);
	setLevel((range[1] + range[0]) * 0.5);
}

void QvtkImage::setWindow(Data * self, QStandardItem * item)
{
	QvtkImage* _self = static_cast<QvtkImage*>(self);
	_self->setWindow(getAttribute(item).toDouble());
}

void QvtkImage::setLevel(Data * self, QStandardItem * item)
{
	QvtkImage* _self = static_cast<QvtkImage*>(self);
	_self->setLevel(getAttribute(item).toDouble());
}

unsigned int QvtkImage::suffixTranslate(QString suffix)
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
	else if (suffix.contains("DCM") || suffix.isEmpty()) {
		return DICOM;
	}
	else {
		return UNKNOWN;
	}
}

