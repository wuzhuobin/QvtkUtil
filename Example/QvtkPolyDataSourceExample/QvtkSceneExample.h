#ifndef __QVTK_SCENE_EXAMPLE_H__
#define __QVTK_SCENE_EXAMPLE_H__
#pragma once
// me
namespace Ui{ class QvtkSceneExample; }
namespace Q {
	namespace vtk {
		class Viewer2;
		class SceneWidget;
		class PolyDataActor;
		class PolyDataSource;
	}
}
// qt
#include <QMainWindow>
class  QvtkSceneExample: public QMainWindow
{
	Q_OBJECT;
public:
	explicit QvtkSceneExample(QWidget* parent = nullptr);
	virtual ~QvtkSceneExample() override;
private slots:
	void on_action_New_triggered(bool checked = false);
	//void on_action_Import_triggered();
	//void on_action_Commit_triggered();
	//void on_action_Print_triggered();

private:
	Ui::QvtkSceneExample* m_ui;
	Q::vtk::Viewer2 *m_viewer;
	Q::vtk::PolyDataSource *m_sources[5];
	Q::vtk::PolyDataActor *m_actors[5];
};

#endif // !__QVTK_SCENE_EXAMPLE_H__
