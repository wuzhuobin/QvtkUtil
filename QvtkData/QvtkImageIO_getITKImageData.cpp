// me 
#include "QvtkImage.h"
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
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type) \
template QVTKDATA_EXPORT void Q::vtk::Image::getITKImageData(itk::Image<type, 3>* itkImage) const;
QVTK_IMAGE_EXPORT_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type, component) \
template QVTKDATA_EXPORT void Q::vtk::Image::getITKImageData(itk::Image<itk::Vector<type, component>, 3>* itkImage) const;
QVTK_IMAGE_EXPORT_VECTOR_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE