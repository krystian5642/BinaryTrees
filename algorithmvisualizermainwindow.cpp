#include "algorithmvisualizermainwindow.h"
#include "./ui_algorithmvisualizermainwindow.h"

#include <QPainter>
#include<unordered_map>

#include "balancedbinarytree.h"
#include "binaryheap.h"
#include "redblacktree.h"

AlgorithmVisualizerMainWindow::AlgorithmVisualizerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AlgorithmVisualizerMainWindow())
{
    ui->setupUi(this);

    binaryTree = createTree(ui->treeNameBox->currentText());
    updateBinaryTreeProperties();
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
        //redrawBinaryTree();
        updateBinaryTreeProperties();
    }
}

void AlgorithmVisualizerMainWindow::on_removeValueButton_clicked()
{
    const auto value = ui->removeValueText->text().toInt(nullptr, 0);
    if (binaryTree->remove(value))
    {
        //redrawBinaryTree();
        updateBinaryTreeProperties();
    }
}

void AlgorithmVisualizerMainWindow::on_clearButton_clicked()
{
    binaryTree = createTree(ui->treeNameBox->currentText());
    //redrawBinaryTree();
    updateBinaryTreeProperties();
}

void AlgorithmVisualizerMainWindow::on_randomFillButton_clicked()
{
    binaryTree = createTree(ui->treeNameBox->currentText());
    binaryTree->randomFill();
    //redrawBinaryTree();
    updateBinaryTreeProperties();
}

void AlgorithmVisualizerMainWindow::on_treeNameBox_currentTextChanged(const QString &treeName)
{
    binaryTree = createTree(treeName);
    //redrawBinaryTree();
    updateBinaryTreeProperties();
}

void AlgorithmVisualizerMainWindow::redrawBinaryTree(QPainter& painter)
{
    drawBinaryTreeNode(binaryTree->getRoot(), QPoint(600, 90), painter);
}

void AlgorithmVisualizerMainWindow::drawBinaryTreeNode(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node, const QPoint &location, QPainter& painter)
{
    if (!binaryTree->isNodeValid(node))
    {
        return;
    }

    // Reset value
    resetNodeLoc(node);

    // Calculate initial X
    calculateInitialX(node);
    calculateInitialXX(node);
    calculateInitialXXX(node);

    drawBinaryTreeNodeRec(node, QPoint(location.x() + node->x * 70, location.y()), painter);
}

void AlgorithmVisualizerMainWindow::drawBinaryTreeNodeRec(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node, const QPoint &location, QPainter& painter)
{
    if(binaryTree->isNodeValid(node->getLeft()))
    {
        auto newLocX = 600 + 70 * node->getLeft()->x;
        drawBinaryTreeNodeRec(node->getLeft(), QPoint(newLocX, location.y() + 60), painter);
    }

    if(node->getParent())
    {
        painter.drawLine(QPoint(location.x() + 15, location.y() + 15), QPoint(600 + 70 * node->getParent()->x + 15, location.y() - 45));
    }

    painter.drawEllipse(location.x(), location.y(), 30, 30);
    painter.drawText(location, QString::number(node->getValue()));

    if(binaryTree->isNodeValid(node->getRight()))
    {
        auto newLocXX =  600 + 70 * node->getRight()->x;
        drawBinaryTreeNodeRec(node->getRight(), QPoint(newLocXX, location.y() + 60), painter);
    }
}

void AlgorithmVisualizerMainWindow::updateBinaryTreeProperties()
{
    // // clear layout
    // while(auto* layoutItem = ui->propertiesVerticleBox->takeAt(0))
    // {
    //     if(auto* widget = layoutItem->widget())
    //     {
    //         widget->deleteLater();
    //     }
    // }

    // ui->propertiesVerticleBox->setDirection(QBoxLayout::Direction::BottomToTop);
    // ui->propertiesVerticleBox->addStretch(1);

    // std::unordered_map<std::string, int> binaryTreeProperties;
    // binaryTree->buildProperties(binaryTreeProperties);

    // for(const auto& [propertyName, propertyValue] : binaryTreeProperties)
    // {
    //     QLabel* propertyLabel = new QLabel(QString(QString::fromStdString(propertyName) + "     :      %1").arg(propertyValue));
    //     ui->propertiesVerticleBox->addWidget(propertyLabel);
    //     ui->propertiesVerticleBox->setAlignment(propertyLabel, Qt::AlignmentFlag::AlignTop);
    // }

    // ui->binaryTreePropertiesBox->setLayout(ui->propertiesVerticleBox);
}

std::unique_ptr<BinaryTreeBase<int>> AlgorithmVisualizerMainWindow::createTree(const QString &treeName)
{
    if(treeName == "Binary Search Tree")
    {
        return std::make_unique<BinarySearchTree<int>>();
    }

    if(treeName == "AVL Tree")
    {
        return std::make_unique<BalancedBinaryTree<int>>();
    }

    if(treeName == "Red Black Tree")
    {
        return std::make_unique<RedBlackTree<int>>();
    }

    if(treeName == "Heap")
    {
        return std::make_unique<BinaryHeap<int>>();
    }

    return nullptr;
}

void AlgorithmVisualizerMainWindow::resetNodeLoc(shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node)
{
    if(!binaryTree->isNodeValid(node))
    {
        return;
    }

    resetNodeLoc(node->getLeft());

    node->x = 0;
    node->shift = 0;
    node->mod = 0;

    resetNodeLoc(node->getRight());
}

void AlgorithmVisualizerMainWindow::calculateInitialX(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node)
{
    if (!binaryTree->isNodeValid(node))
    {
        return;
    }

    calculateInitialX(node->getLeft());
    calculateInitialX(node->getRight());

    const auto left = binaryTree->isNodeValid(node->getParent()) ? node->getParent()->getLeft() : nullptr;
    const auto right = binaryTree->isNodeValid(node->getParent()) ? node->getParent()->getRight() : nullptr;

    if(node == binaryTree->getRoot())
    {
        node->x = getMidpointOfChildren(node);
        return;
    }

    if(right == node)
    {
        right->x = binaryTree->isNodeValid(left) ? left->x + 1 : 1;

        if(binaryTree->isNodeValid(right->getLeft()) || binaryTree->isNodeValid(right->getRight()))
        {
            right->mod = right->x - getMidpointOfChildren(node);
        }
    }
    else if(left == node)
    {
        left->x = getMidpointOfChildren(node);
    }

    if(right == node && binaryTree->isNodeValid(left))
    {
        right->shift = getSubtreeShift(left, right);
    }
}

void AlgorithmVisualizerMainWindow::calculateInitialXX(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node, float cumMod)
{
    if (!binaryTree->isNodeValid(node))
    {
        return;
    }

    node->x += node->shift + cumMod + 1;

    calculateInitialXX(node->getLeft(), cumMod + node->mod + node->shift);
    calculateInitialXX(node->getRight(), cumMod + node->mod + node->shift);
}

void AlgorithmVisualizerMainWindow::calculateInitialXXX(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node)
{
    // for later
}

float AlgorithmVisualizerMainWindow::getMidpointOfChildren(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> node)
{
    if(!binaryTree->isNodeValid(node))
    {
        return 0.f;
    }

    auto left = node->getLeft();
    auto right = node->getRight();

    if(!left && !right)
    {
        if(node->getParent())
        {
            if(node->getParent()->getLeft())
            {
                return -1;
            }
            else
            {
                return 1;
            }
        }

        return 0.f;
    }

    if(left && !right)
    {
        return left->x + left->shift + 1;
    }

    if(!left && right)
    {
        return right->x + right->shift - 1;
    }

    return (left->x + left->shift + right->x + right->shift) / 2.f;

}

float AlgorithmVisualizerMainWindow::getSubtreeShift(const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> left, const shared_ptr<BinaryTreeBase<int>::BinaryTreeNode> right
                                                     , float leftCumShift, float rightCumShift, float cumShift, bool initialRun) const
{
    float newShift = 0.f;

    if(!initialRun)
    {
        float xLeft = left->x + left->shift + leftCumShift;
        float xRight = right->x + right->shift + rightCumShift + cumShift;
        newShift = std::max(0.f, xLeft + 1 - xRight);
    }

    bool hasLeftChildren = binaryTree->isNodeValid(left->getLeft()) || binaryTree->isNodeValid(left->getRight());
    bool hasRightChildren = binaryTree->isNodeValid(right->getLeft()) || binaryTree->isNodeValid(right->getRight());

    if(hasLeftChildren && hasRightChildren)
    {
        return getSubtreeShift(binaryTree->isNodeValid(left->getRight()) ? left->getRight() : left->getLeft()
                               , binaryTree->isNodeValid(right->getLeft()) ? right->getLeft() : right->getRight(), leftCumShift + left->mod + left->shift
                               , rightCumShift + right->mod + right->shift, cumShift + newShift, false);
    }

    return cumShift + newShift;
}

void AlgorithmVisualizerMainWindow::paintEvent(QPaintEvent *event)
{
    QPen pen;
    pen.setWidth(5);

    QPainter painter(this);
    painter.setPen(pen);

    redrawBinaryTree(painter);
    update();
}





















