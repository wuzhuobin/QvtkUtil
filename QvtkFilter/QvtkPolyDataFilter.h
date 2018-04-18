#ifndef __Qvtk_ABSTRACT_POLYDATA_FILTER_H
#define __Qvtk_ABSTRACT_POLYDATA_FILTER_H

#include "QvtkAbstractFilter.h"

class vtkPolyDataAlgorithm;
class vtkPolyData;
class vtkDataObject;

class ABSTRACTFILTER_EXPORT QvtkAbstractPolyDataFilter: public QvtkAbstractFilter
{
	Q_OBJECT
public:
	QvtkAbstractPolyDataFilter(QWidget* parent = nullptr);
	virtual ~QvtkAbstractPolyDataFilter() override;
	
	/* Wrap IO of vtkAlgorithm */
	virtual void SetInputData(vtkDataObject *data, int index=0);
	virtual vtkPolyData* GetOutput(int index=0);
	

protected:

	/**
	 * @brief	Convenient method to get the PolyDataFilter
	 * Doing down cast of the #vtkFilter, and return it 
	 * @return	the down casted #vtkFilter
	 */
	 vtkPolyDataAlgorithm* GetFilter();

};


#endif
