#ifndef __SCB_EXAMPLE_ITK_PIPELINE_H__
#define __SCB_EXAMPLE_ITK_PIPELINE_H__

#include "SCBAbstractPipeline.h"


#include "Example_ITKFilter.h"

#include <vtkImageData.h>

class SCBExampleITKPipeline : public SCBAbstractPipeline
{
public:
	SCBExampleITKPipeline(QWidget* parent = nullptr);
	~SCBExampleITKPipeline();

	/* The type is pipeline specific, change this if you wish to use other image scalar type */
	typedef itk::Image<float, 3> ImageType;

	/**
	 * void SetInputData
	 *
	 * @brief 
	 *
	 * Set input to the pipeline
	 *
	 * @param vtkImageData * im 
	 * @return void
	 */
	void SetInputData(vtkImageData* im);

	/**
	 * QT_NAMESPACE::QVBoxLayout* GetLayout
	 *
	 * @brief 
	 *
	 * Required to implement
	 *
	 * @return QT_NAMESPACE::QVBoxLayout*
	 */
	QVBoxLayout* GetLayout();

protected:
};


#endif