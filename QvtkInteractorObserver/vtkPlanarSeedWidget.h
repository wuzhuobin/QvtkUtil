#ifndef __VTK_PLANAR_SEED_WIDGET_H__
#define __VTK_PLANAR_SEED_WIDGET_H__

// me
#include "abstractinteractorobserver_export.h"

// vtk 
#include "vtkSeedWidget.h"
class vtkBoundedPlanePointPlacer;
class vtkHandleWidget;
class vtkPolyData;
class vtkWidgetSet2;

class ABSTRACTINTERACTOROBSERVER_EXPORT vtkPlanarSeedWidget : public vtkSeedWidget
{
public:
	static vtkPlanarSeedWidget* New();
	vtkTypeMacro(vtkPlanarSeedWidget, vtkSeedWidget);
	virtual void PrintSelf(ostream& os, vtkIndent indent) VTK_OVERRIDE;

	vtkGetObjectMacro(WidgetSet, vtkWidgetSet2);
	virtual void SetWidgetSet(vtkWidgetSet2* widgetSet);

	virtual void SetEnableHandleRange(double range) { this->Range = range; }
	virtual void SetProjectionNormal(int normal);
	virtual void SetProjectionPosition(double x, double y, double z);

	virtual void DropSeed(double pos[3]) { this->DropSeed(pos[0], pos[1], pos[2]); }
	virtual void DropSeed(double x, double y, double z);
	virtual void CleanAllSeed();
	virtual void LoadSeedFromPolyData(vtkPolyData* polyData);
	virtual void SaveSeedToPolyData(vtkPolyData* polyData);

protected:
	vtkPlanarSeedWidget();
	virtual ~vtkPlanarSeedWidget() VTK_OVERRIDE;

	virtual void EnabledHandleInRange(vtkHandleWidget* handle);


	static void AddPointDispatcher(vtkAbstractWidget* widget);
	static void MoveDispatcher(vtkAbstractWidget* widget);
	static void DeleteDispatcher(vtkAbstractWidget* widget);

	
	virtual void AddPointAction(vtkPlanarSeedWidget* dispatcher);
	virtual void MoveAction(vtkPlanarSeedWidget* dispatcher);
	virtual void DeleteAction(vtkPlanarSeedWidget* dispatcher);

	vtkWidgetSet2* WidgetSet;

	double Range;

	int DeletedHandle;
private:
	vtkPlanarSeedWidget(const vtkPlanarSeedWidget&) VTK_DELETE_FUNCTION;
	void operator= (const vtkPlanarSeedWidget&) VTK_DELETE_FUNCTION;

};


#endif // !__VTK_PLANAR_SEED_WIDGET_H__
