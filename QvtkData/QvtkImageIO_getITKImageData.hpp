#ifndef __QVTK_IMAGE_IO__GET_ITK_IMAGE_DATA_HPP__
#define __QVTK_IMAGE_IO__GET_ITK_IMAGE_DATA_HPP__
// me 
#include "QvtkImage.h"
#include "QvtkImageIO_getITKImageData.hpp"
// itk
#include <itkAffineTransform.h>
#include <itkResampleImageFilter.h>
template<typename PixelType>
void Q::vtk::Image::getITKImageData(itk::Image<PixelType, 3>* itkImage) const
{
	_VTKImageToITKImage(itkImage, this->getImageData(), this->getOrientation(), this->getPosition(), this->getScale());
	AffineTransformType::Pointer itkTransform = AffineTransformType::New();
	vtkMatrix4x4ToitkAffineTransform(itkTransform, this->getUserMatrix());
	typedef typename itk::ResampleImageFilter<itk::Image<PixelType, 3>, itk::Image<PixelType, 3>> ResampleImageFilter;
	ResampleImageFilter::Pointer resampleImageFilter = ResampleImageFilter::New();
	resampleImageFilter->SetInput(itkImage);
	resampleImageFilter->SetSize(itkImage->GetLargestPossibleRegion().GetSize());
	resampleImageFilter->SetTransform(itkTransform);
	resampleImageFilter->Update();
	itkImage->Graft(resampleImageFilter->GetOutput());
}
#endif // !__QVTK_IMAGE_IO__GET_ITK_IMAGE_DATA_HPP__