/*!
* \file Example_ConnectivityFilter.cpp
* \date 2017/07/19 16:30
*
* \author 		Wong, Matthew Lun
* Occupation	Chinese University of Hong Kong,
* 				Department of Imaging and Interventional Radiology,
* 				M.Phil Student
* Contact: 	fromosia@gmail.com
*
* \brief   An Example implementation of polydata filter used to demonstrate both
*          how to use SCBAbstractFilter class and also SCBPipeline.
*
*
* \note
*/

#include "Example_ConnectivityFilter.h"

// me
#include "ui_Example_ConnectivityFilter.h"

#include <QWidget>


ExampleConnectivityFilter::ExampleConnectivityFilter(QWidget* parent)
    : SCBAbstractPolyDataFilter(parent)
{
    /* Call this macro at the very beging of this constructor */
    SCB_FILTER_CONSTRUCT_MACRO(vtkPolyDataConnectivityFilter);

    this->m_filterNameString = QString(typeid(*this).name()).split(' ')[1];
}

ExampleConnectivityFilter::~ExampleConnectivityFilter()
{
    this->m_vtkfilter->Delete();
}


void ExampleConnectivityFilter::LoadUI()
{
        /* Setup UI */
        m_ui = new Ui_ConnectivityFilterUI;
        m_ui->setupUi(this);
}

int ExampleConnectivityFilter::ConnectUI()
{
    connect(m_ui->checkBoxColorByRegionID, SIGNAL(stateChanged(int)),
        this, SLOT(slotFilterSettingsUpdate()));

    connect(m_ui->checkBoxExtractAllRegions, SIGNAL(stateChanged(int)),
        this, SLOT(slotFilterSettingsUpdate()));

    connect(m_ui->spinBoxExtractRegionID, SIGNAL(valueChanged(int)),
        this, SLOT(slotFilterSettingsUpdate()));

    return 0;
}


vtkPolyDataConnectivityFilter * ExampleConnectivityFilter::GetFilter()
{
    return vtkPolyDataConnectivityFilter::SafeDownCast(this->m_vtkfilter);
}

void ExampleConnectivityFilter::slotFilterSettingsUpdate()
{
    if (this->sender() == this->m_ui->checkBoxColorByRegionID)
        this->GetFilter()->SetColorRegions(this->m_ui->checkBoxColorByRegionID->isChecked());
    
    if (this->sender() == this->m_ui->checkBoxExtractAllRegions)
    {
        if (this->m_ui->checkBoxExtractAllRegions->isChecked())
        {
            this->GetFilter()->SetExtractionModeToAllRegions();
            this->GetFilter()->Update();
            this->m_ui->spinBoxExtractRegionID
                ->setMaximum(this->GetFilter()->GetNumberOfExtractedRegions() - 1);

            this->m_ui->spinBoxExtractRegionID->setDisabled(true);
        }
        else
        {
            this->GetFilter()->SetExtractionModeToSpecifiedRegions();
            this->GetFilter()->InitializeSpecifiedRegionList();
            this->GetFilter()->AddSpecifiedRegion(this->m_ui->spinBoxExtractRegionID->value());

            this->m_ui->spinBoxExtractRegionID->setDisabled(false);
        }
    }
    
    if (this->sender() == this->m_ui->spinBoxExtractRegionID)
    {
        this->GetFilter()->InitializeSpecifiedRegionList();
        this->GetFilter()->AddSpecifiedRegion(this->m_ui->spinBoxExtractRegionID->value());
    }


    this->Update();
}
