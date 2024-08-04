
#ifndef ALGORITHMVISUALIZERMAINWINDOW_H
#define ALGORITHMVISUALIZERMAINWINDOW_H

#include "binarytreebase.h"

#include <QLabel>
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

private slots:
    void on_addValueButton_clicked();
    void on_removeValueButton_clicked();

private:
    Ui::AlgorithmVisualizerMainWindow* ui;
    std::unique_ptr<BinaryTreeBase<int>> binaryTree;
    QVector<QLabel*> binaryTreeValueLabels;

    void redrawBinaryTree();
    void drawBinaryTreeNode(BinaryTreeBase<int>::node* node, const QPoint& Location);
};

#endif //ALGORITHMVISUALIZERMAINWINDOW_H
