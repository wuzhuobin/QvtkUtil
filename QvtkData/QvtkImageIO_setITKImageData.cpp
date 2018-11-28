// me
#include "QvtkImage.h"
#include "QvtkImageIO_setITKImageData.hpp"
// itk
#include <itkImage.h>
#include <itkOrientImageFilter.h>
#ifdef QVTK_MANUAL_INSTANTIATION
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type) \
template QVTKDATA_EXPORT void Q::vtk::Image::setITKImageData(itk::Image<type, 3>* itkImage);
QVTK_IMAGE_EXPORT_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type, component) \
template QVTKDATA_EXPORT void Q::vtk::Image::setITKImageData(itk::Image<itk::Vector<type, component>, 3>* itkImage);
QVTK_IMAGE_EXPORT_VECTOR_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#endif // QVTK_MANUAL_INSTANTIATION