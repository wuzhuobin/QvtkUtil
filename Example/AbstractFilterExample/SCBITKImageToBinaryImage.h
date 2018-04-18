/*!
 * \file SCB_ITKImageToBinaryImage.h
 * \date 2017/07/28 19:27
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief  This class demonstrates the use of SCBAbstractITKGeneralFilter. It should be
 *         noted that more suitable to use SCBAbstractITKImageToImageFilter to write 
 *         this filter.
 *
 * TODO: long description
 *
 * \note
*/

#ifndef __SCB_ITK_IMAGE_TO_BINARY_IMAGE_H__
#define __SCB_ITK_IMAGE_TO_BINARY_IMAGE_H__

#include "SCBAbstractITKGeneralFilter.h"


#include "ui_SCBITKImageToBinaryImage.h"

#include <itkImage.h>
#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>

class Ui_BinaryThresholdFilter;
namespace Ui { class BinaryThresholdFilter; };

template <typename TInputScalarType>
class  SCBITKImageToBinaryImage :
    public SCBAbstractITKGeneralFilter<
        itk::Image<TInputScalarType, 3>, 
        itk::Image<unsigned char, 3>, 
        itk::BinaryThresholdImageFilter<
			itk::Image<TInputScalarType, 3>, 
			itk::Image<unsigned char, 3>
		>
    >
{
public:
    SCBITKImageToBinaryImage();
    ~SCBITKImageToBinaryImage();
    
    typedef itk::BinaryThresholdImageFilter<
		itk::Image<TInputScalarType, 3>, 
		itk::Image<uchar, 3>
	> FilterType;
	typedef itk::Image<unsigned char, 3> OutImageType;
	typedef itk::Image<TInputScalarType, 3> InImageType;
	typedef itk::ImageToVTKImageFilter<OutImageType> OutConnectorType;
	typedef itk::VTKImageToImageFilter<InImageType> InConnectorType;

    /**
    * @brief ConnectUI
    *
    * Signals from parameters UI should be connected to the corresponding
    * slots in this function
    *
    * @return int
    */
    virtual int ConnectUI();

    /**
    * @brief LoadUI
    *
    * m_widget should be prepared after this LoadUI() function.
    * This function is a pure virtual function in base class.
    *
    * @return void
    */
    virtual void LoadUI();


    /**
     * typename FilterType* GetITKFilter
     *
     * @brief 
     *
     * Inherit this to cast filter to suitable type
     *
     * @return typename FilterType*
     */
    virtual typename FilterType* GetITKFilter();

	/**
	 * void SetInputData
	 *
	 * @brief 
	 *
	 * Setting input manually
	 *
	 * @param vtkImageData * im 
	 * @return void
	 */
	virtual void SetInputData(vtkImageData* im);

	/**
	 * void slotsFilterSettingsUpdate
	 *
	 * @brief 
	 *
	 * ITK slot to control UI interaction
	 *
	 * @return void
	 */
	virtual void slotsFilterSettingsUpdate();


protected:
	typename InConnectorType::Pointer m_inConnector;
	typename OutConnectorType::Pointer m_outConnector;

	Ui_BinaryThresholdFilter* m_ui;

    /**
     * typename itk<TInputScalarType, 3>::Pointer ConvertInputType
     *
     * @brief 
     *
     * Required to override
     *
     * @param vtkAlgorithmOutput * input 
     * @return typename itk<TInputScalarType, 3>::Pointer
     */
    virtual typename itk::Image<TInputScalarType, 3>::Pointer ConvertInputType(vtkAlgorithmOutput* input) override;

    /**
     * typename vtkDataObject* ConvertOutputToVTKObject
     *
     * @brief 
     *
     * Override to return output as vtk object data
     *
     * @param typename OutImageType::Pointer outputPtr 
     * @return typename vtkDataObject*
     */
    virtual typename vtkDataObject* ConvertOutputToVTKObject(typename OutImageType::Pointer outputPtr) override;

    /**
     * typename vtkAlgorithmOutput* ConvertOutputToVTKPort
     *
     * @brief 
     *
     * Override to convert output pointer to a port
     *
     * @param typename OutImageType::Pointer outputPtr 
     * @return typename vtkAlgorithmOutput*
     */
    virtual typename vtkAlgorithmOutput* ConvertOutputToVTKPort(typename OutImageType::Pointer outputPtr) override;

};

#include "SCBITKImageToBinaryImage.hpp"

#endif
