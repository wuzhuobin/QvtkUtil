// me
#include "QvtkImageSlice.h"
#include "QvtkImage.h"
#include "QvtkImageLabel2.h"
#include "QvtkImageLabel.h"
// vtk 
#include <vtkImageActor.h>
#include <vtkImageProperty.h>
#include <vtkImageResliceMapper.h>
#include <vtkNew.h>
#include <vtkPlane.h>
#include <vtkExtractVOI.h>
#include <vtkImageData.h>
// qt
#include <QDomElement>
#include <QDebug>

namespace Q {
	namespace vtk {
		Q_VTK_DATA_CPP(ImageSlice);
		class vtkImageActor2 : public vtkImageActor
		{
		public:
			static vtkImageActor2 *New() { return new vtkImageActor2; }
			virtual double *GetBounds() VTK_OVERRIDE { return vtkImageSlice::GetBounds(); }
		};
		ImageSlice::ImageSlice()
		{
			vtkNew<vtkExtractVOI> extractVOI;
			this->extractVOI = extractVOI.GetPointer();
			// vtkImageResliceMapper use world coordinate plane to cut the 
			// vtkImageActor, so the region do not need to change to Image Coordinate
			// for orthongonal plane, region region should have a pair region which 
			// is the same, set max equals to min 
			vtkNew<vtkImageResliceMapper> mapper;
			this->imageResliceMapper = mapper.GetPointer();
			this->imageResliceMapper->SetInputConnection(this->extractVOI->GetOutputPort());
			this->imageResliceMapper->BorderOn();
			this->imageResliceMapper->ResampleToScreenPixelsOn();
			this->imageResliceMapper->AutoAdjustImageQualityOn();
			this->imageResliceMapper->SetSlabSampleFactor(1);
			this->imageResliceMapper->SliceAtFocalPointOff();
			this->imageResliceMapper->SliceFacesCameraOff();
			this->imageResliceMapper->GetSlicePlane()->SetNormal(0, 0, 1);
			this->imageResliceMapper->GetSlicePlane()->SetOrigin(0, 0, 0);

			vtkImageActor* imageSlice = vtkImageActor2::New();
			imageSlice->SetMapper(this->imageResliceMapper);
			imageSlice->ForceTranslucentOff();

			this->setProp(imageSlice);
		}

		ImageSlice::~ImageSlice()
		{
			setRenderDataSet(nullptr);
		}

		void ImageSlice::printSelf() const
		{
			PlanarProp::printSelf();
		}

		void ImageSlice::reset()
		{
			PlanarProp::reset();
		}

		void ImageSlice::setPlanarNormal(double x, double y, double z)
		{
			PlanarProp::setPlanarNormal(x, y, z);
			this->imageResliceMapper->GetSlicePlane()->SetNormal(x, y, z);
		}

		void ImageSlice::setPlanarOrigin(double x, double y, double z)
		{
			PlanarProp::setPlanarOrigin(x, y, z);
			this->imageResliceMapper->GetSlicePlane()->SetOrigin(x, y, z);
		}

		void ImageSlice::setDisplayRegion(const double region[6])
		{
			PlanarProp::setDisplayRegion(region);

			double _region[6];

			if (this->getRenderDataSet()) {
				vtkImageData* image = static_cast<vtkImageData*>(this->getRenderDataSet()->getDataSet());
				double* spacing = image->GetSpacing();
				/*this->getRenderDataSet()->*/worldRegionToDataSetRegion(region, _region);
				int extent[6];
				image->GetExtent(extent);
				extent[0] = fmax(extent[0], floor(_region[0] / spacing[0]));
				extent[1] = fmin(extent[1], ceil(_region[1] / spacing[0]));
				extent[2] = fmax(extent[2], floor(_region[2] / spacing[1]));
				extent[3] = fmin(extent[3], ceil(_region[3] / spacing[1]));
				extent[4] = fmax(extent[4], floor(_region[4] / spacing[2]));
				extent[5] = fmin(extent[5], ceil(_region[5] / spacing[2]));
				this->extractVOI->SetVOI(extent);

			}
		}

		vtkImageActor * ImageSlice::getImageActor() const
		{
			return static_cast<vtkImageActor*>(this->getProp());
		}

		void ImageSlice::setRenderDataSet(DataSet * data)
		{
			if (this->getRenderDataSet() == data) {
				return;
			}
			// nullptr to remove connection
			if (this->getRenderDataSet()) {
				this->extractVOI->SetInputConnection(nullptr);
				Image* image = qobject_cast<Image*>(this->getRenderDataSet());
				if (image) {
					disconnect(image, &Image::windowChanged,
						this, &ImageSlice::setWindow);
					disconnect(image, &Image::levelChanged,
						this, &ImageSlice::setLevel);
				}
			}
			PlanarProp::setRenderDataSet(data);
			if (this->getRenderDataSet())
			{
				Image* image = qobject_cast<Image*>(data);
				ImageLabel *imageLabel = qobject_cast<ImageLabel*>(data);
				ImageLabel2 *imageLabel2 = qobject_cast<ImageLabel2*>(data);
				if (!image) {
					qCritical() << "data is not Image.";
				}
				else {
					// when using Label class, using nearest interpolation.
					if (imageLabel) {
						this->getImageActor()->GetProperty()->SetInterpolationTypeToNearest();
						this->extractVOI->SetInputConnection(imageLabel->getOutputPort());
					}
					else if (imageLabel2) {
						this->extractVOI->SetInputConnection(imageLabel2->getLabelOutputPort());
					}
					else {
						this->getImageActor()->GetProperty()->SetInterpolationTypeToLinear();
						this->extractVOI->SetInputConnection(data->getOutputPort());
					}
					connect(image, &Image::windowChanged,
						this, &ImageSlice::setWindow);
					connect(image, &Image::levelChanged,
						this, &ImageSlice::setLevel);
					setWindow(image->getWindow());
					setLevel(image->getLevel());
				}
			}
		}
		void ImageSlice::setOpacity(double opacity)
		{
			if (getImageActor()) {
				getImageActor()->GetProperty()->SetOpacity(opacity);
			}
		}
		void ImageSlice::setWindow(double window)
		{
			getImageActor()->GetProperty()->SetColorWindow(window);
		}
		void ImageSlice::setLevel(double level)
		{
			getImageActor()->GetProperty()->SetColorLevel(level);
		}
	}
}