#include <QApplication>


#include "Test_Pipeline_MainWindow.h"
#include "SCBViewer.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow* mainwin = new MainWindow;
    mainwin->show();

    return app.exec();
}
