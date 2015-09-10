//
//  GameLayer.cpp
//  FindRabbit
//
//  Created by yangjh on 15/4/23.
//
//

#include "GameLayer.h"

#define Key_HighScore   "Key_HighScore"

bool GameLayer::init(){
    if(!Layer::init()){
        return false;
    }
    m_gameRun = true;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 最佳成绩
    m_labelHighScore = Label::createWithSystemFont("", "", 20);
    m_labelHighScore->setPosition(visibleSize.width-120, visibleSize.height-30);
    m_labelHighScore->setWidth(200);
    m_labelHighScore->setAlignment(TextHAlignment::RIGHT);
    m_labelHighScore->setTextColor(Color4B(222, 0, 53, 255));
    this->addChild(m_labelHighScore);
    // 关卡提示
    m_labelLevel = Label::createWithSystemFont("", "", 50);
    m_labelLevel->setPosition(visibleSize.width/2, visibleSize.height/2+RabbitMatrix::RABBITMATRIX_SIZE/2+60);
    this->addChild(m_labelLevel);
    // 兔子矩阵
    m_rabbitMatrix = RabbitMatrix::create();
    m_rabbitMatrix->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    this->addChild(m_rabbitMatrix);
    
    // 点击监听
    EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan,this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);
    
    m_labelLevel->setString("第1关");
    m_labelHighScore->setVisible(false);
    // 最佳成绩
    int highScore = UserDefault::getInstance()->getIntegerForKey(Key_HighScore, 0);
    if (UserDefault::getInstance()->getIntegerForKey(Key_HighScore, 0) > 0) {
        m_labelHighScore->setVisible(true);
        m_labelHighScore->setString(StringUtils::format("最佳成绩 %d 只", highScore));
    }
    return true;
}

bool GameLayer::onTouchBegan(Touch* touch,Event* event)
{
    // 游戏结束后不再接收游戏点击
    if (!m_gameRun) {
        return true;
    }
    
    // 计算点击的坐标
    Point p = touch->getLocationInView();
    p = Director::getInstance()->convertToGL(p);
    // 计算兔子单元矩阵的Rect
    Vec2 pos = m_rabbitMatrix->getPosition();
    switch (m_rabbitMatrix->catchRabbit(Vec2(p.x-pos.x, p.y-pos.y))) {
        case CatchRabbitResult_Success:
        {
            // 更新关卡文本
            ScaleTo* scale0 = ScaleTo::create(0.15, 0.0f);
            CallFunc* callFunc = CallFunc::create([=]{
                // 更新关卡文本
                int currentLevel = m_rabbitMatrix->getCurrentLevel();
                m_labelLevel->setString(StringUtils::format("第%d关", currentLevel));
            });
            ScaleTo* scale1 = ScaleTo::create(0.15, 1.0f);
            m_labelLevel->runAction(Sequence::create(scale0, callFunc, scale1, NULL));
        }
            break;
        case CatchRabbitResult_Failure:
        {
            m_gameRun = false;
            // 游戏结束
            CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(GameLayer::gameOver,this));
            this->runAction(Sequence::create(DelayTime::create(1.1f), callFunc, NULL));
        }
            break;
        default:
            break;
    }
    return true;
}

void GameLayer::startGame()
{
    m_labelLevel->setString("第1关");
    m_rabbitMatrix->resetRabbitMatrix();
    m_rabbitMatrix->runAction(ScaleTo::create(0.3, 1));
    
    m_gameRun = true;
}

void GameLayer::gameOver()
{
    // 当前得分
    int nowScore = m_rabbitMatrix->getCurrentScore();
    // 历史最佳纪录
    int bestScore = UserDefault::getInstance()->getIntegerForKey(Key_HighScore, 0);
    // 保存游戏纪录
    if (nowScore > bestScore) {
        // 更新最佳成绩
        m_labelHighScore->setVisible(true);
        m_labelHighScore->setString(StringUtils::format("最佳成绩 %d 只", nowScore));
        // 保存最佳成绩
        UserDefault::getInstance()->setIntegerForKey(Key_HighScore, nowScore);
        UserDefault::getInstance()->flush();
    }
    // 隐藏兔子矩阵，然后游戏结束
    m_rabbitMatrix->runAction(Sequence::create(ScaleTo::create(0.21, 0.0f), CallFunc::create([=]{
        // 游戏结束
        m_delegate->gameLayerGameOver(nowScore, bestScore);
    }), NULL));
}
