//
//  RabbitMatrix.cpp
//  FindRabbit
//
//  Created by yangjh on 15/4/24.
//
//

#include "RabbitMatrix.h"

bool RabbitMatrix::init()
{
    if (!Node::init()) {
        return false;
    }
    // 兔子单元矩阵
    for (int r = 0; r < MAX_SIZE; r++) {
        for (int c = 0; c < MAX_SIZE; c++) {
            m_rabbitCell[r][c] = RabbitCell::create();
            this->addChild(m_rabbitCell[r][c], 1);
            // 暂时不显示
            m_rabbitCell[r][c]->setVisible(false);
        }
    }
    
    m_canCatch = true;
    // 重置兔子单元矩阵
    this->resetRabbitMatrix();
    return true;
}

void RabbitMatrix::resetRabbitMatrix() {
    m_level = 1;
    // 显示兔子单元矩阵
    this->showRabbitMatrix();
}

CatchRabbitResult RabbitMatrix::catchRabbit(const Point &p)
{
    if (!m_canCatch) {
        return CatchRabbitResult_DotCatch;
    }
    // 遍历查看点到哪个兔子单元了
    for (int r = 0; r < MAX_SIZE; r++) {
        for (int c = 0; c < MAX_SIZE; c++) {
            // 未显示的不往下遍历了
            if (!m_rabbitCell[r][c]->isVisible()) {
                break;
            }
            // 计算兔子单元矩阵的Rect
            Vec2 pos = m_rabbitCell[r][c]->getPosition();
            float size = m_rabbitCell[r][c]->getCellSize();
            Rect rect = Rect(pos.x-size/2, pos.y-size/2, size, size);
            // 点到当前兔子单元
            if (rect.containsPoint(p)) {
                // 当前兔子单元里是兔子
                if (m_rabbitCell[r][c] == m_rabbit) {
                    // 下一关
                    m_level++;
                    // 动画隐藏，换关卡后，动画显示
                    this->runAction(Sequence::create(CallFunc::create([=]{this->hideRabbitMatrix();}), DelayTime::create(0.15), CallFunc::create([=]{this->showRabbitMatrix();}), NULL));
                    return CatchRabbitResult_Success;
                }
                // 当前兔子单元里不是兔子
                else {
                    // 兔子单元闪烁
                    m_rabbit->runAction(Blink::create(0.7, 5));
                    return CatchRabbitResult_Failure;
                }
            }
        }
    }
    return CatchRabbitResult_Invalid;
}

int RabbitMatrix::getMatrixSizeShow() {
    // 根据关卡计算兔子单元显示密度
    int matrixSize_Show = 0;
    int levelSum = 0;
    for (int size = 2; size < MAX_SIZE; size++) {
        if (m_level > levelSum && m_level <= levelSum+size) {
            matrixSize_Show = size;
            break;
        }
        levelSum += size;
    }
    if (matrixSize_Show < 2) {
        matrixSize_Show = MAX_SIZE;
    }
    return matrixSize_Show;
}

void RabbitMatrix::hideRabbitMatrix()
{
    // 开始动画不能点
    m_canCatch = false;
    // 遍历兔子单元，把显示着的缩小
    for (int r = 0; r < MAX_SIZE; r++) {
        for (int c = 0; c < MAX_SIZE; c++) {
            // 未显示的不往下遍历了
            if (!m_rabbitCell[r][c]->isVisible()) {
                break;
            }
            // 动画隐藏
            ScaleTo* scale0 = ScaleTo::create(0.05, 1.03);
            ScaleTo* scale1 = ScaleTo::create(0.1, 0.0);
            m_rabbitCell[r][c]->runAction(Sequence::create(scale0, scale1, NULL));
        }
    }
}

void RabbitMatrix::showRabbitMatrix() {
    // 开始动画不能点
    m_canCatch = false;
    // 获取兔子单元显示密度
    int matrixSize_Show = this->getMatrixSizeShow();
    // 随机生成兔子的坐标
    struct timeval tv;
    gettimeofday(&tv,NULL);
    srand(tv.tv_usec);
    int rRabbit = rand()%matrixSize_Show;
    int cRabbit = rand()%matrixSize_Show;
    float rabbitSize = RABBITMATRIX_SIZE/matrixSize_Show;
    CallFunc* callFunc = CallFunc::create([=]{ m_canCatch = true; });
    // 显示兔子单元
    for (int r = 0; r < MAX_SIZE; r++) {
        for (int c = 0; c < MAX_SIZE; c++) {
            if (r < matrixSize_Show && c < matrixSize_Show) {
                m_rabbitCell[r][c]->setVisible(true);
                Point pos = Point(-RABBITMATRIX_SIZE/2+r*rabbitSize+rabbitSize/2,
                                  -RABBITMATRIX_SIZE/2+c*rabbitSize+rabbitSize/2);
                m_rabbitCell[r][c]->setPosition(pos);
                m_rabbitCell[r][c]->setVisible(true);
                m_rabbitCell[r][c]->setRabbitAndSize(r==rRabbit&&c==cRabbit, 400/matrixSize_Show);
                m_rabbitCell[r][c]->setScale(0.0f);
                // 动画显示
                ScaleTo* scale0 = ScaleTo::create(0.1, 1.03);
                ScaleTo* scale1 = ScaleTo::create(0.05, 1.0f);
                if (callFunc != nullptr) {
                    m_rabbitCell[r][c]->runAction(Sequence::create(scale0, scale1, callFunc, NULL));
                    callFunc = nullptr;
                }
                else {
                    m_rabbitCell[r][c]->runAction(Sequence::create(scale0, scale1, NULL));
                }
            }
            else {
                m_rabbitCell[r][c]->setVisible(false);
            }
        }
    }
    m_rabbit = m_rabbitCell[rRabbit][cRabbit];
}
