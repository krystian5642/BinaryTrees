
#pragma once

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class AlgorithmVisualizerMainWindow;
}
QT_END_NAMESPACE

class AlgorithmVisualizerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AlgorithmVisualizerMainWindow(QWidget *parent = nullptr);
    ~AlgorithmVisualizerMainWindow();

private:
    std::unique_ptr<Ui::AlgorithmVisualizerMainWindow> ui;
};
