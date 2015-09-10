//
//  RabbitMatrix.h
//  FindRabbit
//
//  Created by yangjh on 15/4/24.
//
//

#ifndef __FindRabbit__RabbitMatrix__
#define __FindRabbit__RabbitMatrix__

#include <stdio.h>
#include "RabbitCell.h"

USING_NS_CC;

typedef enum {
    CatchRabbitResult_Success = 1,
    CatchRabbitResult_DotCatch,
    CatchRabbitResult_Failure,
    CatchRabbitResult_Invalid,
}CatchRabbitResult;

class RabbitMatrix : public Node
{
public:
    virtual bool init();
    CREATE_FUNC(RabbitMatrix);
    
    int getCurrentScore() { return m_level-1; }
    int getCurrentLevel() { return m_level; }
    void resetRabbitMatrix();
    CatchRabbitResult catchRabbit(const Point& p);
    
public:
    constexpr const static float RABBITMATRIX_SIZE = 400;
    const static int MAX_SIZE = 7;
    
private:
    int getMatrixSizeShow();
    void hideRabbitMatrix();
    void showRabbitMatrix();
    
private:
    int m_level;
    bool m_canCatch;
    RabbitCell *m_rabbit;
    RabbitCell *m_rabbitCell[MAX_SIZE][MAX_SIZE];
};

#endif /* defined(__FindRabbit__RabbitMatrix__) */
