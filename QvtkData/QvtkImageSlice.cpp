// me
#include "QvtkImageSlice.h"
#include "QvtkImage.h"

// vtk 
#include <vtkImageActor.h>
//#include <vtkImageActor.h>
//#include <vtkImageActorMapper.h>
//#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageProperty.h>
#include <vtkImageResliceMapper.h>
#include <vtkNew.h>
#include <vtkPlane.h>
#include <vtkExtractVOI.h>
#include <vtkImageData.h>

// qt
#include <QDomElement>
#include <QDebug>

Q_VTK_DATACPP(QvtkImageSlice);
class vtkImageActor2 : public vtkImageActor
{
public:
	static vtkImageActor2 *New() { return new vtkImageActor2; }
	virtual double *GetBounds() VTK_OVERRIDE { return vtkImageSlice::GetBounds(); }
};

QvtkImageSlice::QvtkImageSlice()
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

	//setSlicePlane(this->imageResliceMapper->GetSlicePlane());

	//mapper->Register(this->prop3D);
}

QvtkImageSlice::~QvtkImageSlice()
{
	setRenderDataSet(nullptr);
}

void QvtkImageSlice::printSelf() const
{
	QvtkAbstractPlanarProp::printSelf();
}

//bool QvtkImageSlice::isClass(QString className) const
//{
//	if (className != getClassName()) {
//		return QvtkAbstractPlanarProp::isClass(className);
//	}
//	return true;
//}

void QvtkImageSlice::reset()
{
	QvtkAbstractPlanarProp::reset();
}

void QvtkImageSlice::setPlanarNormal(double x, double y, double z)
{
	QvtkAbstractPlanarProp::setPlanarNormal(x, y, z);
	this->imageResliceMapper->GetSlicePlane()->SetNormal(x, y, z);
}

void QvtkImageSlice::setPlanarOrigin(double x, double y, double z)
{
	QvtkAbstractPlanarProp::setPlanarOrigin(x, y, z);
	this->imageResliceMapper->GetSlicePlane()->SetOrigin(x, y, z);
}

void QvtkImageSlice::setDisplayRegion(const double region[6])
{
	QvtkAbstractPlanarProp::setDisplayRegion(region);

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

vtkImageActor * QvtkImageSlice::getImageActor() const
{
	return static_cast<vtkImageActor*>(this->getProp());
}

void QvtkImageSlice::setRenderDataSet(DataSet * data)
{
	if (this->getRenderDataSet() == data) {
		return;
	}
	// nullptr to remove connection
	if (this->getRenderDataSet()) {
		this->extractVOI->SetInputConnection(nullptr);
		QvtkImage* image = qobject_cast<QvtkImage*>(this->getRenderDataSet());
		if (image) {
			disconnect(image, &QvtkImage::windowChanged,
				this, &QvtkImageSlice::setWindow);
			disconnect(image, &QvtkImage::levelChanged,
				this, &QvtkImageSlice::setLevel);
		}
	}
	QvtkAbstractPlanarProp::setRenderDataSet(data);
	if(this->getRenderDataSet())
	{
		this->extractVOI->SetInputConnection(data->getOutputPort());
		QvtkImage* image = qobject_cast<QvtkImage*>(data);
		if (!image) {
			qCritical() << "data is not QvtkImage.";
		}
		else {
			connect(image, &QvtkImage::windowChanged,
				this, &QvtkImageSlice::setWindow);
			connect(image, &QvtkImage::levelChanged,
				this, &QvtkImageSlice::setLevel);
			setWindow(image->getWindow());
			setLevel(image->getLevel());
		}

	}
}

void QvtkImageSlice::setOpacity(double opacity)
{
	if (getImageActor()) {
		getImageActor()->GetProperty()->SetOpacity(opacity);
	}
}

void QvtkImageSlice::setWindow(double window)
{
	getImageActor()->GetProperty()->SetColorWindow(window);
}

void QvtkImageSlice::setLevel(double level)
{
	getImageActor()->GetProperty()->SetColorLevel(level);
}
