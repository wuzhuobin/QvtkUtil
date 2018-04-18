/*!
* \file QvtkAbstractITKGeneralFilter.h
* \date 2017/07/28 18:15
*
* \author 		Wong, Matthew Lun
* Occupation	Chinese University of Hong Kong,
* 				Department of Imaging and Interventional Radiology,
* 				M.Phil Student
* Contact: 	fromosia@gmail.com
*
* \brief   This class will wrap a black box itk filter. This class should be used when
*          all other ITK abstract classes are not suitable.
*
* TODO: The following function should be implemented:
*       1. ConvertInputType
*       2. ConvertOutputTypeToVTKObject
*       3. ConvertOutputTypeToVTKPort
*       4. GetITKFilter()
*
* \note
*/

#ifndef __Qvtk_ABSTRACT_ITK_GENERAL_FILTER_H__
#define __Qvtk_ABSTRACT_ITK_GENERAL_FILTER_H__

#include "QvtkAbstractITKFilter.h"

template<typename TInputType, typename TOutputType, typename TOutputFilterType>
class /*ABSTRACTFILTER_EXPORT*/ QvtkAbstractITKGeneralFilter : public QvtkAbstractITKFilter
{
public:
	QvtkAbstractITKGeneralFilter();
	~QvtkAbstractITKGeneralFilter();


	/**
	* void SetInputConnection
	*
	* @brief
	*
	* Wrapped method of vtkAlgorithm. Inherit this method if you have more then
	* one ITK filter wrapped by this pipeline.
	*
	* @param vtkAlgorithmOutput * input
	* @param int port
	* @return void
	*/
	virtual void SetInputConnection(vtkAlgorithmOutput* input, int port = 0);

	/**
	* vtkAlgorithmOutput* GetOutputPort
	*
	* @brief
	*
	* Wrapped method of vtkAlgorithm
	*
	* @param int port
	* @return vtkAlgorithmOutput*
	*/
	virtual vtkAlgorithmOutput* GetOutputPort(int port = 0);


	/**
	* vtkDataObject* GetVTKOutputData
	*
	* @brief
	*
	* Return vtkDataObject output
	*
	* @param int port
	* @return vtkDataObject*
	*/
	virtual vtkDataObject* GetVTKOutputData(int port = 0);

	/**
	* void PreConnectFilter
	*
	* @brief
	*
	* Required by QvtkAbstractPipeline subclasses
	*
	* @param QvtkAbstractFilter * preFilter
	* @param int prePort
	* @param int thisPort
	* @return void
	*/
	virtual void PreConnectFilter(QvtkAbstractFilter* preFilter, int prePort = 0, int thisPort = 0);

	/**
	* TOutputFilterType* GetITKFilter
	*
	* @brief
	*
	* Implement this function in subclass. This function should return the
	* downcasted version of itkfilter, which is the last filter in the black box.
	*
	* @return TOutputFilterType*
	*/
	virtual TOutputFilterType* GetITKFilter() = 0;

	/**
	* void Update
	*
	* @brief
	*
	* Update the pipeline in this filter;
	* Rewrite this function in subclass if you have more than one filters
	*
	* @return void
	*/
	virtual void Update();

	/**
	* void UpdateOutputPort
	*
	* @brief
	*
	* Update the output conversion. If you have more than one filters in the
	* black box you should inherit and rewrite this function
	*
	* @return void
	*/
	virtual void UpdateOutputPort();

	/**
	* void UpdateInputPort
	*
	* @brief
	*
	* Update the input conversion. If you have more than one filters in the
	* black box you should inherit and rewrite this function
	*
	* @return void
	*/
	virtual void UpdateInputPort();

protected:

	/**
	* TInputType ConvertInputType
	*
	* @brief
	*
	* The method is pure virtual and should be implemented by child classes.
	* Return the itk input type of the filter.
	*
	* @param vtkAlgorithmOutput * input
	* @return TInputType
	*/
	virtual typename TInputType::Pointer ConvertInputType(vtkAlgorithmOutput* input) = 0;

	/**
	* vtkDataObject* ConvertOutputToVTKObject
	*
	* @brief
	*
	* The method is pure virtual and should be implemented by child classes.
	* Return the vtkDataObject* from itk output type.
	*
	* @param TOutputType::Pointer
	* @return vtkDataObject*
	*/
	virtual typename vtkDataObject*      ConvertOutputToVTKObject(typename TOutputType::Pointer) = 0;

	/**
	* vtkAlgorithmOutput* ConvertOutputToVTKPort
	*
	* @brief
	*
	* The method is pure virtual and should be implemented by child classes.
	* Return the port, preferrable from this->GetFilter() where GetFilter() is
	* defaulted to be vtkPassthroughFilter.
	*
	* @param TOutputType::Pointer
	* @return vtkAlgorithmOutput*
	*/
	virtual typename vtkAlgorithmOutput* ConvertOutputToVTKPort(typename TOutputType::Pointer) = 0;


};

#include "QvtkAbstractITKGeneralFilter.hxx"

#endif