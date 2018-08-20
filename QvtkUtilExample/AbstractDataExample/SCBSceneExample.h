#ifndef __SCB_SCENE_EXAMPLE_H__
#define __SCB_SCENE_EXAMPLE_H__

#include <QMainWindow>

class SCBAbstractDataModel;


namespace Ui{ class SCBSceneExample; }

class  SCBSceneExample: public QMainWindow
{
	Q_OBJECT;
public:
	SCBSceneExample(QWidget* parent = nullptr);
	~SCBSceneExample();
private slots:

	void slotPushButton();
	void slotImport();
	void slotCommit();
	void slotPrint();
private:
	Ui::SCBSceneExample* m_ui;
};

#endif // !__SCB_SCENE_EXAMPLE_H__
