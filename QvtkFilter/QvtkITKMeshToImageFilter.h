/*!
 * \file QvtkAbstractITKMeshToImageFilter.h
 * \date 2017/07/28 15:20
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class wraps Mesh to Image filter as itk::ImageSource. 
 *        Note that there are restriction to this class. This itk filter
 *        wrapped with this class can only connect to 1 prefilter due the
 *        the lack of a convertion filter to convert mesh between ITK and
 *        VTK format.
 *
 * \note  This class is unstable
*/

#ifndef __Qvtk_ABSTRACT_MESH_TO_IMAGE_FILTER_H__
#define __Qvtk_ABSTRACT_MESH_TO_IMAGE_FILTER_H__

#include "QvtkAbstractITKFilter.h"


#include <itkMeshSource.h>
#include <itkImageSource.h>
#include <itkImageToVTKImageFilter.h>

#include <vtkPolyData.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPassThroughFilter.h>

template <typename TInputMeshType, typename TOutputImageType>
class /*ABSTRACTFILTER_EXPORT*/ QvtkAbstractMeshToImageFilter
	: public QvtkAbstractITKFilter
{
public:
	QvtkAbstractITKImageToImageFilter();
	~QvtkAbstractITKImageToImageFilter();

	typedef itk::ImageSource<TOutputImageType> FilterType;
    typedef itk::ImageToVTKImageFilter<TOutputImageType> ConnectorType;

	/**
	* void SetInputData
	*
	* @brief
	*
	* Convert input to TInputMeshType. Be warned that only triangles and
	* points are kepts.
	*
	* @param vtkPolyData * input
	* @return void
	*/
	virtual void SetInputData(vtkPolyData* input);


	/**
	* void SetInputData
	*
	* @brief
	*
	* ITK Method wrapped
	*
	* @param TInputMeshType::Pointer input
	* @return void
	*/
	virtual void SetInputData(typename TInputMeshType::Pointer input);

	/**
	* void SetInputConnection
	*
	* @brief
	*
	* Override SetInputConnection to convert input into ITK format first. Strictly
	* for vtk input algorithm.
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
	* Override GetOutputPort to convert output back to VTK format. Strictly for
	* vtk algorithm output.
	*
	* @param int port
	* @return vtkAlgorithmOutput*
	*/
	virtual typename vtkAlgorithmOutput* GetOutputPort(int port = 0);

	///**
	// * FilterType GetITKFilter
	// *
	// * @brief 
	// *
	// * Return the ITK filter 
	// *
	// * @return FilterType
	// */
	virtual typename FilterType* GetITKFilter();

	/**
	* void PreConnectFilter
	*
	* @brief
	*
	* Override this method for compatibility to QvtkPipeline
	*
	* @param QvtkAbstractFilter * preFilter
	* @param int prePort
	* @param int thisPort
	* @return void
	*/
	virtual void PreConnectFilter(QvtkAbstractFilter* preFilter, int prePort = 0, int thisPort = 0);


	/**
	* void Update
	*
	* @brief
	*
	* Update ITK then update vtkPassThroughFilter
	*
	* @return void
	*/
	virtual void Update();


	/**
	* TOutputImageType::Pointer GetITKOutputData
	*
	* @brief
	*
	* Get ITK output, return as TOutputImageType
	*
	* @return TOutputMeshType
	*/
	virtual typename TOutputImageType::Pointer GetITKOutputData();

	/**
	* vtkPolyData* GetVTKOutputData
	*
	* @brief
	*
	* Get VTK output as vtkImageData*
	*
	* @return vtkImageData**
	*/
	virtual vtkImageData* GetVTKOutputData();

protected:
	/**
	* void UpdateOutputPort
	*
	* @brief
	*
	* Update the port output. The port require update every time the ITKFilter changes.
	*
	* @return void
	*/
	void UpdateOutputPort();
    void UpdateInputPort(int thisPort = 0);

	virtual bool ErrorCheck();

	template<typename MeshType>
	static void ConvertVtkPolyDataToMesh(vtkPolyData* inpd, MeshType mesh);

    typename ConnectorType::Pointer outputConnector;

};



// WongQvtkAbstractITKMeshToImageFilterMatthew69c1bcf9-5679-4037-b343-13c7bb467869Lun  
//[ 7/28/2017-10:02:07 Author: Wong, Matthew Lun]

#include "QvtkAbstractITKMeshToImageFilter.hxx"

#endif