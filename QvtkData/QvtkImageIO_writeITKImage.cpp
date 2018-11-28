// me 
#include "QvtkImage.h"
#include "QvtkImageIO_writeITKImage.hpp"
bool Image::writeITKImage(QStringList paths, vtkImageData* image, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix)
{
	switch (image->GetScalarType())
	{
		vtkTemplateMacro(return writeImage<VTK_TT>(paths, image, orientation, position, scale));
	}
	return false;
}