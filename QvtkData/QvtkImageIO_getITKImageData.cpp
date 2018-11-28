// me 
#include "QvtkImage.h"
#include "QvtkImageIO_getITKImageData.hpp"
#ifdef QVTK_MANUAL_INSTANTIATION
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type) \
template QVTKDATA_EXPORT void Q::vtk::Image::getITKImageData(itk::Image<type, 3>* itkImage) const;
QVTK_IMAGE_EXPORT_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type, component) \
template QVTKDATA_EXPORT void Q::vtk::Image::getITKImageData(itk::Image<itk::Vector<type, component>, 3>* itkImage) const;
QVTK_IMAGE_EXPORT_VECTOR_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#endif // QVTK_MANUAL_INSTANTIATION