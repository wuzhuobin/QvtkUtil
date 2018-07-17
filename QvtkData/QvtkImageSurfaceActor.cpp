// me 
#include "QvtkImageSurfaceActor.h"
#include "QvtkImageLabel2.h"
#include "QvtkPolyData.h"
#undef VTK_SMP_TBB
#if !defined(VTK_SMP_TBB)
#include "vtkDiscreteMarchingCubesWithSmooth.h"
#else
#include "vtkDiscreteFlyingEdges3D.h"
// vtk
#include <vtkWindowedSincPolyDataFilter.h>
#endif // !defined(VTK_SMP_TBB)
// vtk
#include <vtkClipPolyData.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkLookupTable.h>
#include <vtkMatrix4x4.h>
// qt
#include <QDebug>
namespace Q {
	namespace vtk {
		Q_VTK_DATA_CPP(ImageSurfaceActor);
		ImageSurfaceActor::ImageSurfaceActor()
		{
			this->marchingCubes = nullptr;
			this->flyingEdge = nullptr;
			this->windowedSincPolyDataFilter = nullptr;
#ifndef VTK_SMP_TBB
			this->marchingCubes = vtkDiscreteMarchingCubesWithSmooth::New();
			this->marchingCubes->SetPassBand(0.1);
			this->marchingCubes->SetFeatureAngle(120);
			this->marchingCubes->SetBoundarySmoothing(false);
			this->marchingCubes->SetFeatureEdgeSmoothing(false);
			this->marchingCubes->SetNonManifoldSmoothing(true);
			this->marchingCubes->SetNormalizeCoordinates(true);
			this->marchingCubes->SetNumberOfIterations(20);
			this->clipper->SetInputConnection(this->marchingCubes->GetOutputPort());
#else
			this->flyingEdge = vtkDiscreteFlyingEdges3D::New();
			this->flyingEdge->SetComputeScalars(true);
			this->flyingEdge->SetInterpolateAttributes(true);
			this->windowedSincPolyDataFilter = vtkWindowedSincPolyDataFilter::New();
			this->windowedSincPolyDataFilter->SetInputConnection(this->flyingEdge->GetOutputPort());
			this->windowedSincPolyDataFilter->SetPassBand(0.1);
			this->windowedSincPolyDataFilter->SetFeatureAngle(120);
			this->windowedSincPolyDataFilter->SetBoundarySmoothing(false);
			this->windowedSincPolyDataFilter->SetFeatureEdgeSmoothing(false);
			this->windowedSincPolyDataFilter->SetNonManifoldSmoothing(true);
			this->windowedSincPolyDataFilter->SetNormalizeCoordinates(true);
			this->windowedSincPolyDataFilter->SetNumberOfIterations(20);
			this->clipper->SetInputConnection(this->windowedSincPolyDataFilter->GetOutputPort());
#endif // !VTK_SMP_TBB
			this->polyDataMapper->SetScalarVisibility(true);
		}

		ImageSurfaceActor::~ImageSurfaceActor()
		{
#ifndef VTK_SMP_TBB
			this->marchingCubes->Delete();
#else
			this->flyingEdge->Delete();
			this->windowedSincPolyDataFilter->Delete();
#endif // !VTK_SMP_TBB
		}

		void ImageSurfaceActor::getPolyData(PolyData * data) const
		{
#ifndef VTK_SMP_TBB
			data->getPolyData()->ShallowCopy(this->marchingCubes->GetOutput());
#else
			data->getPolyData()->ShallowCopy(this->windowedSincPolyDataFilter->GetOutput());
#endif // !VTK_SMP_TBB
			data->setPosition(0.0, 0.0, 0.0);
			data->setOrigin(0.0, 0.0, 0.0);
			data->setScale(1.0, 1.0, 1.0);
			data->setOrientation(0.0, 0.0, 0.0);
			data->getUserMatrix()->Identity();
		}

		void ImageSurfaceActor::setRenderDataSet(DataSet * data)
		{
			if (this->getRenderDataSet() == data) {
				return;
			}
			// nullptr to remove connection
			if (this->getRenderDataSet()) {
#ifndef VTK_SMP_TBB
				this->marchingCubes->SetInputConnection(nullptr);
#else
				this->flyingEdge->SetInputConnection(nullptr);
#endif // !VTK_SMP_TBB
			}
			Prop::setRenderDataSet(data);
			if (this->getRenderDataSet())
			{
				ImageLabel2 *label = qobject_cast<ImageLabel2*>(this->getRenderDataSet());
				if (!label) {
					qCritical() << "data is not ImageLabel2";
				}
				else {
#ifndef VTK_SMP_TBB
					this->marchingCubes->GenerateValues(
						label->getLookupTable()->GetNumberOfTableValues(),
						0,
						label->getLookupTable()->GetNumberOfTableValues() - 1);
					this->marchingCubes->SetInputConnection(this->getRenderDataSet()->getOutputPort());
#else
					this->flyingEdge->GenerateValues(
						label->getLookupTable()->GetNumberOfTableValues(),
						0, 
						label->getLookupTable()->GetNumberOfTableValues() - 1);
					this->flyingEdge->SetInputConnection(this->getRenderDataSet()->getOutputPort());
#endif // !VTK_SMP_TBB
					this->polyDataMapper->SetLookupTable(label->getLookupTable());
					this->polyDataMapper->SetUseLookupTableScalarRange(true);
				}
			}
		}
	}
}