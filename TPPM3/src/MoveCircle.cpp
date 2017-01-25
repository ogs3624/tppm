//
//  CustomCircle.cpp
//  pmApp_receiver
//
//  Created by kayayose on 2014/07/07.
//
//

#include "MoveCircle.h"


MoveCircle::MoveCircle()
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

void MoveCircle::update()
{
    pos=getPosition();
    //寿命が尽きたら死亡
    /*
     if (counter > lifeTime) {
     dead = true;
     }//年齢を追加
     counter++;
     */
    
}

void MoveCircle::draw()
{
    //半径を取得
    float radius = getRadius();
    
    //座標を変更
    glPushMatrix();
    //物体の位置に座標を移動
    glTranslatef(getPosition().x, getPosition().y, 0);
    ofFill(); //色を塗り潰す
    //パーティクルを描く
    ofSetColor(ccolor);
    ofCircle(0, 0, radius);
    //座標を復元
    glPopMatrix();
}

void MoveCircle::setccolor(ofColor col){
    ccolor=col;
}
void MoveCircle::setdeadtime(int time){
    lifeTime=time;
}
