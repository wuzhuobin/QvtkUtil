/*!
 * \file QvtkAbstractNonCartesianViewer.h
 * \date 2017/07/14 18:21
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief This class should be inherited instead of QvtkAbstractViewer if the coordinate space of the viewer
 *        is not Cartesian. Dev should make sure the method MapCoord2Cartesian(e1, e2, e3) returns correct
 *        coordinate in Cartesian space. 
 *
 *
 * \note
*/

#ifndef __Qvtk_ABSTRACT_NON_CARTESIAN_VIEWER_H__
#define __Qvtk_ABSTRACT_NON_CARTESIAN_VIEWER_H__

#include "QvtkAbstractViewer.h"

class ABSTRACTVIEWER_EXPORT QvtkABstractNonCartesianViewer : public QvtkAbstractViewer
{
    Q_OBJECT
public:

    /**
     * @brief double* MapCoord2Cartesian
     *
     * Convert coordinate from viewer's space to Cartesian space
     *
     * @param double e1 x-coordinate in viewer space
     * @param double e2 y-coordinate in viewer space
     * @param double e3 z-coordinate in viewer space
     * @return double*
     */
    virtual double* MapCoord2Cartesian(double*);
    virtual double* MapCoord2Cartesian(double e1, double e2, double e3) = 0;
    virtual double* MapCursor2Cartesian();

    /**
     * @brief vtkTransform* GetCursorTransform
     *
     * Override this method to return static non-Cartesian cursor transform instead
     *
     * @return vtkTransform*
     */
    //virtual vtkTransform* GetCursorTransform();

protected:
	QvtkABstractNonCartesianViewer(QWidget* parent = nullptr);
	~QvtkABstractNonCartesianViewer() {};

    //static vtkTransform* s_nonCartesianCusorTransform;
};

#endif
