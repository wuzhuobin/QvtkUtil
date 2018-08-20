/*!
* \file Example_PDTransformFilter.h
* \date 2017/07/19 17:31
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

#include "Example_PDTransformFilter.h"

// me
#include "ui_Example_PDTransformFilter.h"

#include <QDoubleSpinBox>
#include <QWidget>

#include <vtkMatrix4x4.h>
#include <vtkTransform.h>

ExamplePDTransfromFilter::ExamplePDTransfromFilter(QWidget* parent)
    : SCBAbstractPolyDataFilter(parent)
{
    /* Call this macro at the very beging of this constructor */
    SCB_FILTER_CONSTRUCT_MACRO(vtkTransformPolyDataFilter);

    this->m_transform = vtkTransform::New();

    this->GetFilter()->SetTransform(this->m_transform);

    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_11);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_12);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_13);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_14);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_21);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_22);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_23);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_24);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_31);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_32);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_33);
    this->m_matrixSpinBoxList.append(this->m_ui->doubleSpinBox_34);    

    this->m_filterNameString = QString(typeid(*this).name()).split(' ')[1];
}

ExamplePDTransfromFilter::~ExamplePDTransfromFilter()
{
    this->m_vtkfilter->Delete();
}


void ExamplePDTransfromFilter::LoadUI()
{
        /* Setup UI */
        m_ui = new Ui_PDTransformFilterUI;
        m_ui->setupUi(this);
}

int ExamplePDTransfromFilter::ConnectUI()
{
    try {
        connect(m_ui->hSliderRotateX, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));
        connect(m_ui->hSliderRotateY, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));
        connect(m_ui->hSliderRotateZ, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));

        connect(m_ui->doubleSpinBox_11, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_12, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_13, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_14, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_21, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_22, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_23, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_24, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_31, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_32, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_33, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
        connect(m_ui->doubleSpinBox_34, SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));

        return 0;
    }
    catch (...)
    {
        return 1;
    }
}


vtkTransformPolyDataFilter * ExamplePDTransfromFilter::GetFilter()
{
    return vtkTransformPolyDataFilter::SafeDownCast(this->m_vtkfilter);
}

void ExamplePDTransfromFilter::slotFilterSettingsUpdate()
{

    if (this->sender() == m_ui->doubleSpinBox_11 ||
        this->sender() == m_ui->doubleSpinBox_12 ||
        this->sender() == m_ui->doubleSpinBox_13 ||
        this->sender() == m_ui->doubleSpinBox_14 ||
        this->sender() == m_ui->doubleSpinBox_21 ||
        this->sender() == m_ui->doubleSpinBox_22 ||
        this->sender() == m_ui->doubleSpinBox_23 ||
        this->sender() == m_ui->doubleSpinBox_24 ||
        this->sender() == m_ui->doubleSpinBox_31 ||
        this->sender() == m_ui->doubleSpinBox_32 ||
        this->sender() == m_ui->doubleSpinBox_33 ||
        this->sender() == m_ui->doubleSpinBox_34 
        )
    { 
        this->MatrixUI2VTK();
    }

    this->Update();
}



void ExamplePDTransfromFilter::MatrixUI2VTK()
{
    /* Prevent QT signal triggers loop */
    disconnect(m_ui->hSliderRotateX, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));
    disconnect(m_ui->hSliderRotateY, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));
    disconnect(m_ui->hSliderRotateZ, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));

    double *out = new double[16];

    for (int i = 0; i < 12;i++)
    {
        out[i] = this->m_matrixSpinBoxList[i]->value();
    }

    out[12] = 0;
    out[13] = 0;
    out[14] = 0;
    out[15] = 1;
    
    this->m_transform->SetMatrix(out);


    /* Update sliders too */
    double* ori = this->m_transform->GetOrientation(); // in degrees
    this->m_ui->hSliderRotateX->setValue(int(ori[0] * 10));
    this->m_ui->hSliderRotateY->setValue(int(ori[1] * 10));
    this->m_ui->hSliderRotateZ->setValue(int(ori[2] * 10));

    connect(m_ui->hSliderRotateX, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));
    connect(m_ui->hSliderRotateY, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));
    connect(m_ui->hSliderRotateZ, SIGNAL(valueChanged(int)), this, SLOT(slotSliderRotationsHandle()));
}

void ExamplePDTransfromFilter::MatrixVTK2UI()
{
    /* Prevent QT signal triggers loop */
    for (int i = 0; i < this->m_matrixSpinBoxList.size();i++)
        disconnect(this->m_matrixSpinBoxList[i], SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));


    vtkMatrix4x4* mat 
        = vtkTransform::SafeDownCast(this->GetFilter()->GetTransform())->GetMatrix();

    for (int i = 0; i < 3;i++)
    {
        for (int j = 0; j < 4;j++)
        {
            this->m_matrixSpinBoxList[i * 4 + j]->setValue(mat->GetElement(i, j));
        }
    }

    for (int i = 0; i < this->m_matrixSpinBoxList.size(); i++)
        connect(this->m_matrixSpinBoxList[i], SIGNAL(valueChanged(double)), this, SLOT(slotFilterSettingsUpdate()));
}

void ExamplePDTransfromFilter::slotSliderRotationsHandle()
{
    /* Rotate the tranform at (0,0,0) */
    double trans[3];
	trans[0] = this->m_transform->GetMatrix()->GetElement(0, 3);
	trans[1] = this->m_transform->GetMatrix()->GetElement(1, 3);
	trans[2] = this->m_transform->GetMatrix()->GetElement(2, 3);
	this->m_transform->Identity();
	this->m_transform->Translate(trans[0], trans[1], trans[2]);
    this->m_transform->RotateX(this->m_ui->hSliderRotateX->value() / 10.);
    this->m_transform->RotateY(this->m_ui->hSliderRotateY->value() / 10.);
    this->m_transform->RotateZ(this->m_ui->hSliderRotateZ->value() / 10.);

    this->m_ui->doubleSpinBoxRotateX->setValue(this->m_ui->hSliderRotateX->value() / 10.);
    this->m_ui->doubleSpinBoxRotateY->setValue(this->m_ui->hSliderRotateY->value() / 10.);
    this->m_ui->doubleSpinBoxRotateZ->setValue(this->m_ui->hSliderRotateZ->value() / 10.);

	//this->m_transform->GetMatrix()->Print(cout);

    /* Update UI */
    this->MatrixVTK2UI();

    /* Update filter and emit update signal */
    this->Update();
}
