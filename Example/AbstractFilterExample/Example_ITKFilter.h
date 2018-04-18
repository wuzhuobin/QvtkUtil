#ifndef __SCB_EXAMPLE_ITK_FILTER_H__
#define __SCB_EXAMPLE_ITK_FILTER_H__

#include "SCBAbstractITKImageToImageFilter.h"


#include <itkImage.h>
#include <itkImageToImageFilter.h>
#include <itkThresholdImageFilter.h>

#include <QRadioButton>

#include "ui_Example_ITKFilter.h"

class Ui_ExampleITKFilter;
namespace Ui { class SCBExampleITKFilter; };


template<typename ImageType>
class SCBExampleITKFilter :
    public SCBAbstractITKImageToImageFilter<ImageType,ImageType>
{
    //Q_OBJECT
public:
    SCBExampleITKFilter();
    ~SCBExampleITKFilter();

    typedef itk::ThresholdImageFilter<ImageType> FilterType;

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

    virtual FilterType* GetITKFilter();

    /**
     * void slotsFilterSettingsUpdate
     *
     * @brief 
     *
     * This method is the only way UI can interact with this class. Use this as a general slot
     *
     * @return void
     */
    virtual void slotsFilterSettingsUpdate();
	
protected:
    /**
     * void slotsThresholdModeCheck
     *
     * @brief 
     *
     * Protected method for controlling the radio buttons
     *
     * @param bool  
     * @return void
     */
    void slotsThresholdModeCheck(bool);

    Ui_ExampleITKFilter* m_ui;

};

#include "Example_ITKFilter.hpp"

#endif