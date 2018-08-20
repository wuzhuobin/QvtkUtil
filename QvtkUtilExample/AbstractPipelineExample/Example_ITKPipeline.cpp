#include "Example_ITKPipeline.h"
#include "SCBITKTypeCastFilter.h"

#include "Example_ITKFilter.h"

#include <itkImage.h>

SCBExampleITKPipeline::SCBExampleITKPipeline(QWidget* parent)
	: SCBAbstractPipeline(parent)
{
	this->SetVBoxLayout();

	SCBITKTypeCastFilter<ImageType, ImageType>* f0 = new SCBITKTypeCastFilter<ImageType, ImageType>;
	SCBExampleITKFilter<ImageType>* f1 = new SCBExampleITKFilter<ImageType>;

	this->AddFilter(f0, false);
	this->AddFilter(f1);
	this->ConnectAllFilters();
	this->BuildUI();
}

SCBExampleITKPipeline::~SCBExampleITKPipeline()
{

}

void SCBExampleITKPipeline::SetInputData(vtkImageData* im)
{
	/* Choose a type caster according to input */
	int scalarType = im->GetScalarType();
	SCBAbstractFilter* originalFilter = this->m_filterList[0];

	if (scalarType == VTK_FLOAT)
	{
		this->m_filterList[0] = new SCBITKTypeCastFilter<itk::Image<float, 3>, ImageType>;
		static_cast<SCBITKTypeCastFilter<itk::Image<float, 3>, ImageType>*>(this->m_filterList[0])->SetInputData(im);
	}
	else if (scalarType == VTK_DOUBLE)
	{
		this->m_filterList[0] = new SCBITKTypeCastFilter<itk::Image<double, 3>, ImageType>;
		static_cast<SCBITKTypeCastFilter<itk::Image<double, 3>, ImageType>*>(this->m_filterList[0])->SetInputData(im);
	}
	else if (scalarType == VTK_SHORT)
	{
		this->m_filterList[0] = new SCBITKTypeCastFilter<itk::Image<short, 3>, ImageType>;
		static_cast<SCBITKTypeCastFilter<itk::Image<short, 3>, ImageType>*>(this->m_filterList[0])->SetInputData(im);
	}
	else if (scalarType == VTK_INT)
	{
		this->m_filterList[0] = new SCBITKTypeCastFilter<itk::Image<int, 3>, ImageType>;
		static_cast<SCBITKTypeCastFilter<itk::Image<int, 3>, ImageType>*>(this->m_filterList[0])->SetInputData(im);
	}
	else if (scalarType == VTK_UNSIGNED_CHAR)
	{
		this->m_filterList[0] = new SCBITKTypeCastFilter<itk::Image<unsigned char, 3>, ImageType>;
		static_cast<SCBITKTypeCastFilter<itk::Image<unsigned char, 3>, ImageType>*>(this->m_filterList[0])->SetInputData(im);
	}
	else if (scalarType == VTK_UNSIGNED_INT)
	{
		this->m_filterList[0] = new SCBITKTypeCastFilter<itk::Image<unsigned int, 3>, ImageType>;
		static_cast<SCBITKTypeCastFilter<itk::Image<unsigned int, 3>, ImageType>*>(this->m_filterList[0])->SetInputData(im);
	}
	else if (scalarType == VTK_UNSIGNED_LONG)
	{
		this->m_filterList[0] = new SCBITKTypeCastFilter<itk::Image<unsigned long, 3>, ImageType>;
		static_cast<SCBITKTypeCastFilter<itk::Image<unsigned long, 3>, ImageType>*>(this->m_filterList[0])->SetInputData(im);
	}
	else if (scalarType == VTK_UNSIGNED_SHORT)
	{
		this->m_filterList[0] = new SCBITKTypeCastFilter<itk::Image<unsigned short, 3>, ImageType>;
		static_cast<SCBITKTypeCastFilter<itk::Image<unsigned short, 3>, ImageType>*>(this->m_filterList[0])->SetInputData(im);
	}
	this->ConnectAllFilters();

	originalFilter->deleteLater();
}

QVBoxLayout* SCBExampleITKPipeline::GetLayout()
{
    return (QVBoxLayout*)this->layout();
}

