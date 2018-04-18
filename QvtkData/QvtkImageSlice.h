#ifndef __Qvtk_IMAGE_SLICE_H__
#define __Qvtk_IMAGE_SLICE_H__

// me
#include "QvtkAbstractPlanarProp.h"

// vtk
class vtkImageActor;
class vtkImageResliceMapper;
class vtkExtractVOI;

class ABSTRACTDATA_EXPORT QvtkImageSlice : public QvtkAbstractPlanarProp
{
	Q_OBJECT;
	Q_VTK_DATAH(QvtkImageSlice)
public:
	QvtkImageSlice();

	virtual ~QvtkImageSlice() override;

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

	virtual Data* newInstance() const override { return new QvtkImageSlice; }

	vtkImageResliceMapper* imageResliceMapper;

	vtkExtractVOI* extractVOI;

private:




};


#endif // !__Qvtk_IMAGE_SLICE_H__
