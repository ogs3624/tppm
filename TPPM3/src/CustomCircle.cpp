//
//  CustomCircle.cpp
//  pmApp_receiver
//
//  Created by kayayose on 2014/07/07.
//
//

#include "CustomCircle.h"


CustomCircle::CustomCircle()
{
    //現在の年齢
    counter = 0;
    //初期位相
    phase = ofRandom(0, PI*2);
    //寿命を設定
    lifeTime = 10;
    //生死の判定
    dead = false;
    
}

void CustomCircle::update()
{
    //寿命が尽きたら死亡
    if (counter > lifeTime) {
        dead = true;
    }//年齢を追加
    counter++;
}

void CustomCircle::draw()
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
    ofSetColor(127, 255, 255, 24);
    ofCircle(0, 0, radius*2.0);
    ofSetColor(ccolor, 127);
    ofCircle(0, 0, r);
    ofSetColor(255, 255, 255);
    ofCircle(0, 0, r * 0.25);
    //座標を復元
    glPopMatrix();
    
}

void CustomCircle::setccolor(ofColor col){
    ccolor=col;
}
void CustomCircle::setdeadtime(int time){
    lifeTime=time;
}
