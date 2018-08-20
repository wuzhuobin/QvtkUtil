#ifndef __QVTK_SCENE_EXAMPLE_H__
#define __QVTK_SCENE_EXAMPLE_H__
#pragma once
// me
namespace Ui{ class QvtkSceneExample; }
namespace Q {
	namespace vtk {
		class PlanarViewer;
		class Image;
		class ImageLabel2;
		class ImageSlice;
	}
}
// qt
#include <QMainWindow>
class QvtkSceneExample : public QMainWindow
{
	Q_OBJECT;
public:
	explicit QvtkSceneExample(QWidget* parent = nullptr);
	virtual ~QvtkSceneExample() override;
	private slots:
	void on_action_New_triggered(bool checked = false);
	void on_action_New2_triggered(bool checked = false);
private:
	Ui::QvtkSceneExample* m_ui;
	Q::vtk::PlanarViewer* m_viewer;
	Q::vtk::Image *m_image;
	Q::vtk::ImageLabel2 *m_label;
	Q::vtk::ImageSlice *m_imageSlice;
	Q::vtk::ImageSlice *m_imageLabelSlice;



};

#endif // !__QVTK_SCENE_EXAMPLE_H__
