#ifndef __QVTK_IMAGE_SURFACE_ACTOR_H_
#define __QVTK_IMAGE_SURFACE_ACTOR_H_
#pragma once
// me 
#include "QvtkPolyDataActor.h"
// vtk
class vtkDiscreteMarchingCubes;
class vtkWindowedSincPolyDataFilter;
namespace Q {
	namespace vtk {
		class QVTKDATA_EXPORT ImageSurfaceActor : public PolyDataActor
		{
			Q_OBJECT;
			Q_VTK_DATA_H(ImageSurfaceActor);
		public:
			ImageSurfaceActor();
			virtual ~ImageSurfaceActor() override;
			public Q_SLOTS:
			virtual void setRenderDataSet(DataSet *data) override;
		protected:
			virtual Data *newInstance() const override { return new ImageSurfaceActor; }
			vtkDiscreteMarchingCubes *marchingCubes;
			vtkWindowedSincPolyDataFilter *windowedSincPolyDataFilter;
		};
	}
}

#endif // !__QVTK_IMAGE_SURFACE_ACTOR_H_
