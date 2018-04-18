#ifndef __SCB_SCENE_EXAMPLE_H__
#define __SCB_SCENE_EXAMPLE_H__

// me
namespace Ui{ class SCBSceneExample; }
class SCBOrthogonalViewer;
class SCBSceneWidget;
class SCBPolyDataActor;
class SCBPolyDataSource;

// qt
#include <QMainWindow>

class  SCBSceneExample: public QMainWindow
{
	Q_OBJECT;
public:
	explicit SCBSceneExample(QWidget* parent = nullptr);
	virtual ~SCBSceneExample() override;
private slots:
	void on_action_New_triggered(bool checked = false);
	//void on_action_Import_triggered();
	//void on_action_Commit_triggered();
	//void on_action_Print_triggered();

private:
	Ui::SCBSceneExample* m_ui;

	SCBOrthogonalViewer *m_viewer;

	SCBPolyDataSource *m_sources[5];
	SCBPolyDataActor *m_actors[5];
};

#endif // !__SCB_SCENE_EXAMPLE_H__
