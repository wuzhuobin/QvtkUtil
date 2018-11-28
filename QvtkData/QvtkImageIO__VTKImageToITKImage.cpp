// me 
#include "QvtkImage.h"
#include "QvtkImageIO__VTKImageToITKImage.hpp"
#ifdef QVTK_MANUAL_INSTANTIATION
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type) \
template QVTKDATA_EXPORT bool Q::vtk::Image::_VTKImageToITKImage(itk::Image<type, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
QVTK_IMAGE_EXPORT_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type, component) \
template QVTKDATA_EXPORT bool Q::vtk::Image::_VTKImageToITKImage(itk::Image<itk::Vector<type, component>, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
QVTK_IMAGE_EXPORT_VECTOR_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#endif // QVTK_MANUAL_INSTANTIATION