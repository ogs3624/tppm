#pragma once

#include "ofMain.h"
#include "ofxiOS.h"
#include "ofxiPhone.h"
#include "ofxiOSExtras.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#define HOST "localhost"
#define PORT 12345

class ofApp : public ofxiOSApp {
	
public:
    void setup();
    void update();
    void draw();
    void exit();
	
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);
    void touchDoubleTap(ofTouchEventArgs & touch);
    void touchCancelled(ofTouchEventArgs & touch);
    
    void lostFocus();
    void gotFocus();
    void gotMemoryWarning();
    void deviceOrientationChanged(int newOrientation);
    
    ofxOscSender sender;
    ofxiOSKeyboard * keyboard;
    
    string touch_s;
    string textfield;
    string ipAddress;
    
    int field_val=6;
    int field_val_t=0;
    Boolean touched=false;
    
    //タッチした座標の配列(5コ)
    ofPoint touchLoc[5];
    
    ofColor color;
    ofxPanel panel;                 //GUI
    ofParameter<ofColor> color_v;   //color in panel
    //パネルの値が変更された時に対応する関数が呼ばれる
    void colorChanged(ofColor & color_);
    
    int effect=0;
    int effect_seg=0;
    
    void audioIn(float * input, int bufferSize, int nChannels);
    ofSoundStream soundStream;
    float curVol;
    
protected:
    string host_;
};


