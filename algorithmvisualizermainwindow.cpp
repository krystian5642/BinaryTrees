#include "algorithmvisualizermainwindow.h"
#include "./ui_algorithmvisualizermainwindow.h"

AlgorithmVisualizerMainWindow::AlgorithmVisualizerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(std::make_unique<Ui::AlgorithmVisualizerMainWindow>())
{
    ui->setupUi(this);
}

AlgorithmVisualizerMainWindow::~AlgorithmVisualizerMainWindow()
{
}
