#include "Test_InteractorStyle.h"

#include <vtkCamera.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMath.h>

#include <QDebug>

vtkStandardNewMacro(Test_InteractorStyle);

void Test_InteractorStyle::OnKeyPress()
{
    std::string key = this->Interactor->GetKeySym();

    if (key == "Up")
        this->CursorMoveUp();
    if (key == "Down")
        this->CursorMoveDown();
    if (key == "Left")
        this->CursorMoveLeft();
    if (key == "Right")
        this->CursorMoveRight();

	/* SetCursorPosition already render all viewers */
    //SCBAbstractViewer::RenderViewersByGroup(this->m_thisviewer->GetViewerGroup());
}

void Test_InteractorStyle::SetInteractor(vtkRenderWindowInteractor *interactor)
{
    Superclass::SetInteractor(interactor);
    this->FindViewer();
}

void Test_InteractorStyle::SetViewer(SCBAbstractViewer* viewer)
{
    this->m_thisviewer = viewer;
}

Test_InteractorStyle::Test_InteractorStyle()
{
    
}

void Test_InteractorStyle::FindViewer()
{
	this->m_thisviewer = SCBAbstractViewer::GetViewerOfInteractor(this);
}

bool Test_InteractorStyle::ErrorCheck()
{
    if (!this->m_thisviewer)
    {
        throw std::logic_error("Viewer not found");
        return 1;
    }
    return 0;
}

void Test_InteractorStyle::CursorMoveUp()
{
    if (this->ErrorCheck())
        return;

    double* viewup = this->m_thisviewer->GetActiveCamera()->GetViewUp();
    double  newPos[3];
    vtkMath::Add(viewup, this->m_thisviewer->GetCursorPosition(), newPos);
    this->m_thisviewer->SetCursorPosition(newPos);
}

void Test_InteractorStyle::CursorMoveDown()
{
    if (this->ErrorCheck())
        return;

    double* viewup = this->m_thisviewer->GetActiveCamera()->GetViewUp();
    double  newPos[3];
    vtkMath::Subtract(this->m_thisviewer->GetCursorPosition(), viewup, newPos);
    this->m_thisviewer->SetCursorPosition(newPos);
}


void Test_InteractorStyle::CursorMoveRight()
{
    if (this->ErrorCheck())
        return;

    double* viewup = this->m_thisviewer->GetActiveCamera()->GetViewUp();
    double* camangle = this->m_thisviewer->GetActiveCamera()->GetViewPlaneNormal();
    double view[3], newPos[3];
    vtkMath::Cross(viewup, camangle, view);
    vtkMath::Add(this->m_thisviewer->GetCursorPosition(), view, newPos);
    this->m_thisviewer->SetCursorPosition(newPos);
}

void Test_InteractorStyle::CursorMoveLeft()
{
    if (this->ErrorCheck())
        return;

    double* viewup = this->m_thisviewer->GetActiveCamera()->GetViewUp();
    double* camangle = this->m_thisviewer->GetActiveCamera()->GetViewPlaneNormal();
    double view[3], newPos[3];
    vtkMath::Cross(viewup, camangle, view);
    vtkMath::Subtract(this->m_thisviewer->GetCursorPosition(), view, newPos);
    this->m_thisviewer->SetCursorPosition(newPos);
}
