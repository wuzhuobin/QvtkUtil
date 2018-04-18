/*!
 * \file QvtkAbstractImageFilter.h
 * \date 2017/07/06 10:28
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class is the abstract class for all image output filters.
 *
 *
 * \note
*/

#ifndef __Qvtk_ABSTRACT_IMAGE_FILTER_H__
#define __Qvtk_ABSTRACT_IMAGE_FILTER_H__

#include "QvtkAbstractFilter.h"

// vtk
class vtkImageAlgorithm;
class vtkDataObject;
class vtkImageData;

class ABSTRACTFILTER_EXPORT QvtkAbstractImageFilter : public QvtkAbstractFilter
{
	Q_OBJECT
public:
	explicit QvtkAbstractImageFilter(QWidget* parent = nullptr);
	virtual ~QvtkAbstractImageFilter() override;

	/**
	 * @brief SetInputData
	 *
	 * Wrapper to vtkImageAlgorithm::SetInputData()
	 *
	 * @param *data Input data object pointer
	 * @param index Port number of the input, default = 0
	 * @return void
	 */

	virtual void SetInputData(vtkDataObject *data, int index = 0);
	
	/**
	 * @brief GetOutput
	 *
	 * Wrapper to vtkImageAlgorithm::GetOutput()
	 *
	 * @param index Port of the desired output, default = 0
	 * @return vtkImageData*
	 */

	virtual vtkImageData* GetOutput(int index = 0);

protected:


	/**
	 * @brief	Convenient method to get ImageFilter
	 * Doing down cast of the #vtkFilter, and return it 
	 * @return	the down casted #vtkFilter
	 */
	vtkImageAlgorithm* GetFilter();

    /**
     * bool IsITKFilter
     *
     * @brief 
     *
     * Internal method
     *
     * @return bool
     */
    bool IsITKFilter();
};

#endif
