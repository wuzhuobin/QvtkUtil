#ifndef __QVTK_IMAGE_IO__WRITE_ITK_IMAGE_HPP__
#define __QVTK_IMAGE_IO__WRITE_ITK_IMAGE_HPP__
// me 
#include "QvtkImage.h"
// vtk
#include <vtkImageData.h>
// itk
#include <itkImage.h>
#include <itkAffineTransform.h>
#include <itkImageFileWriter.h>
#include <itkResampleImageFilter.h>
// qt 
#include <QDebug>
using namespace Q::vtk;
template<typename PixelType>
bool Image::writeImage(QStringList paths, vtkImageData* image, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix)
{
	using namespace itk;
	typedef itk::Image< PixelType, 3> TImageType;
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
		if (userMatrix != nullptr) {
			AffineTransformType::Pointer itkTransform = AffineTransformType::New();
			vtkMatrix4x4ToitkAffineTransform(itkTransform, userMatrix);
			typedef typename itk::ResampleImageFilter<itk::Image<PixelType, 3>, itk::Image<PixelType, 3>> ResampleImageFilter;
			typename ResampleImageFilter::Pointer resampleImageFilter = ResampleImageFilter::New();
			resampleImageFilter->SetInput(itkImage);
			resampleImageFilter->SetSize(itkImage->GetLargestPossibleRegion().GetSize());
			resampleImageFilter->SetTransform(itkTransform);
			resampleImageFilter->Update();
			itkImage = resampleImageFilter->GetOutput();
		}
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
#endif // !__QVTK_IMAGE_IO__WRITE_ITK_IMAGE_HPP__