//
//  CustomCircle.cpp
//  pmApp_receiver
//
//  Created by kayayose on 2014/07/07.
//
//

#include "SakuraCircle.h"


SakuraCircle::SakuraCircle(ofImage img)
{
    //現在の年齢
    counter = 0;
    //初期位相
    phase = ofRandom(0, PI*2);
    //寿命を設定
    lifeTime = 27;
    //生死の判定
    dead = false;
    sakuraimg=img;
    deg=0;
    degplus=ofRandom(-5,5);
}

void SakuraCircle::update()
{
    //寿命が尽きたら死亡
    if (counter > lifeTime) {
        dead = true;
    }//年齢を追加
    counter++;
    deg+=degplus;
}

void SakuraCircle::draw()
{
    //半径を取得
    float radius = getRadius();
    float r = abs(sin(counter * 0.03 + phase)) * radius + radius * 0.25;
    //座標を変更
    glPushMatrix();
    //物体の位置に座標を移動
    glTranslatef(getPosition().x, getPosition().y, 0);
    ofFill(); //色を塗り潰す
    //パーティクルを描く
    //ofSetColor(255, 255, 255);
    //ofCircle(0, 0, r * 0.25);
    ofRotateZ(deg);
    if(counter<15){
        ofSetColor(255, 255, 255,255);
    }else{
        ofSetColor(255, 255, 255,255-ofMap(counter, 0, lifeTime, 0, 255));
    }
    sakuraimg.draw(-10,-10, 24,24);
    //座標を復元
    glPopMatrix();
    
}

void SakuraCircle::setccolor(ofColor col){
    ccolor=col;
}
void SakuraCircle::setdeadtime(int time){
    lifeTime=time;
}
