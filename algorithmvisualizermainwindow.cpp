#include "algorithmvisualizermainwindow.h"
#include "./ui_algorithmvisualizermainwindow.h"
#include "binarysearchtree.h"

#include <QLabel>

AlgorithmVisualizerMainWindow::AlgorithmVisualizerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AlgorithmVisualizerMainWindow())
    , binaryTree(std::make_unique<BinarySearchTree<int>>())
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
    if(binaryTree->add(value))
    {
        redrawBinaryTree();
    }
}

void AlgorithmVisualizerMainWindow::on_removeValueButton_clicked()
{
    const auto value = ui->removeValueText->text().toInt(nullptr, 0);
    if(binaryTree->remove(value))
    {
        redrawBinaryTree();
    }
}

void AlgorithmVisualizerMainWindow::redrawBinaryTree()
{
    foreach(auto valueLabel, binaryTreeValueLabels)
    {
        delete valueLabel;
    }
    binaryTreeValueLabels.clear();

    const auto renderAreaSize = ui->renderArea->size();
    drawBinaryTreeNode(binaryTree->getRoot(), QPoint{renderAreaSize.width() / 2, 20});
}

void AlgorithmVisualizerMainWindow::drawBinaryTreeNode(BinaryTreeBase<int>::node *node, const QPoint& Location)
{
    if(!node)
    {
        return;
    }

    constexpr int offset = 60;

    drawBinaryTreeNode(node->left.get(), QPoint(Location.x() - offset * 2, Location.y() + offset));

    auto textItem = new QLabel(QString::number(node->value), ui->renderArea);
    const QFont font("Arial", 24, QFont::Bold);
    textItem->setFont(font);
    textItem->move(Location);
    textItem->show();
    binaryTreeValueLabels.push_back(textItem);

    drawBinaryTreeNode(node->right.get(), QPoint(Location.x() + offset * 2, Location.y() + offset));
}







