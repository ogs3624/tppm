//
//  CustomCircle.h
//  pmApp_receiver
//
//  Created by kayayose on 2014/07/07.
//
//

#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"

//ofxBox2dCircleを継承したクラスCustomCircleを定義
class CustomCircle : public ofxBox2dCircle {
public:
    CustomCircle(); //コンストラクタ
    void update(); //カウンタ更新
    void draw(); //円を描画する
    void setccolor(ofColor col);
    void setdeadtime(int time);
    float counter; //カウンタ
    float phase; //初期位相
    int lifeTime; //表示される長さ
    bool dead;//生死の判定
    ofColor ccolor;
    
};
