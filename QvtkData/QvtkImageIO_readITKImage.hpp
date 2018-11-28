#ifndef __QVTK_IMAGE_IO__READ_ITK_IMAGE_HPP__
#define __QVTK_IMAGE_IO__READ_ITK_IMAGE_HPP__
// me
#include "QvtkImage.h"
// itk
#include <itkImageIOBase.h>
#include <itkImageIOFactory.h>
#include <itkImageFileReader.h>
#include <itkImageSeriesReader.h>
#include <itkOrientImageFilter.h>
// qt 
#include <QDebug>
template<typename PixelType>
bool Q::vtk::Image::readImage(QStringList paths, vtkImageData * image, double orientation[3], double position[3], double scale[3])
{
	using namespace itk;
	//typedef float PixelType;
	typedef itk::Image< PixelType, 3> TImageType;
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

//typedef short OPixelType;
//typedef int IPixelType;

//template void Image::_ITKImageCasting<double, double>(itk::Image<double, 3>* output, vtkImageData* image);
//
//template<typename PixelType>
//bool Image::writeImage(QStringList paths) const
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
#endif // !__QVTK_IMAGE_IO__READ_ITK_IMAGE_HPP__