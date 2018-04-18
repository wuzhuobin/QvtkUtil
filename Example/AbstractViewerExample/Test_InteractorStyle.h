/*!
 * \file Test_InteractorStyle.h
 * \date 2017/07/14 16:43
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief Interactorstyle written for demonstrating the synced cursor and desync function.
 *        FindViewer function can be considered a standard method to find viewer from interactorstyle
 *
 * \note
*/

#ifndef __SCB_TEST_INTERACTOR_H__
#define __SCB_TEST_INTERACTOR_H__



#include <SCBAbstractViewer.h>
#include <vtkInteractorStyleTrackballCamera.h>

#include <vtkObjectFactory.h>

class Test_InteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
    static Test_InteractorStyle* New();
    vtkTypeMacro(Test_InteractorStyle, vtkInteractorStyleTrackballCamera);

    /**
     * @brief OnKeyPress
     *
     * Press arrow keys to move the cursor 
     *
     * @return void
     */
    
    virtual void OnKeyPress() override;


    /**
     * @brief SetInteractor
     *
     * FindViewer is called after Superclass call to find the viewer 
     * with this interactorstyle
     *
     * @param interactor Put argument desc here
     * @return void
     */
    virtual void SetInteractor(vtkRenderWindowInteractor *interactor) override;

    void SetViewer(SCBAbstractViewer*);

protected:
    Test_InteractorStyle();
    ~Test_InteractorStyle() {};
    
    /**
     * @brief FindViewer
     *
     * Finds viewer of this interactorstyle
     *
     * @return void
     */
    void FindViewer();

    /**
     * @brief ErrorCheck
     *
     * Return 0 if success 1 if failed
     *
     * @return bool
     */
    bool ErrorCheck();

    SCBAbstractViewer* m_thisviewer;
private:
    void CursorMoveUp();
    void CursorMoveDown();
    void CursorMoveLeft();
    void CursorMoveRight();

};

#endif