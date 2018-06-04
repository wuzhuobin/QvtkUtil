// me 
#include "QvtkImageSurfaceActor.h"
#include "QvtkImageLabel2.h"
#include "QvtkPolyData.h"
// vtk
#include <vtkDiscreteMarchingCubes.h>
#include <vtkWindowedSincPolyDataFilter.h>
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
			this->marchingCubes = vtkDiscreteMarchingCubes::New();
			this->windowedSincPolyDataFilter = vtkWindowedSincPolyDataFilter::New();
			this->windowedSincPolyDataFilter->SetInputConnection(this->marchingCubes->GetOutputPort());
			this->windowedSincPolyDataFilter->SetBoundarySmoothing(false);
			this->windowedSincPolyDataFilter->SetFeatureEdgeSmoothing(false);
			this->windowedSincPolyDataFilter->SetFeatureAngle(120.0);
			this->windowedSincPolyDataFilter->SetNonManifoldSmoothing(true);
			this->windowedSincPolyDataFilter->SetNormalizeCoordinates(true);
			this->clipper->SetInputConnection(this->windowedSincPolyDataFilter->GetOutputPort());
			this->polyDataMapper->SetScalarVisibility(true);
		}

		ImageSurfaceActor::~ImageSurfaceActor()
		{
		}

		void ImageSurfaceActor::getPolyData(PolyData * data) const
		{
			data->getPolyData()->ShallowCopy(this->windowedSincPolyDataFilter->GetOutput());
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
				this->marchingCubes->SetInputConnection(nullptr);
			}
			Prop::setRenderDataSet(data);
			if (this->getRenderDataSet())
			{
				ImageLabel2 *label = qobject_cast<ImageLabel2*>(this->getRenderDataSet());
				if (!label) {
					qCritical() << "data is not ImageLabel2";
				}
				else {
					this->marchingCubes->GenerateValues(
						label->getLookupTable()->GetNumberOfTableValues(), 
						0, label->getLookupTable()->GetNumberOfTableValues() - 1);
					this->marchingCubes->SetInputConnection(this->getRenderDataSet()->getOutputPort());
					//this->polyDataMapper->SetScalarRange(0, label->getLookupTable()->GetNumberOfTableValues());
					this->polyDataMapper->SetLookupTable(label->getLookupTable());
					this->polyDataMapper->SetUseLookupTableScalarRange(true);
				}
			}
		}
	}
}