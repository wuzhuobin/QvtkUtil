/*!
 * \file QvtkViewer.h
 * \date 2017/07/14 13:26
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief QvtkViewer is the basic viewer class used in the framework
 *
 *
 * \note
*/

#ifndef __Qvtk_VIEWER_H__
#define __Qvtk_VIEWER_H__

//me
#include "QvtkAbstractViewer.h"
namespace Ui { class QvtkViewer; }

// vtk
class vtkGenericOpenGLRenderWindow;
class QVTKInteractor;
class QVTKWidget2;


class ABSTRACTVIEWER_EXPORT QvtkViewer: public QvtkAbstractViewer
{
	Q_OBJECT;
public:
	explicit QvtkViewer(QWidget* parent = nullptr);
	virtual ~QvtkViewer() override;

	/**
	 * @brief GetRenderWindow
	 *
     * >>> MUST BE IMPLMENETED!!! <<<
	 * Return subclass of vtkRenderWindow built by QVTKWidget
	 *
	 * @return vtkGenericOpenGLRenderWindow*
	 */
	virtual vtkRenderWindow* GetRenderWindow() override;
	virtual vtkGenericOpenGLRenderWindow* GetGenericOpenGLRenderWindow();

	/**
	 * @brief GetInteractor
	 *
	 * >>> MUST BE IMPLEMENTED <<<
	 * Return subclass of vtkInteractor built by QVTKWidget
     *
	 * @return QVTKInteractor*
	 */
	virtual vtkRenderWindowInteractor* GetInteractor() override;
	virtual QVTKInteractor* GetQVTKInteractor();

	/**
	 * QVTKWidget2* GetQVTKWidget
	 *
	 * @brief
	 *
	 * Return qvtkwidget2 of this viewer
	 *
	 * @return QVTKWidget2*
	 */
	virtual QVTKWidget2* GetQVTKWidget();

protected:
	Ui::QvtkViewer* ui;
};

class QVTKInteractor;

#endif // !__Qvtk_VIEWER_H__
