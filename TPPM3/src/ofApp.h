#pragma once

#include "ofMain.h"
#include "ofxQuadWarp.h"
#include "ofxOsc.h"
#include "Devices.h"
#include "Ring.h"
#include "ofxBox2d.h"
#include "CustomCircle.h"
#include "MoveCircle.h"
#include "SakuraCircle.h"
#include "Pointtext.h"

// listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20

class ofApp : public ofBaseApp
{
public:
    
    void setup  ();
    void update ();
    void draw   ();
    
    void keyPressed     ( int key );
    void keyReleased    ( int key );
    void mouseMoved     ( int x, int y );
    void mouseDragged   ( int x, int y, int button );
    void mousePressed   ( int x, int y, int button );
    void mouseReleased  ( int x, int y, int button );
    void windowResized  ( int w, int h );
    void dragEvent      ( ofDragInfo dragInfo );
    void gotMessage     ( ofMessage msg );
    
    ofTrueTypeFont font;
    ofxOscReceiver receiver;
    
    int current_msg_string;
    string msg_strings[NUM_MSG_STRINGS];
    float timers[NUM_MSG_STRINGS];
    
    int mouseX, mouseY,mouseY_;
    int field_val_t;
    string mouseButtonState;
    
    vector <Devices>    devices;
    int device_num     (string ip );
    
    vector <Ring *> rings; //拡大する輪"Ring"の配列
    vector <Pointtext *> pointtexts; //拡大する輪"Ring"の配列
    
    ofxQuadWarp         warper[6];
    ofFbo               fbo[6];
    int x,y,w,h,face_val,move_count=0,effect=0,timecount=0;
    int effect_dim[6];
    int ball_count[6];
    int ball_max[6];
    
    ofVec2f move;
    float minDis=100;
    
    ofxBox2d box2d[6];
    ofRectangle bounds[6];
    //vector <ofPtr<ofxBox2dCircle> >  circles[6];
    vector <SakuraCircle *> sakuracircle[6];
    vector <CustomCircle *> costomcircle[6];
    vector <MoveCircle *> movecircle[6];
    
    //音声を拾う
    void audioIn(float * input, int bufferSize, int nChannels);
    ofSoundStream soundStream;
    float curVol;
    float vol[6];
    
    //音波形関係
    int groundRes[6];
    float groundSpeed[6];
    float groundTimer[6];
    
	ofPolyline groundLine[6];
	ofxBox2dEdge ground[6];
    
    //音楽
    ofSoundPlayer  sound[5];
    ofSoundPlayer  rhythm;
    ofSoundPlayer  fan;
    ofSoundPlayer bgm;
    
    int bpm = 120;
    int speed= 8;
    int meter_b=0;
    
    //cooperation
    bool coo=false;
    int cootime=0;
    bool tempo=false;
    int meter=0;
    bool beat=false;
    int ringtime=0;
    int getpoint=0;
    int pointmax=1500;
    bool ending=false;
    int endingtime=0;
    
    //放置
    int houtitime=0;
    
    //force
    int time=0;
    int movingtime[6];
    ofPoint move_pos[6];
    int moving_arc[6];
    int xx=0,xxx=0;
    
    int ringval=0;
    ofImage sakuraimg;
    
};

