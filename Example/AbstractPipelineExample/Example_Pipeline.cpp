/*!
* \file Example_Pipeline.cpp
* \date 2017/07/20 19:20
*
* \author 		Wong, Matthew Lun
* Occupation	Chinese University of Hong Kong,
* 				Department of Imaging and Interventional Radiology,
* 				M.Phil Student
* Contact: 	fromosia@gmail.com
*
* \brief This class is demonstrate how to use SCBAbstractPipeline to create a usable pipeline.
*
*
* \note
*/

#include "Example_Pipeline.h"

#include <QStackedLayout>

#include "Example_ConnectivityFilter.h"
#include "Example_PDTransformFilter.h"
#include "Example_SphereClipFilter.h"

ExamplePipeline::ExamplePipeline(QWidget* parent)
    : SCBAbstractPipeline(parent)
{
    this->SetTabWidgetLayout();

    ExamplePDTransfromFilter*  f1 = new ExamplePDTransfromFilter(this);
    ExampleSphereClipFilter*   f2 = new ExampleSphereClipFilter(this);
    ExampleConnectivityFilter* f3 = new ExampleConnectivityFilter(this);

    /* Add filter in sequential order */
    this->AddFilter(f1);
    this->AddFilter(f2);
    this->AddFilter(f3);
    this->ConnectAllFilters();
    this->BuildUI();
}


ExamplePipeline::~ExamplePipeline()
{

}

QVBoxLayout* ExamplePipeline::GetLayout()
{
    return (QVBoxLayout*)this->layout();
}

