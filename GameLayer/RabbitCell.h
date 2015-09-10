//
//  RabbitCell.h
//  FindRabbit
//
//  Created by yangjh on 15/4/24.
//
//

#ifndef __FindRabbit__RabbitCell__
#define __FindRabbit__RabbitCell__

#include <stdio.h>

USING_NS_CC;

class RabbitCell : public Node
{
public:
    virtual bool init();
    CREATE_FUNC(RabbitCell);
    
    void setRabbitAndSize(bool rabbit, float size);
    float getCellSize() { return m_cellSize; }
    
private:
    float m_cellSize;
    Sprite *m_rabbitNest;
    Label *m_rabbitLabel;
};

#endif /* defined(__FindRabbit__RabbitCell__) */
