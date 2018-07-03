/*!
 * \file Viewer2.h
 * \date 2017/07/14 13:26
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong,
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief Viewer2 is the basic viewer class used in the framework
 *
 *
 * \note
*/
#ifndef __QVTK_VIEWER2_H__
#define __QVTK_VIEWER2_H__
#pragma once
//me
#include "QvtkViewer.h"
namespace Q {
	namespace vtk {
		namespace Ui { class Viewer2; }
	}
}
// vtk
class vtkGenericOpenGLRenderWindow;
class QVTKInteractor;
class QVTKWidget2;
namespace Q {
	namespace vtk {
		class QVTKVIEWER_EXPORT Viewer2 : public Viewer
		{
			Q_OBJECT;
		public:
			explicit Viewer2(QWidget* parent = nullptr);
			virtual ~Viewer2() override;
			/**
			 * @brief GetRenderWindow
			 *
			 * >>> MUST BE IMPLMENETED!!! <<<
			 * Return subclass of vtkRenderWindow built by QVTKWidget
			 *
			 * @return vtkGenericOpenGLRenderWindow*
			 */
			virtual vtkRenderWindow* getRenderWindow() override;
			virtual vtkGenericOpenGLRenderWindow* GetGenericOpenGLRenderWindow();
			/**
			 * @brief getInteractor
			 *
			 * >>> MUST BE IMPLEMENTED <<<
			 * Return subclass of vtkInteractor built by QVTKWidget
			 *
			 * @return QVTKInteractor*
			 */
			virtual vtkRenderWindowInteractor* getInteractor() override;
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
			Ui::Viewer2* ui;
		};
	}
}
#endif // !__QVTK_VIEWER2_H__
