#ifndef __Qvtk_ORTHOGONAL_DOCK_VIEWERS_H__
#define __Qvtk_ORTHOGONAL_DOCK_VIEWERS_H__

// me
class OrthogonalViewer;
#include "abstractviewer_export.h"

// qt 
#include <QMainWindow>
template <typename T>
class QList;
class QDockWidget;
class QSignalMapper;


class ABSTRACTVIEWER_EXPORT QvtkOrthogonalDockViewers : public QMainWindow
{
	Q_OBJECT;
public: 
	explicit QvtkOrthogonalDockViewers(QWidget* parent = nullptr);
	virtual ~QvtkOrthogonalDockViewers() override;

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

#endif // !__Qvtk_ORTHOGONAL_DOCK_VIEWERS_H__
