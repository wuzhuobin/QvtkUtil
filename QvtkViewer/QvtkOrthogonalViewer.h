#ifndef __Qvtk_ORTHOGONAL_VIEWER_H__
#define __Qvtk_ORTHOGONAL_VIEWER_H__

// me
#include "QvtkAbstractViewer.h"
namespace Ui{
	class QvtkOrthogonalViewer;
}
// vtk
class QVTKInteractor;
class vtkGenericOpenGLRenderWindow;

class ABSTRACTVIEWER_EXPORT	QvtkOrthogonalViewer : public QvtkAbstractViewer
{
	Q_OBJECT
public:
	explicit QvtkOrthogonalViewer(QWidget* parent = nullptr);
	virtual ~QvtkOrthogonalViewer() override;

    Ui::QvtkOrthogonalViewer* GetUi() { return this->ui; }
	
	enum ORIENTATION {
		Sagital = 0,
		Coronal = 1,
		Axial = 2
	};

	
	/**
	* void SetAxialViewPlaneNormal
	*
	* @brief
	*
	* Specify one of the three orthogonal directions.
	* The s_axialViewPlaneNormal has a higher priority than s_sagitalViewPlaneNormal.
	* Meaning specifying s_axialViewPlaneNormal can potentially change
	* s_sagitalViewPlaneNormal if it is not orthogonal with the specified s_axialViewPlaneNormal.
	*
	*
	* @param double e1 x-component of desired view plane normal
	* @param double e2 y-component of desired view plane normal
	* @param double e3 z-component of desired view plane normal
	* @return void
	*/
	static void SetAxialViewPlaneNormal(double*);
	static void SetAxialViewPlaneNormal(double e1, double e2, double e3);
	static void SetSagitalViewPlaneNormal(double*);
	static void SetSagitalViewPlaneNormal(double e1, double e2, double e3);
	static const double* GetAxiaViewPlaneNormal();
	static const double* GetSagitalViewPlaneNormal();
	static const double* GetCoronalViewPlaneNormal();


	/**
	* void UpdateCameraViewPlaneNormal
	*
	* @brief
	*
	* Update the view plane normal after the orientation is set
	*
	* @return void
	*/
	virtual void UpdateCameraViewPlaneNormal();
	/**
	* void UpdateViewUp
	*
	* @brief
	*
	* Inherit this method if you wish to force the view up to custom orientation
	*
	* @return void
	*/
	virtual void UpdateViewUp();

	/**
	* @brief GetRenderWindow
	*
	* >>> MUST BE IMPLMENETED!!! <<<
	* Return subclass of vtkRenderWindow built by QVTKWidget
	*
	* @return vtkGenericOpenGLRenderWindow*
	*/

	virtual vtkRenderWindow* GetRenderWindow() override;
	//virtual vtkGenericOpenGLRenderWindow* GetGenericOpenGLRenderWindow();

	/**
	* @brief GetInteractor
	*
	* >>> MUST BE IMPLEMENTED <<<
	* Return subclass of vtkInteractor built by QVTKWidget
	*
	* @return QVTKInteractor*
	*/

	virtual vtkRenderWindowInteractor* GetInteractor() override;
	//virtual QVTKInteractor* GetQVTKInteractor();
	/**
	* void SetRighthandness
	*
	* @brief
	*
	* Setting the righthandness determines the cross sequence of getting yViewPlaneNormal
	* from xViewPlaneNormal and zViewPlaneNormal.
	* If righthandness is true:
	*      \f[ \vec(zViewPlaneNormal) \cross \vec(xViewPlaneNormal) = \vec(yViewPlaneNormal)
	*      \f]
	* Otherwise:
	*      \f[ \vec(xViewPlaneNormal) \cross \vec(zViewPlaneNormal) = \vec(yViewPlaneNormal)
	*      \f]
	*
	* @param bool  righthandness
	* @return void
	*/
	virtual void SetRighthandness(bool right);
	virtual bool GetRighthandness();

public slots:

	virtual void SetOrientation(int orientation);
	virtual void SetOrientationToAxial() { SetOrientation(Axial); }
	virtual void SetOrientationToCoronal() { SetOrientation(Coronal); }
	virtual void SetOrientationToSagital() { SetOrientation(Sagital); }
	virtual int  GetOrientation();

	void slotPushBtnMaximize();
	void slotpushBtnNormal();
	void slotPushBtnDropDownMenu();

signals:

	void OrientationChanged(int orientation);

protected:
	static void InitializeStaticVariables();

	bool righthandness; // default True

	int orientation;
	Ui::QvtkOrthogonalViewer* ui;

	static double s_axialViewPlaneNormal[3];
	static double s_sagitalViewPlaneNormal[3];
	static double s_coronalViewPlaneNormal[3];
	static bool initializeFlag;
};

#endif
