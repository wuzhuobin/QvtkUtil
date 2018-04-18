/*!
 * \file QvtkAbstractITKImageToMeshFilter.h
 * \date 2017/07/27 19:17
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class we wrap image to mesh filter of ITK.
 *
 * \note
*/

#ifndef __Qvtk_ABSTRACT_ITK_IMAGE_TO_MESH_FILTER_H__
#define __Qvtk_ABSTRACT_ITK_IMAGE_TO_MESH_FILTER_H__

#include "QvtkAbstractITKFilter.h"


#include <itkImage.h>
#include <itkImageToImageFilter.h>
#include <itkImageToMeshFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <itkVTKImageToImageFilter.h>


#include <vtkPolyData.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPassThroughFilter.h>

template <typename TInputImageType, typename TOutputMeshType>
class /*ABSTRACTFILTER_EXPORT*/ QvtkAbstractImageToMeshFilter
    : public QvtkAbstractITKFilter
{
public:
    QvtkAbstractITKImageToImageFilter();
    ~QvtkAbstractITKImageToImageFilter();

    typedef itk::ImageToMeshFilter<TInputImageType, TOutputMeshType> FilterType;
    typedef itk::VTKImageToImageFilter<TInputImageType> InputConnectorType;


    /**
    * void SetInputData
    *
    * @brief
    *
    * ITK Method wrapped
    *
    * @param vtkImageData * input
    * @return void
    */
    virtual void SetInputData(vtkImageData* input);

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
    virtual vtkAlgorithmOutput* GetOutputPort(int port = 0);

    ///**
    // * FilterType GetITKFilter
    // *
    // * @brief 
    // *
    // * Return the ITK filter 
    // *
    // * @return FilterType
    // */
    virtual FilterType* GetITKFilter();

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
     * TOutputMeshType GetITKOutputData
     *
     * @brief 
     *
     * Get ITK output, return as TOutputMeshType
     *
     * @return TOutputMeshType
     */
    virtual typename TOutputMeshType::Pointer GetITKOutputData();

    /**
     * vtkPolyData* GetVTKOutputData
     *
     * @brief 
     *
     * Get VTK output as vtkPolyData*
     *
     * @return vtkPolyData*
     */
    virtual vtkPolyData* GetVTKOutputData();

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

    static void ConvertMeshTovtkPolyData(typename TOutputMeshType::Pointer mesh, 
        vtkPolyData* outpd);

    typename InputConnectorType::Pointer inputConnector;
};


#include "QvtkAbstractITKImageToMeshFilter.hxx"

#endif