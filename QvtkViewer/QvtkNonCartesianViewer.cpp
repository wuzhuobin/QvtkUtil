/*!
* \file QvtkAbstractNonCartesianViewer.cpp
* \date 2017/07/14 18:21
*
* \author 		Wong, Matthew Lun
* Occupation	Chinese University of Hong Kong,
* 				Department of Imaging and Interventional Radiology,
* 				M.Phil Student
* Contact: 	fromosia@gmail.com
*
* \brief This class should be inherited instead of Viewer if the coordinate space of the viewer
*        is not Cartesian. Dev should make sure the method MapCoord2Cartesian(e1, e2, e3) returns correct
*        coordinate in Cartesian space.
*
*
* \note
*/

#include "QvtkNonCartesianViewer.h"

//vtkTransform* NonCartesianViewer::s_nonCartesianCusorTransform = nullptr;
namespace Q {
namespace vtk{
double* NonCartesianViewer::MapCoord2Cartesian(double* coord)
{
    return this->MapCoord2Cartesian(coord[0], coord[1], coord[2]);
}

double* NonCartesianViewer::MapCursor2Cartesian()
{
    return this->MapCoord2Cartesian(this->GetCursorPosition());
}

//vtkTransform* NonCartesianViewer::GetCursorTransform()
//{
//    if (this->desyncCursorFlag)
//    {
//        return this->desyncCursorPosTransform; // Note that the desync cursor here is also non-cartesian
//    }
//    else
//    {
//        return s_nonCartesianCusorTransform;
//    }
//}

NonCartesianViewer::NonCartesianViewer(QWidget* parent /*= nullptr*/) 
	: Viewer(parent)
{

}


}
}