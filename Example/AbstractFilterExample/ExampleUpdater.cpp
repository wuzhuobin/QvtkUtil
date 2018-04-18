/*!
* \file ExampleUpdater.cpp
* \date 2017/07/06 10:22
*
* \author 		Wong, Matthew Lun
* Occupation	Chinese University of Hong Kong,
* 				Department of Imaging and Interventional Radiology,
* 				M.Phil Student
* Contact: 	fromosia@gmail.com
*
* \brief This class is an example of how to implement an updater to update a renderer
*
*
* \note
*/

#include "ExampleUpdater.h"

#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRendererCollection.h"

void Updater::SetRenderWindow(vtkRenderWindow* ren)
{
	this->m_ren = ren;
}

void Updater::ConnectModifyingObject(QObject* obj)
{
	// #TODO: maybe this signal should be named more generally.
	connect(obj, SIGNAL(signalFilterModified()), this, SLOT(slotRender()));
}

void Updater::slotRender()
{
	this->m_ren->Render();
    this->m_ren->GetRenderers()->GetFirstRenderer()->ResetCameraClippingRange();
}