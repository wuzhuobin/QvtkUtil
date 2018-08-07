/*!
 * \file Test_MainWindow.h
 * \date 2017/07/14 16:45
 *
 * \author 		Wong, Matthew Lun
 * Occupation	Chinese University of Hong Kong, 
 * 				Department of Imaging and Interventional Radiology,
 * 				M.Phil Student
 * Contact: 	fromosia@gmail.com
 *
 * \brief  This is a simple Mainwindow Demo written to test and demonstrate
 *         the functionalities of SCBViewer class
 *
 *
 * \note
*/
#ifndef __SCB_TEST_MAINWINDOW_H__
#define __SCB_TEST_MAINWINDOW_H__


//qt
#include <QObject>
#include <QMainWindow>



#include "ui_Test_MainWindow.h"

/// Maniwindow for test
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow() {};
 
    Ui_MainWindow* m_ui;

signals:

public slots :
    void slotPrepareViewers();
    void slotDisplayCube();
	void slotDisplayAxisActor();
    void slotSwitchInteractor();
    void slotDesyncUpperLeftViewer();
};

#endif //__SCB_TEST_MAINWINDOW_H__