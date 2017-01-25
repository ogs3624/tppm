#include "ofApp.h"
#include "MyGuiView.h"

MyGuiView * myGuiViewController;

//--------------------------------------------------------------
void ofApp::setup(){
    //ofBackground(0, 0, 0);
    ofSetFrameRate(60);
    ofSetCircleResolution(64);
    
	// register touch events
    ofRegisterTouchEvents(this);
    // initialize the accelerometer
    ofxAccelerometer.setup();
    //iPhoneAlerts will be sent to this.
    ofxiPhoneAlerts.addListener(this);
    
    ofSetOrientation(OFXIOS_ORIENTATION_LANDSCAPE_LEFT); 
    
    
    //Add Gui setup
    myGuiViewController = [[MyGuiView alloc] initWithNibName:@"MyGuiView" bundle:nil];
    //[ofxiPhoneGetUIWindow() addSubview:myGuiViewController.view];
    [ofxiPhoneGetGLView() addSubview:myGuiViewController.view];
    
    
    textfield="localhost";
    
    host_="localhost";
    // open an outgoing connection to HOST:PORT
	sender.setup( host_, PORT );
    
    //printf("%s",ipAddress.c_str());
    if(ipAddress=="(null)"){
        ipAddress="Not connected.";
    }
    
    //タッチポイントを初期化、画面の外に
    for (int i=0; i<5; i++) {
        touchLoc[i].set(-100, -100);
    }
    
    ofColor mincolor=ofColor(0,0,0,0);
    ofColor maxcolor=ofColor(255,255,255,255);
    
    //以下panelの設定たち
    color_v.addListener(this, &ofApp::colorChanged);
    color.r=31; color.g=63; color.b=200;
    color_v=color;
    
    ofxGuiSetTextPadding(4);
    ofxGuiSetDefaultWidth(200);
    ofxGuiSetDefaultHeight(25);
    
    panel.setup();
    panel.setPosition(820, 90);
    panel.add(color_v.set("color",color_v,mincolor,maxcolor));
    
    //setup(this, outChannels, inChannels, SamplingRate, BufferSize, nBuffer);
    soundStream.setup(this, 0, 1, 44100, 256, 4);
}

//--------------------------------------------------------------
void ofApp::update(){
    //we do a heartbeat on iOS as the phone will shut down the network connection to save power
	//this keeps the network alive as it thinks it is being used.
	if( ofGetFrameNum() % 120 == 0 ){
		ofxOscMessage m;
		m.setAddress( "/misc/heartbeat" );
        m.addStringArg(ipAddress);
		m.addIntArg( ofGetFrameNum() );
		sender.sendMessage( m );
        
	}
    
    if(host_!=textfield){
        host_=textfield;
        sender.setup( host_, PORT );
    }
    if(effect!=effect_seg){
        effect=effect_seg;
        //printf("%d",effect);
        ofxOscMessage m;
		m.setAddress( "/effect" );
        m.addStringArg(ipAddress);
		m.addIntArg(effect);
		sender.sendMessage( m );

    }
    if( effect== 3 ){
        //printf("%f\n",curVol);
        ofxOscMessage m;
        m.setAddress( "/curVol" );
        m.addStringArg(ipAddress);
        m.addFloatArg(curVol);
        sender.sendMessage( m );

    }
}
//--------------------------------------------------------------
void ofApp::draw(){
	// display instructions
	string buf;
	buf = "sending osc messages to " + string( host_ ) +" "+ ofToString( PORT );
	ofDrawBitmapString( buf, 10, 20 );
    ofDrawBitmapString( "This device's ipAddress: "+ ofToString(ipAddress), 10, 50 );
    
    ofSetColor(255,255,255, 255);
    ofNoFill();
    /*
    for(int i=0;i<6;i++){
        ofRect(50+(i/2)*250,120+(i%2)*250, 250, 250);
        
    }
    */
    ofRect(387, 9, 250, 250);
    ofRect(262, 259, 250, 250);
    ofRect(512, 259, 250, 250);
    ofRect(137, 509, 250, 250);
    ofRect(387, 509, 250, 250);
    ofRect(637, 509, 250, 250);
    
    ofSetColor(255,255,255, 255);
    ofFill();
    
    for (int i=0; i<5; i++) {
        if(touchLoc[i].x>387&&touchLoc[i].x<637&&touchLoc[i].y>9&&touchLoc[i].y<259){
            ofCircle(touchLoc[i].x, touchLoc[i].y, 10);
            ofLine(387, touchLoc[i].y,637, touchLoc[i].y);
            ofLine(touchLoc[i].x,9,touchLoc[i].x,259);
        }
        if(touchLoc[i].x>262&&touchLoc[i].x<512&&touchLoc[i].y>259&&touchLoc[i].y<509){
            ofCircle(touchLoc[i].x, touchLoc[i].y, 10);
            ofLine(262, touchLoc[i].y,512, touchLoc[i].y);
            ofLine(touchLoc[i].x,259,touchLoc[i].x,509);
        }
        if(touchLoc[i].x>512&&touchLoc[i].x<762&&touchLoc[i].y>259&&touchLoc[i].y<509){
            ofCircle(touchLoc[i].x, touchLoc[i].y, 10);
            ofLine(512, touchLoc[i].y,762, touchLoc[i].y);
            ofLine(touchLoc[i].x,259,touchLoc[i].x,509);
        }
        if(touchLoc[i].x>137&&touchLoc[i].x<387&&touchLoc[i].y>509&&touchLoc[i].y<759){
            ofCircle(touchLoc[i].x, touchLoc[i].y, 10);
            ofLine(137, touchLoc[i].y,387, touchLoc[i].y);
            ofLine(touchLoc[i].x,509,touchLoc[i].x,759);
        }
        if(touchLoc[i].x>387&&touchLoc[i].x<637&&touchLoc[i].y>509&&touchLoc[i].y<759){
            ofCircle(touchLoc[i].x, touchLoc[i].y, 10);
            ofLine(387, touchLoc[i].y,637, touchLoc[i].y);
            ofLine(touchLoc[i].x,509,touchLoc[i].x,759);
        }
        if(touchLoc[i].x>637&&touchLoc[i].x<887&&touchLoc[i].y>509&&touchLoc[i].y<759){
            ofCircle(touchLoc[i].x, touchLoc[i].y, 10);
            ofLine(637, touchLoc[i].y,887, touchLoc[i].y);
            ofLine(touchLoc[i].x,509,touchLoc[i].x,759);
        }
        
        
        
        ofDrawBitmapString("touch: "
                           + ofToString(i,0)
                           + " = ("
                           + ofToString(touchLoc[i].x,0)
                           + ", "
                           + ofToString(touchLoc[i].y,0)
                           + ")",
                           10, 12*i+80);
        
        
    }
    
    
    panel.draw();
}

//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::touchDown(ofTouchEventArgs & touch){
    if(touch.id<5){
        touchLoc[touch.id].set(touch.x, touch.y);
    
    
    
    ofxOscMessage m,m1;
	m.setAddress( "/mouse/button" );
    
	m.addStringArg(ipAddress);
    touch_s="down";
	m.addStringArg(touch_s);
    m.addIntArg(touch.id);
    sender.sendMessage( m );
    
    m1.setAddress( "/mouse/position" );
	m1.addStringArg(ipAddress);
    for (int i=0; i<5; i++) {
        m1.addIntArg(touchLoc[i].x);
        m1.addIntArg(touchLoc[i].y);
    }
    sender.sendMessage( m1 );
    
    
    
    touched=true;
    }
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs & touch){
    if(touch.id<5){
        touchLoc[touch.id].set(touch.x, touch.y);
    
    
	ofxOscMessage m,m1;
    
    m.setAddress( "/mouse/button" );
    m.addStringArg(ipAddress);
    touch_s="move";
	m.addStringArg(touch_s);
    m.addIntArg(touch.id);
    sender.sendMessage( m );
    
	m1.setAddress( "/mouse/position" );
    m1.addStringArg(ipAddress);
    for (int i=0; i<5; i++) {
        m1.addIntArg(touchLoc[i].x);
        m1.addIntArg(touchLoc[i].y);
    }
    sender.sendMessage( m1 );
    
    }
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs & touch){
    if(touch.id<5){
        touchLoc[touch.id].set(-100,-100);
    
    
	ofxOscMessage m,m1;
    
	m.setAddress( "/mouse/button" );
    m.addStringArg(ipAddress);
    touch_s="up";
	m.addStringArg(touch_s);
    m.addIntArg(touch.id);
    sender.sendMessage( m );
    
	m1.setAddress( "/mouse/position" );
    m1.addStringArg(ipAddress);
    for (int i=0; i<5; i++) {
        m1.addIntArg(touchLoc[i].x);
        m1.addIntArg(touchLoc[i].y);
    }
    sender.sendMessage( m1 );
    
    touched=false;
        }
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs & touch){
    
}

//--------------------------------------------------------------
void ofApp::lostFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotFocus(){
    
}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning(){
    
}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation){
    
}

void ofApp::colorChanged(ofColor & color_){
    ofxOscMessage m;
	m.setAddress( "/color" );
    
	m.addStringArg(ipAddress);
    m.addIntArg(color_.r);
    m.addIntArg(color_.g);
    m.addIntArg(color_.b);
    m.addIntArg(color_.a);
    
    sender.sendMessage( m );
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    curVol = 0.0;
    
    for (int i = 0; i < bufferSize; i++){
        curVol += input[i]*input[i]; // 聴覚上は出力の2乗に比例するので、2乗しています。
    }
    
    curVol /= bufferSize; // 平均を取ります。
}

