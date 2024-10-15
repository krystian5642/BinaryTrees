
#ifndef ALGORITHMVISUALIZERMAINWINDOW_H
#define ALGORITHMVISUALIZERMAINWINDOW_H

#include "binarytreebase.h"

#include <QLabel>
#include <QMainWindow>
#include <QPainter>
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
    void on_clearButton_clicked();
    void on_randomFillButton_clicked();

    void on_treeNameBox_currentTextChanged(const QString &treeName);

private:
    Ui::AlgorithmVisualizerMainWindow *ui;
    std::unique_ptr<BinaryTreeBase<int>> binaryTree;

    void redrawBinaryTree(QPainter& painter);
    void drawBinaryTreeNode(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node, const QPoint &location, QPainter& painter);

    void drawBinaryTreeNodeRec(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node, const QPoint &location, QPainter& painter);

    void updateBinaryTreeProperties();

    std::unique_ptr<BinaryTreeBase<int>> createTree(const QString &treeName);

    void resetNodeLoc(shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node);
    void calculateInitialX(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node);
    void calculateInitialXX(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node, float cumMod = 0.f);
    void calculateInitialXXX(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node);

    float getMidpointOfChildren(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node);

    // left (Node): left subtree, with right contour to be traversed
    // right (Node): right subtree, with left contour to be traversed
    // leftCumShift : cumulative `mod + shift` for left subtree from the ancestors, defaults to 0
    // rightCumShift : cumulative `mod + shift` for right subtree from the ancestors, defaults to 0
    // cumShift : cumulative shift amount for right subtree, defaults to 0
    // initialRun : indicates whether left subtree and right subtree are the main subtrees, defaults to True
    float getSubtreeShift(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> left, const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> right
                          , float leftCumShift = 0, float rightCumShift = 0, float cumShift = 0, bool initialRun = true) const;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // ALGORITHMVISUALIZERMAINWINDOW_H
