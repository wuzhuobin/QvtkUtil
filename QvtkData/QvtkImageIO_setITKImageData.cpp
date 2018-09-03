// me
#include "QvtkImage.h"
// itk
#include <itkImage.h>
#include <itkOrientImageFilter.h>
template<typename PixelType>
void Q::vtk::Image::setITKImageData(itk::Image<PixelType, 3>* itkImage)
{
	typedef itk::Image<PixelType, 3> TImageType;
	typedef itk::OrientImageFilter< TImageType, TImageType > OrientImageFilter;
	typename OrientImageFilter::Pointer orientImageFilter =
		OrientImageFilter::New();
	orientImageFilter->SetInput(itkImage);
	orientImageFilter->UseImageDirectionOn();
	orientImageFilter->SetDesiredCoordinateOrientation(itk::SpatialOrientation::ITK_COORDINATE_ORIENTATION_RAI);
	orientImageFilter->Update();
	double orientation[3];
	double position[3];
	double scale[3];
	_ITKImageToVTKImage(this->getImageData(), orientImageFilter->GetOutput(), orientation, position, scale);
	this->setOrientation(orientation);
	this->setPosition(position);
	this->setScale(scale);
}
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type) \
template QVTKDATA_EXPORT void Q::vtk::Image::setITKImageData(itk::Image<type, 3>* itkImage);
QVTK_IMAGE_EXPORT_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type, component) \
template QVTKDATA_EXPORT void Q::vtk::Image::setITKImageData(itk::Image<itk::Vector<type, component>, 3>* itkImage);
QVTK_IMAGE_EXPORT_VECTOR_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
