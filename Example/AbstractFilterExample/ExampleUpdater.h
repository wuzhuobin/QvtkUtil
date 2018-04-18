/*!
 * \file ExampleUpdater.h
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

#ifndef __SCB_EXAMPLE_UPDATER_H__
#define __SCB_EXAMPLE_UPDATER_H__


#include <QObject>

class vtkRenderWindow;
class vtkRenderer;

class Updater : public QObject
{
	Q_OBJECT
public:

	/**
	 * @brief SetRenderWindow
	 *
	 * Set the render window to be updated.
	 *
	 * @param * Put argument desc here
	 * @return void
	 */
	
	void SetRenderWindow(vtkRenderWindow*);
	
	
	/**
	 * @brief ConnectModifyingObject
	 *
	 * Set the modifying object that emits the required signal.
	 *
	 * @param * Put argument desc here
	 * @return void
	 */

	void ConnectModifyingObject(QObject*);

public slots:
	
	void slotRender();

protected:
	vtkRenderWindow* m_ren;
};


#endif