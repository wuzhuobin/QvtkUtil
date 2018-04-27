#ifndef __Qvtk_IMAGE_SLICE_H__
#define __Qvtk_IMAGE_SLICE_H__

// me
#include "QvtkPlanarProp.h"

// vtk
class vtkImageActor;
class vtkImageResliceMapper;
class vtkExtractVOI;
namespace Q {
namespace vtk{

class QVTKDATA_EXPORT ImageSlice : public PlanarProp
{
	Q_OBJECT;
	Q_VTK_DATA_H(ImageSlice)
public:
	ImageSlice();

	virtual ~ImageSlice() override;

	virtual void printSelf() const override;

	virtual vtkImageActor* getImageActor() const;

public slots:

	virtual void reset() override;

	virtual void setPlanarNormal(double x, double y, double z) override;

	virtual void setPlanarOrigin(double x, double y, double z) override;

	virtual void setDisplayRegion(const double region[6]) override;

	virtual void setRenderDataSet(DataSet* data = nullptr) override;
	/**
	 * @brief call in #setRenderDataSet
	 */
	virtual void setOpacity(double opacity) override;
	virtual void setWindow(double window);
	virtual void setLevel(double level);
	
protected:

	virtual Data* newInstance() const override { return new ImageSlice; }

	vtkImageResliceMapper* imageResliceMapper;

	vtkExtractVOI* extractVOI;

private:




};

}
}
#endif // !__Qvtk_IMAGE_SLICE_H__

