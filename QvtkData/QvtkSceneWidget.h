#ifndef __Qvtk_SCENE_WIDGET_H__
#define __Qvtk_SCENE_WIDGET_H__

// me
#include "abstractdata_export.h"
namespace Ui {
	class QvtkSceneWidget;
};
class Data;

// qt
#include <QWidget>
class QListWidgetItem;

class ABSTRACTDATA_EXPORT QvtkSceneWidget : public QWidget
{
	Q_OBJECT;
public:
	explicit QvtkSceneWidget(QWidget* parent = nullptr);
	virtual ~QvtkSceneWidget() override;

public slots:

	void slotSceneReloaded();
	void slotSceneAddedData(QString uniqueName);
	void slotSceneRemoveData(QString uniqueName);
	void slotSceneClicked(QListWidgetItem* item);
	void slotSceneClicked(QString uniqueName);

private:
	Ui::QvtkSceneWidget* ui;
	static QString uniqueNameToItemName(QString uniqueName);
	static QString itemNameToUniqueName(QString uniqueName);

	Data* tmpData;
};


#endif // !__Qvtk_SCENE_WIDGET_H__
