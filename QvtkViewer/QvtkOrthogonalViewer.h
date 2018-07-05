#ifndef __QVTK_ORTHOGONAL_VIEWER_H__
#define __QVTK_ORTHOGONAL_VIEWER_H__
#pragma once
// me
#include "QvtkViewer.h"
namespace Q {
	namespace vtk {
		namespace Ui {
			class OrthogonalViewer;
		}
	}
}
// vtk
class QVTKInteractor;
class vtkGenericOpenGLRenderWindow;
namespace Q {
	namespace vtk {
		class QVTKVIEWER_EXPORT	OrthogonalViewer : public Viewer
		{
			Q_OBJECT
		public:
			static const double ORIENTATION_YZ_NORMAL[3];
			static const double ORIENTATION_XZ_NORMAL[3];
			static const double ORIENTATION_XY_NORMAL[3];
			explicit OrthogonalViewer(QWidget* parent = nullptr);
			virtual ~OrthogonalViewer() override;
			vtkRenderer *getFirstRenderer() { return this->getRenderers()[0]; }
			Ui::OrthogonalViewer* getUi() { return this->ui; }
			typedef enum ORIENTATION {
				ORIENTATION_YZ = 0,
				ORIENTATION_XZ = 1,
				ORIENTATION_XY = 2,
				SAGITAL = 3,
				CORONAL = 4,
				AXIAL = 5
			}ORIENTATION;
			/**
			* void SetAxialViewPlaneNormal
			*
			* @brief
			*
			* Specify one of the three orthogonal directions.
			* The GetAxiaViewPlaneNormal() has a higher priority than GetSagitalViewPlaneNormal().
			* Meaning specifying GetAxiaViewPlaneNormal() can potentially change
			* GetSagitalViewPlaneNormal() if it is not orthogonal with the specified GetAxiaViewPlaneNormal().
			*
			*
			* @param double e1 x-component of desired view plane normal
			* @param double e2 y-component of desired view plane normal
			* @param double e3 z-component of desired view plane normal
			* @return void
			*/
			virtual void setAxialViewPlaneNormal(double e1, double e2, double e3);
			void setAxialViewPlaneNormal(double*);
			virtual void setSagitalViewPlaneNormal(double e1, double e2, double e3);
			void setSagitalViewPlaneNormal(double*);
			virtual const double* getAxialViewPlaneNormal() const;
			void getAxialViewPlaneNormal(double normal[3]) const;
			virtual const double* getSagitalViewPlaneNormal() const;
			void getSagitalViewPlaneNormal(double normal[3]) const;
			virtual const double* getCoronalViewPlaneNormal() const;
			virtual const double* getCurrentPlaneNormal() const { return this->currentPlaneNormal; }
			void getCurrentPlaneNormal(double normal[3]) const;
			/**
			* @brief GetRenderWindow
			*
			* >>> MUST BE IMPLMENETED!!! <<<
			* Return subclass of vtkRenderWindow built by QVTKWidget
			*
			* @return vtkGenericOpenGLRenderWindow*
			*/
			virtual vtkRenderWindow* getRenderWindow() override;
			/**
			* @brief getInteractor
			*
			* >>> MUST BE IMPLEMENTED <<<
			* Return subclass of vtkInteractor built by QVTKWidget
			*
			* @return QVTKInteractor*
			*/
			virtual vtkRenderWindowInteractor* getInteractor() override;
			/**
			* void SetRighthandness
			*
			* @brief
			*
			* Setting the righthandness determines the cross sequence of getting yViewPlaneNormal
			* from xViewPlaneNormal and zViewPlaneNormal.
			* If righthandness is true:
			*      \f[ \vec(zViewPlaneNormal) \cross \vec(xViewPlaneNormal) = \vec(yViewPlaneNormal)
			*      \f]
			* Otherwise:
			*      \f[ \vec(xViewPlaneNormal) \cross \vec(zViewPlaneNormal) = \vec(yViewPlaneNormal)
			*      \f]
			*
			* @param bool  righthandness
			* @return void
			*/
			virtual void setRighthandness(bool right);
			virtual bool getRighthandness() const;
			virtual void setViewPlaneNormalSyncFlag(bool flag);
			virtual bool getViewPlaneNormalSyncFlag() const { return this->viewPlaneNormalSyncFlag; }
			virtual int  getOrientation() const { return this->orientation; }
		public Q_SLOTS:
			virtual void setOrientation(int orientation);
			void setOrientationToAxial() { this->setOrientation(AXIAL); }
			void setOrientationToXY() { this->setOrientation(ORIENTATION_XY); }
			void setOrientationToCoronal() { this->setOrientation(CORONAL); }
			void setOrientationToXZ() { this->setOrientation(ORIENTATION_XZ); }
			void setOrientationToSagital() { this->setOrientation(SAGITAL); }
			void setOrientationToYZ() { this->setOrientation(ORIENTATION_YZ); }
		Q_SIGNALS:
			void OrientationChanged(int orientation);
		protected:
			/**
			* void UpdateCameraViewPlaneNormal
			*
			* @brief
			*
			* Update the view plane normal after the orientation is set
			*
			* @return void
			*/
			const double* UpdateCameraViewPlaneNormal();
			/**
			* void UpdateViewUp
			*
			* @brief
			*
			* Inherit this method if you wish to force the view up to custom orientation
			*
			* @return void
			*/
			virtual double* UpdateViewUp();
			Ui::OrthogonalViewer* ui;
		private:
			int orientation;
			bool viewPlaneNormalSyncFlag;
			bool righthandness; // default True
			double sagitalViewPlaneNormal[3];
			mutable double coronalViewPlaneNormal[3];
			double axialViewPlaneNormal[3];
			double currentPlaneNormal[3];
		};
	}
}
#endif

