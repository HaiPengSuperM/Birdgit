//
//  RabbitCell.cpp
//  FindRabbit
//
//  Created by yangjh on 15/4/24.
//
//

#include "RabbitCell.h"

bool RabbitCell::init()
{
    if (!Node::init()) {
        return false;
    }
    m_cellSize = 0.0f;
    // 兔子窝
    m_rabbitNest = Sprite::create("images/rabbitnest.png");
    this->addChild(m_rabbitNest);
    // 文本
    m_rabbitLabel = Label::createWithSystemFont("", "", 20);
    this->addChild(m_rabbitLabel, 0);
    return true;
}

void RabbitCell::setRabbitAndSize(bool rabbit, float size) {
    m_cellSize = size;
    // 显示兔子窝和文字
    m_rabbitNest->setScale(m_cellSize/96);
    m_rabbitLabel->setString(rabbit?"兔":"免");
    m_rabbitLabel->setSystemFontSize(0.95*m_cellSize);
}
