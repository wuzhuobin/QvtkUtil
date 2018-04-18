#ifndef __SCB_SCENE_EXAMPLE_H__
#define __SCB_SCENE_EXAMPLE_H__

// me
namespace Ui{ class SCBSceneExample; }
class SCBSceneWidget;
class SCBPlanarViewer;
class SCBImage;
class SCBImageSlice;
//class SCBImageLabel;

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
	void on_action_New2_triggered(bool checked = false);
	void on_action_Opacity_triggered(bool checked = false);
	void on_action_Import_triggered(bool checked = false);
	void on_action_Commit_triggered(bool checked = false);

private:
	Ui::SCBSceneExample* m_ui;
	SCBPlanarViewer* m_viewer;
	SCBImage* m_image;
	SCBImageSlice* m_imageSlice;
	//SCBImageLabel* m_imageLabel;
	SCBImageSlice* m_imageLabelSlice;



};

#endif // !__SCB_SCENE_EXAMPLE_H__
