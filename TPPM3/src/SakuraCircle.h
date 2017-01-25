//
//  SakuraCircle.h
//  TPPM2
//
//  Created by Wataru Ogasa on 2015/10/21.
//
//
#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

//ofxBox2dCircleを継承したクラスCustomCircleを定義
class SakuraCircle : public ofxBox2dCircle {
public:
    SakuraCircle(ofImage img); //コンストラクタ
    void update(); //カウンタ更新
    void draw(); //円を描画する
    void setccolor(ofColor col);
    void setdeadtime(int time);
    float counter; //カウンタ
    float phase; //初期位相
    int lifeTime; //表示される長さ
    bool dead;//生死の判定
    ofImage sakuraimg;
    ofColor ccolor;
    float deg;
    float degplus;
};
