//
//  GameLayer.h
//  FindRabbit
//
//  Created by yangjh on 15/4/23.
//
//

#ifndef __FindRabbit__GameLayer__
#define __FindRabbit__GameLayer__

#include <stdio.h>
#include "RabbitMatrix.h"

USING_NS_CC;

class GameLayerDelegate
{
public:
    virtual void gameLayerGameOver(int nowScore, int bestScore) = 0;
};

class GameLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(GameLayer);
    
    // 点击
    virtual bool onTouchBegan(Touch* touch,Event* event);
    
    // 设置代理
    void setDelegate(GameLayerDelegate *delegate) { m_delegate = delegate; }
    // 开始游戏
    void startGame();
    
private:
    // 游戏结束
    void gameOver();
    
private:
    // 回调
    GameLayerDelegate *m_delegate;
    
    Label *m_labelHighScore;
    Label *m_labelLevel;
    // 兔子矩阵
    RabbitMatrix *m_rabbitMatrix;
    
    bool m_gameRun;
};

#endif /* defined(__FindRabbit__GameLayer__) */
