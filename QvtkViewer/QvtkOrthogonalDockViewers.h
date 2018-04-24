#ifndef __QVTK_ORTHOGONAL_DOCK_VIEWERS_H__
#define __QVTK_ORTHOGONAL_DOCK_VIEWERS_H__

// me
#include "qvtkviewer_export.h"
namespace Q {
namespace vtk{
class OrthogonalViewer;
}
}

// qt 
#include <QMainWindow>
template <typename T>
class QList;
class QDockWidget;
class QSignalMapper;

namespace Q {
namespace vtk{
class QVTKVIEWER_EXPORT OrthogonalDockViewers : public QMainWindow
{
	Q_OBJECT;
public: 
	explicit OrthogonalDockViewers(QWidget* parent = nullptr);
	virtual ~OrthogonalDockViewers() override;

	void deleteAllViewer();
	void setViewer(int i);
	void setViewer(int i, OrthogonalViewer* viewer);
	OrthogonalViewer* getViewers(int i) const;
	QDockWidget* getDocker(int i) const;
	int getNumberOfViewers() const;
private slots:
	void showMaximizedOrNormal(int i) const;

private:

	void installViewer();

	QList<OrthogonalViewer*>* viewers;
	QList<QDockWidget*>* dockers;
	QSignalMapper* mapper;

};

}
}
#endif // !__QVTK_ORTHOGONAL_DOCK_VIEWERS_H__
