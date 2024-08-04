#include "algorithmvisualizermainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AlgorithmVisualizerMainWindow w;
    w.show();
    return a.exec();
}
