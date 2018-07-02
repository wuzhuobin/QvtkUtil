#ifndef __QVTK_NON_PLANAR_VIEWER_H__
#define __QVTK_NON_PLANAR_VIEWER_H__
#pragma once
// me 
#include "QvtkOrthogonalViewer.h"
// vtk
class vtkOrientationMarkerWidget;
class vtkAxesActor;
namespace Q {
	namespace vtk {
		class QVTKVIEWER_EXPORT NonPlanarViewer : public OrthogonalViewer
		{
			Q_OBJECT;
		public:
			explicit NonPlanarViewer(QWidget* parent = nullptr);
			virtual ~NonPlanarViewer() override;
			virtual bool getOrientationMarkerWidgetFlag() const { return this->orientationMarkerWidgetFlag; }
		public slots:
			virtual void setOrientationMarkerWidgetFlag(bool flag);
			void orientationMarkerWidgetFlagOn() { this->setOrientationMarkerWidgetFlag(true); }
			void orientationMarkerWidgetFlagOff() { this->setOrientationMarkerWidgetFlag(false); }
		protected:
			bool orientationMarkerWidgetFlag;
			vtkOrientationMarkerWidget *orientationMarkerWidget;
			vtkAxesActor *axesActor;
		};
	}
}
#endif // !__QVTK_NON_PLANAR_VIEWER_H__
