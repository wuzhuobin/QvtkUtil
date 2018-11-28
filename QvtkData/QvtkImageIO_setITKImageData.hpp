#ifndef __QVTK_IMAGE_IO__SET_ITK_IMAGE_DATA_HPP__
#define __QVTK_IMAGE_IO__SET_ITK_IMAGE_DATA_HPP__
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
#endif // !__QVTK_IMAGE_IO__SET_ITK_IMAGE_DATA_HPP__