// me 
#include "QvtkImage.h"
// vtk
#include <vtkTransform.h>
#include <vtkImageData.h>
// itk
#include <itkChangeInformationImageFilter.h>
#include <itkCastImageFilter.h>
#include <itkVTKImageToImageFilter.h>
// qt 
#include <QDebug>
template<typename PixelType>
bool Q::vtk::Image::_VTKImageToITKImage(itk::Image<PixelType, 3> * output, vtkImageData * input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix)
{
	using namespace itk;
	if (!input) {
		qCritical() << "input is an nullptr.";
		return false;
	}
	if (!output) {
		qCritical() << "output is an nullptr.";
		return false;
	}
	typedef itk::Image<PixelType, 3> TImageType;
	typename TImageType::Pointer itkImage = TImageType::New();
	switch (input->GetScalarType())
	{
		vtkTemplateMacro((_ITKImageCasting<PixelType, VTK_TT>(itkImage, input)));
	}
	typename TImageType::PointType origin;
	typename TImageType::DirectionType direction;
	typename TImageType::SpacingType spacing;
	vtkTransform* trans = vtkTransform::New();
	trans->Identity();
	trans->RotateY(orientation[1]);
	trans->RotateX(orientation[0]);
	trans->RotateZ(orientation[2]);
	trans->Update();
	origin[0] = position[0];
	origin[1] = position[1];
	origin[2] = position[2];
	spacing[0] = scale[0];
	spacing[1] = scale[1];
	spacing[2] = scale[2];
	// Setting direction and origin
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			direction[i][j] = trans->GetMatrix()->GetElement(i, j);
		}
	}
	trans->Delete();
	typedef itk::ChangeInformationImageFilter<TImageType> ChangeInformationImageFilter;
	typename ChangeInformationImageFilter::Pointer changeInformationImageFilter =
		ChangeInformationImageFilter::New();
	changeInformationImageFilter->SetInput(itkImage);
	changeInformationImageFilter->SetOutputOrigin(origin);
	changeInformationImageFilter->SetOutputSpacing(spacing);
	changeInformationImageFilter->SetOutputDirection(direction);
	changeInformationImageFilter->ChangeOriginOn();
	changeInformationImageFilter->ChangeSpacingOn();
	changeInformationImageFilter->ChangeDirectionOn();
	changeInformationImageFilter->Update();
	output->Graft(changeInformationImageFilter->GetOutput());
	return true;
}
template<typename OPixelType, typename IPixelType>
void Q::vtk::Image::_ITKImageCasting(itk::Image<OPixelType, 3>* output, vtkImageData* input)
{
	using namespace itk;
	typedef itk::Image<OPixelType, 3> OutputImageType;
	typedef itk::Image<IPixelType, 3> InputImageType;
	typedef VTKImageToImageFilter<InputImageType> VTKImageToImageFilter;
	typename VTKImageToImageFilter::Pointer vtkImageToImageFilter =
		VTKImageToImageFilter::New();
	vtkImageToImageFilter->SetInput(input);
	vtkImageToImageFilter->Update();
	typedef itk::CastImageFilter<InputImageType, OutputImageType> CastImageFilter;
	typename CastImageFilter::Pointer castImageFilter =
		CastImageFilter::New();
	castImageFilter->SetInput(vtkImageToImageFilter->GetOutput());
	castImageFilter->Update();
	output->Graft(castImageFilter->GetOutput());
}
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type) \
template QVTKDATA_EXPORT bool Q::vtk::Image::_VTKImageToITKImage(itk::Image<type, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
QVTK_IMAGE_EXPORT_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE
#define QVTK_IMAGE_EXPORT_FUNCTION_TYPE(type, component) \
template QVTKDATA_EXPORT bool Q::vtk::Image::_VTKImageToITKImage(itk::Image<itk::Vector<type, component>, 3>* output, vtkImageData* input, const double orientation[3], const double position[3], const double scale[3], vtkMatrix4x4 *userMatrix);
QVTK_IMAGE_EXPORT_VECTOR_FUNCTION
#undef QVTK_IMAGE_EXPORT_FUNCTION_TYPE