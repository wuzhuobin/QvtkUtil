#ifndef __Qvtk_ABSTRACT_MAIN_WINDOW_H__
#define __Qvtk_ABSTRACT_MAIN_WINDOW_H__

#include "abstractmainwindow_export.h"

#include <QMainWindow>
#include <QMap>

class QvtkAbstractInteractorObserver;
class QvtkAbstractPipeline;
class Ui_MainWindow;
namespace Ui { class MainWindow;}

class ABSTRACTMAINWINDOW_EXPORT QvtkAbstractMainWindow : public QMainWindow
{
    Q_OBJECT
public:
	~QvtkAbstractMainWindow() { instance = nullptr; }

	/**
	 * QvtkAbstractMainWindow* Get
	 *
	 * @brief
	 *
	 * Static method to get the Mainwindow
	 *
	 * @return QvtkAbstractMainWindow*
	 */
    static QvtkAbstractMainWindow* Get();


    /**
     * QvtkAbstractMainWindow* New
     *
     * @brief
     *
     * This method must be reimplmented in subclasses
     *
     * @return QvtkAbstractMainWindow*
     */
    static QvtkAbstractMainWindow* New();

    /**
     * void Initialize
     *
     * @brief
     *
     * Always call this after New()
     *
     * @return void
     */
    virtual void Initialize();


	/**
	 * QT_NAMESPACE::Ui_MainWindow* GetUI
	 *
	 * @brief
	 *
	 * Return the ui of the mainwindow
	 *
	 * @return QT_NAMESPACE::Ui_MainWindow*
	 */
	virtual Ui_MainWindow* GetUI();

	Ui_MainWindow* ui;

public slots:
    virtual void slotActionBarHandle();
    virtual void slotChangeInteractorStyle();
    virtual void slotChangePipeline();
    virtual void slotTogglePanelHide();
    virtual void slotTogglePanelDock();

protected:
    virtual void ConnectUI();

    virtual int SetUpViewers() = 0;

    virtual void AddInteractorStyle(QvtkAbstractInteractorObserver* obj, QString objName = "");
    virtual void AddPipline(QvtkAbstractPipeline* obj, QString objName = "");

    explicit QvtkAbstractMainWindow(QObject* parent=nullptr);

	static QvtkAbstractMainWindow* instance;

    QMap<QAction*, QvtkAbstractPipeline*>            mapActionToPipeline;
    QMap<QAction*, QvtkAbstractInteractorObserver* > mapActionToInteractorObserver;
	QList<QAction*> menuBarSeperators;
};



#endif