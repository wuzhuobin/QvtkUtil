#ifndef __Qvtk_DISCRETE_MARCHING_CUBES_H__
#define __Qvtk_DISCRETE_MARCHING_CUBES_H__

// me
#include "QvtkAbstractPolyDataFilter.h"

namespace Ui {
	class QvtkDiscreteMarchingCubes;
}
class vtkDiscreteMarchingCubesWithSmooth;

class ABSTRACTFILTER_EXPORT QvtkDiscreteMarchingCubes final : public QvtkAbstractPolyDataFilter
{
	Q_OBJECT;
public:
	explicit QvtkDiscreteMarchingCubes(QObject* parent = nullptr);
	virtual ~QvtkDiscreteMarchingCubes() override;


	virtual void LoadUI() override;

	virtual int ConnectUI() override;

public slots:
	virtual void slotSetNumberOfIterations(int smoothingIterations);
	virtual void slotGenerateValues();

protected:
	
	virtual vtkDiscreteMarchingCubesWithSmooth* GetDiscreteMarchingCubesWithSmooth();

private:
	Ui::QvtkDiscreteMarchingCubes* ui;

};


#endif // !__Qvtk_DISCRETE_MARCHING_CUBES_H__
