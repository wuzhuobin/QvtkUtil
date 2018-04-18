/*!
 * \file QvtkAbstractITKImageFilter.h
 * \date 2017/07/25 14:42
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong,
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class wraps an itk pipeline into the structure. A vtk input is expected.
 *
 * TODO: long description
 *
 * \note
*/

#ifndef __Qvtk_ABSTRACT_ITK_IMAGE_TO_IMAGE_FILTER_H__
#define __Qvtk_ABSTRACT_ITK_IMAGE_TO_IMAGE_FILTER_H__

#include "QvtkAbstractITKFilter.h"


#include <itkImage.h>
#include <itkImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <itkVTKImageToImageFilter.h>


#include <vtkAlgorithmOutput.h>
#include <vtkPassThroughFilter.h>

template <typename TInputImageType, typename TOutputImageType>
class /*ABSTRACTFILTER_EXPORT*/ QvtkAbstractITKImageToImageFilter
    : public QvtkAbstractITKFilter
{
public:
    QvtkAbstractITKImageToImageFilter();
    ~QvtkAbstractITKImageToImageFilter();

    typedef itk::ImageToImageFilter<TInputImageType, TOutputImageType> FilterType;
    typedef itk::ImageToVTKImageFilter<TOutputImageType> OutputConnectorType;
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

    /**
     * TOutputImageType GetITKOutputData
     *
     * @brief 
     *
     * Get ITK output image return template type TOutputImageType
     *
     * @return TOutputImageType
     */
    virtual typename TOutputImageType::Pointer GetITKOutputData();


    /**
     * vtkImageData* GetVTKOutputData
     *
     * @brief 
     *
     * Get VTK output image as vtkImageData*
     *
     * @return vtkImageData*
     */
    virtual vtkImageData* GetVTKOutputData();

    /**
     * FilterType GetITKFilter
     *
     * @brief 
     *
     * Return the ITK filter 
     *
     * @return FilterType
     */
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


protected:
    virtual bool ErrorCheck();

    typename OutputConnectorType::Pointer outputConnector;
    typename InputConnectorType::Pointer inputConnector;
};

// WongQvtkAbstractITKImageToImageFilterMatthewae56809c-1299-48e9-b466-ecfca37fdb3cLun  
//[ 7/27/2017-19:31:07 Author: Wong, Matthew Lun]

#include "QvtkAbstractITKImageToImageFilter.hxx"

#endif