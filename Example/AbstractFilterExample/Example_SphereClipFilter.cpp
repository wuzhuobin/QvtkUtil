/*!
* \file Example_SphereClipFilter.cpp
* \date 2017/07/19 16:54
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

#include "Example_SphereClipFilter.h"

// me
#include "ui_Example_SphereClipFilter.h"

#include <QWidget>


ExampleSphereClipFilter::ExampleSphereClipFilter(QWidget* parent)
    : SCBAbstractPolyDataFilter(parent)
{
    /* Call this macro at the very beging of this constructor */
    SCB_FILTER_CONSTRUCT_MACRO(vtkClipPolyData);

    this->m_implicitSphere = vtkSphere::New();
    this->m_implicitSphere->SetRadius(0);

    this->GetFilter()->SetClipFunction(this->m_implicitSphere);

    this->m_filterNameString = QString(typeid(*this).name()).split(' ')[1];
}

ExampleSphereClipFilter::~ExampleSphereClipFilter()
{
    this->m_vtkfilter->Delete();
    this->m_implicitSphere->Delete();
}


void ExampleSphereClipFilter::LoadUI()
{
        /* Setup UI */
        m_ui = new Ui_SphereClipFilterUI;
        m_ui->setupUi(this);
}

int ExampleSphereClipFilter::ConnectUI()
{
    connect(m_ui->checkBoxEnabled, SIGNAL(stateChanged(int)),
        this, SLOT(slotFilterSettingsUpdate()));
    connect(m_ui->checkBoxInsideOut, SIGNAL(stateChanged(int)),
        this, SLOT(slotFilterSettingsUpdate()));


    connect(m_ui->doubleSpinBoxRadius, SIGNAL(valueChanged(double)),
        this, SLOT(slotFilterSettingsUpdate()));
    connect(m_ui->doubleSpinBoxPosX, SIGNAL(valueChanged(double)),
        this, SLOT(slotFilterSettingsUpdate()));
    connect(m_ui->doubleSpinBoxPosY, SIGNAL(valueChanged(double)),
        this, SLOT(slotFilterSettingsUpdate()));
    connect(m_ui->doubleSpinBoxPosZ, SIGNAL(valueChanged(double)),
        this, SLOT(slotFilterSettingsUpdate()));

    return 0;
}


vtkClipPolyData * ExampleSphereClipFilter::GetFilter()
{
    return vtkClipPolyData::SafeDownCast(this->m_vtkfilter);
}

void ExampleSphereClipFilter::slotFilterSettingsUpdate()
{
    if (this->sender() == this->m_ui->doubleSpinBoxPosX ||
        this->sender() == this->m_ui->doubleSpinBoxPosY ||
        this->sender() == this->m_ui->doubleSpinBoxPosZ)
    {
        this->UpdateSpherePositionWithUI();
    }
    
    if (this->sender() == this->m_ui->doubleSpinBoxRadius)
        this->m_implicitSphere->SetRadius(
            this->m_ui->doubleSpinBoxRadius->value());

    if (this->sender() == this->m_ui->checkBoxInsideOut)
        this->GetFilter()->SetInsideOut(this->m_ui->checkBoxInsideOut->isChecked());

    if (this->sender() == this->m_ui->checkBoxEnabled)
    {
        if (!this->m_ui->checkBoxEnabled->isChecked())
        {
            this->m_implicitSphere->SetRadius(0.);
            this->m_ui->doubleSpinBoxRadius->setDisabled(true);
            this->m_ui->doubleSpinBoxPosX->setDisabled(true);
            this->m_ui->doubleSpinBoxPosY->setDisabled(true);
            this->m_ui->doubleSpinBoxPosZ->setDisabled(true);

        }
        else 
        {
            this->m_implicitSphere->SetRadius(this->m_ui->doubleSpinBoxRadius->value());
            this->m_ui->doubleSpinBoxRadius->setDisabled(false);
            this->m_ui->doubleSpinBoxPosX->setDisabled(false);
            this->m_ui->doubleSpinBoxPosY->setDisabled(false);
            this->m_ui->doubleSpinBoxPosZ->setDisabled(false);
        }
    }

    this->Update();
}

void ExampleSphereClipFilter::UpdateSpherePositionWithUI()
{
    double pos[3] = {
        this->m_ui->doubleSpinBoxPosX->value(),
        this->m_ui->doubleSpinBoxPosY->value(),
        this->m_ui->doubleSpinBoxPosZ->value()
    };

    this->m_implicitSphere->SetCenter(pos);
}

