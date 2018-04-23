#ifndef __Qvtk_SCENE_WIDGET_H__
#define __Qvtk_SCENE_WIDGET_H__

#include "qvtkdata_export.h"
// me

namespace Q {
namespace vtk{
	class Data;
	namespace Ui {
		class SceneWidget;
};
}
}

// qt
#include <QWidget>
class QListWidgetItem;

namespace Q {
namespace vtk{
class QVTKDATA_EXPORT SceneWidget : public QWidget
{
	Q_OBJECT;
public:
	explicit SceneWidget(QWidget* parent = nullptr);
	virtual ~SceneWidget() override;

public slots:

	void slotSceneReloaded();
	void slotSceneAddedData(QString uniqueName);
	void slotSceneRemoveData(QString uniqueName);
	void slotSceneClicked(QListWidgetItem* item);
	void slotSceneClicked(QString uniqueName);

private:
	Ui::SceneWidget* ui;
	static QString uniqueNameToItemName(QString uniqueName);
	static QString itemNameToUniqueName(QString uniqueName);

	Data* tmpData;
};

}
}
#endif // !__Qvtk_SCENE_WIDGET_H__
