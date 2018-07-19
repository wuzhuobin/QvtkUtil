#ifndef __QVTK_PLANAR_VIEWER_H__
#define __QVTK_PLANAR_VIEWER_H__
#pragma once
// me
#include "QvtkOrthogonalViewer.h"
// vtk
//class vtkTextActor;
class vtkAxisActor2D;
namespace Q {
	namespace vtk {

		class QVTKVIEWER_EXPORT PlanarViewer : public OrthogonalViewer
		{
			Q_OBJECT;
		public:
			explicit PlanarViewer(QWidget* parent = nullptr);
			virtual ~PlanarViewer() override;
			vtkRenderer* GetAnnotationRenderer() { return this->getRenderers()[1]; }
			virtual double getSliceThickness() const { return this->sliceThickness; }
			virtual bool getUpdateAxesFlag() const { return this->updateAxesFlag; }
			virtual bool getOrientationTextFlag() const { return this->orientationTextFlag; }
		public Q_SLOTS:
			virtual void IncrementSlice(bool sign);
			virtual void setSliceThickness(double sliceThickness) { this->sliceThickness = sliceThickness; }
			virtual void setOrientation(int orientation) override;
			virtual void setUpdateAxesFlag(bool flag);
			void updateAxesFlagOn() { this->setUpdateAxesFlag(true); }
			void updateAxesFlagOff() { this->setUpdateAxesFlag(false); }
			virtual void setOrientationTextFlag(bool flag);
			void orientationTextFlagOn() { this->setOrientationTextFlag(true); }
			void orientationTextFlagOff() { this->setOrientationTextFlag(false); }
		protected:
			virtual double* UpdateViewUp() override;
			double sliceThickness;
			//vtkTextActor* orientationActor[4];
			vtkAxisActor2D* verticalAxis;
			vtkAxisActor2D* horizontalAxis;
			bool updateAxesFlag;
			bool orientationTextFlag;
		protected Q_SLOTS:
			virtual void updateCursorPosition(double x, double y, double z) override;
		private:
			Q_DISABLE_COPY(PlanarViewer);
		};
	}
}
#endif // !__QVTK_PLANAR_VIEWER_H__