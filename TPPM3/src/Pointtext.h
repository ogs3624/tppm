//
//  Pointtext.h
//  pmApp_receiver
//
//  Created by kayayose on 2014/06/11.
//
//

#pragma once
#include "ofMain.h"

class Pointtext {
    
public:
    Pointtext(ofPoint pos,int dim,int de,int poi); //コンストラクタ
    void update();
    void draw();
    void setcolor_ring(ofColor col);
    
    int dimention;
    int dev;
    
    ofPoint pos; //輪の中心位置
    float radius; //輪の半径
    float radiusSpeed; //輪の拡大スピード
    bool dead; //生死の判定
    ofColor ring_color;
    
    int point;
    
private:
    
};





