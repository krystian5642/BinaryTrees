#include "algorithmvisualizermainwindow.h"
#include "./ui_algorithmvisualizermainwindow.h"

#include "binaryheap.h"

AlgorithmVisualizerMainWindow::AlgorithmVisualizerMainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::AlgorithmVisualizerMainWindow())
    , binaryTree(std::make_unique<BinaryHeap<int>>())
{
    ui->setupUi(this);
}

AlgorithmVisualizerMainWindow::~AlgorithmVisualizerMainWindow()
{
    delete ui;
}

void AlgorithmVisualizerMainWindow::on_addValueButton_clicked()
{
    const auto value = ui->addValueText->text().toInt(nullptr, 0);
    if (binaryTree->add(value))
    {
        redrawBinaryTree();
    }
}

void AlgorithmVisualizerMainWindow::on_removeValueButton_clicked()
{
    const auto value = ui->removeValueText->text().toInt(nullptr, 0);
    if (binaryTree->remove(value))
    {
        redrawBinaryTree();
    }
}

void AlgorithmVisualizerMainWindow::redrawBinaryTree()
{
    foreach (auto valueLabel, binaryTreeValueLabels)
    {
        delete valueLabel;
    }
    binaryTreeValueLabels.clear();

    const auto renderAreaSize = ui->renderArea->size();
    drawBinaryTreeNode(binaryTree->getRoot(), QPoint{renderAreaSize.width() / 2, 20});
}

void AlgorithmVisualizerMainWindow::drawBinaryTreeNode(const shared_ptr<const BinaryTreeBase<int>::BinaryTreeNode> node, const QPoint &location)
{
    if (node == binaryTree->getLeafNode())
    {
        return;
    }

    constexpr int offset = 60;

    drawBinaryTreeNode(node->getLeft(), QPoint(location.x() - offset * 2, location.y() + offset));

    auto textItem = new QLabel(QString::number(node->getValue()), ui->renderArea);
    textItem->setFont(QFont("Arial", 24, QFont::Bold));
    textItem->move(location);
    textItem->show();

    textItem->setStyleSheet(QString("QLabel {background-color: %1}").arg(node->getColor().name()));
    binaryTreeValueLabels.push_back(textItem);

    drawBinaryTreeNode(node->getRight(), QPoint(location.x() + offset * 2, location.y() + offset));
}
