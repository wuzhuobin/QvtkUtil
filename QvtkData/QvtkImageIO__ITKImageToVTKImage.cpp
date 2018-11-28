// me
#include "QvtkImage.h"
#include "QvtkImageIO__ITKImageToVTKImage.hpp"
#ifdef QVTK_MANUAL_INSTANTIATION
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type) \
template QVTKDATA_EXPORT bool Q::vtk::Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<type, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
QVTK_IMAGE_EXPORT_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type, component) \
template QVTKDATA_EXPORT bool Q::vtk::Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<itk::Vector<type, component>, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
QVTK_IMAGE_EXPORT_VECTOR_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#endif // QVTK_MANUAL_INSTANTIATION