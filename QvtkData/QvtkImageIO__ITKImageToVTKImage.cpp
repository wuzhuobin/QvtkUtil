// me
#include "QvtkImage.h"
// vtk
#include <vtkImageData.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkImageChangeInformation.h>
#include <vtkSmartPointer.h>
// itk 
//#include <itkVector.h>
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
template<typename PixelType>
bool Q::vtk::Image::_ITKImageToVTKImage(vtkImageData * output, itk::Image<PixelType, 3>* input, double orientation[3], double position[3], double scale[3])
{
	using namespace itk;
	typedef itk::Image<PixelType, 3> TImageType;

	const typename TImageType::DirectionType& direction = input->GetDirection();
	const typename TImageType::PointType& origin = input->GetOrigin();
	const typename TImageType::SpacingType& spacing = input->GetSpacing();

	vtkMatrix4x4* matrix = vtkMatrix4x4::New();
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			matrix->SetElement(i, j, direction[i][j]);
		}
	}

	vtkTransform::GetOrientation(orientation, matrix);
	matrix->Delete();
	position[0] = origin[0];
	position[1] = origin[1];
	position[2] = origin[2];

	scale[0] = spacing[0];
	scale[1] = spacing[1];
	scale[2] = spacing[2];

	typedef ImageToVTKImageFilter<TImageType> ImageToVTKImageFilter;
	typename ImageToVTKImageFilter::Pointer toVTKImage =
			ImageToVTKImageFilter::New();
	toVTKImage->SetInput(input);
	toVTKImage->Update();

	vtkSmartPointer<vtkImageChangeInformation> imageChangeInformation =
		vtkSmartPointer<vtkImageChangeInformation>::New();
	imageChangeInformation->SetInputData(toVTKImage->GetOutput());
	imageChangeInformation->SetOutputOrigin(0, 0, 0);
	imageChangeInformation->SetOutputSpacing(1, 1, 1);
	imageChangeInformation->Update();

	// do not use ShallowCopy
	output->DeepCopy(imageChangeInformation->GetOutput());
	return true;
}
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type) \
template QVTKDATA_EXPORT bool Q::vtk::Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<type, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
QVTK_IMAGE_EXPORT_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type, component) \
template QVTKDATA_EXPORT bool Q::vtk::Image::_ITKImageToVTKImage(vtkImageData *image, itk::Image<itk::Vector<type, component>, 3>* output,  double orientation[3],  double position[3],  double scale[3]);
QVTK_IMAGE_EXPORT_VECTOR_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE