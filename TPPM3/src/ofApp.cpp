#include "ofApp.h"


ofTrueTypeFont font,font2;

struct S{
    string text;
    int x, y;
    ofRectangle rect;
};
S num;


//--------------------------------------------------------------
void ofApp::setup(){
    
	ofBackground(0,0,0);
    ofSetFrameRate( 60 );
    ofSetVerticalSync( true );
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    
    // listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
    
	current_msg_string = 0;
	mouseX = 0;
	mouseY = 0;
    mouseY_=0;
    field_val_t=0;
	mouseButtonState = "";
    ringval=0;
    
    x = 0;   // center on screen.
    y = 0;   // center on screen.
    w = 250;
    h = 250;
    face_val=6;
    
    for (int i=0; i<face_val; i++) {
        
#ifdef TARGET_OPENGLES
        fbo[i].allocate(w, h, GL_RGBA ); // with alpha, 32 bits red, 32 bits green, 32 bits blue, 32 bits alpha, from 0 to 1 in 'infinite' steps
        ofLogWarning("ofApp") << "GL_RGBA32F_ARB is not available for OPENGLES.  Using RGBA.";
#else
        fbo[i].allocate(w, h, GL_RGBA32F_ARB); // with alpha, 32 bits red, 32 bits green, 32 bits blue, 32 bits alpha, from 0 to 1 in 'infinite' steps
#endif
        fbo[i].begin();
        ofClear(255,255,255, 0);
        fbo[i].end();
        
        warper[i].setSourceRect(ofRectangle((i/2)*260+10, (i%2)*260+10, w, h));
        
        warper[i].setTopLeftCornerPosition( ofPoint( (i/2)*260+10, (i%2)*260+10 ) );             // this is position of the quad warp corners, centering the image on the screen.
        warper[i].setTopRightCornerPosition( ofPoint( (i/2)*260+10+ w, (i%2)*260+10 ) );        // this is position of the quad warp corners, centering the image on the screen.
        warper[i].setBottomLeftCornerPosition( ofPoint( (i/2)*260+10, (i%2)*260+10 + h ) );      // this is position of the quad warp corners, centering the image on the screen.
        warper[i].setBottomRightCornerPosition( ofPoint( (i/2)*260+10 + w, (i%2)*260+10 + h ) ); // this is position of the quad warp corners, centering the image on the screen.
        warper[i].setup();
        
        
        bounds[i].set(0,0,250,250);
        
        box2d[i].init();
        box2d[i].setFPS(60);
        box2d[i].setGravity(0, 0);
        box2d[i].createBounds(bounds[i]);
        //box2d[i].registerGrabbing();  //つかめるかどうか
    }
    for(int i=0;i<6;i++){
        effect_dim[i]=0;
        ball_count[i]=0;
        ball_max[i]=2;
        move_pos[i]=ofPoint(ofRandom(10, 100),ofRandom(10,240));
        movingtime[i]=0;
        moving_arc[i]=ofRandom(30,80);
    }
    
    //フォントデータの読み込み
    font.loadFont("KozGoPro-Regular.otf", 120);
    num.text = "Hello!!";
    num.x = 0;
    num.y = 0;
    //フォントを取り囲む長方形を取得
    num.rect = font.getStringBoundingBox("Hello!!.", 0, 0);
    
    //フォントデータの読み込み
    font2.loadFont("mountainsofchristmas.otf", 26);
    num.text = "Hello!!";
    num.x = 0;
    num.y = 0;
    //フォントを取り囲む長方形を取得
    num.rect = font2.getStringBoundingBox("Hello!!.", 0, 0);
    
    //setup(this, outChannels, inChannels, SamplingRate, BufferSize, nBuffer);
    soundStream.setup(this, 0, 1, 44100, 256, 4);
    
    
    //効果音
    sound[0].loadSound("sfx/se001.mp3");
    sound[0].setVolume(0.3f);
	sound[0].setMultiPlay(true);
    sound[1].loadSound("sfx/3.mp3");
    sound[1].setVolume(0.3f);
	sound[1].setMultiPlay(true);
    sound[2].loadSound("sfx/4.mp3");
    sound[2].setVolume(0.8f);
	sound[2].setMultiPlay(true);
    sound[3].loadSound("sfx/1.mp3");
    sound[3].setVolume(0.5f);
	sound[3].setMultiPlay(true);
    sound[4].loadSound("sfx/5.mp3");
    sound[4].setVolume(0.4f);
	sound[4].setMultiPlay(true);
    rhythm.loadSound("sfx/rhythm2.wav");
    rhythm.setVolume(0.5f);
	rhythm.setMultiPlay(true);
    rhythm.setLoop(true);
    bgm.loadSound("sfx/bgm.wav");
    bgm.setVolume(0.25f);
	bgm.setMultiPlay(true);
    bgm.setLoop(true);
    fan.loadSound("sfx/ji_017.mp3");
    fan.setVolume(0.5f);
	fan.setMultiPlay(true);
    
    sakuraimg.loadImage("sakura.png");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    int num=0;
    time++;
    
    
    // hide old messages
	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		if(timers[i] < ofGetElapsedTimef()){
			msg_strings[i] = "";
		}
	}
    
    for(int i=0;i<face_val;i++){
        vol[i]=curVol;
    }
    
	// check for waiting messages
	while(receiver.hasWaitingMessages()){
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
		// check for mouse moved message
        if(m.getAddress() == "/misc/heartbeat"){
            num=device_num(m.getArgAsString(0));
            devices[num].heartbeat=m.getArgAsInt32(1);
            //printf("%d",m.getArgAsInt32(1));
        }
        else if(m.getAddress() == "/mouse/position"){
			// both the arguments are int32's
            num=device_num(m.getArgAsString(0));
            
            for(int i=0;i<5;i++){
                devices[num].mx[i]=m.getArgAsInt32((i+1)*2-1);
                devices[num].my[i]=m.getArgAsInt32((i+1)*2);
                //printf("%d,%d\n",devices[num].mx[i],devices[num].my[i]);
            }
            devices[num].setdim();
            
		}
		// check for mouse button message
		else if(m.getAddress() == "/mouse/button"){
			// the single argument is a string
            num=device_num(m.getArgAsString(0));
            devices[num].ButtonState=m.getArgAsString(1);
            
            if(m.getArgAsString(1)=="down"){
                
                houtitime=0;
                
                //波
                if(devices[num].dim_x[m.getArgAsInt32(2)]>0&&
                   devices[num].dim_y[m.getArgAsInt32(2)]>0){
                    if(effect_dim[devices[num].dim[m.getArgAsInt32(2)]]==0){
                        rings.push_back(new Ring(ofPoint(devices[num].dim_x[m.getArgAsInt32(2)],devices[num].dim_y[m.getArgAsInt32(2)]),devices[num].dim[m.getArgAsInt32(2)],num,ringval));
                        
                        //さくら
                        if(ringval==1){
                            for(int i=0;i<5;i++){
                                float r = 5;
                                //Box2Dの円のインスタンスを生成
                                ofSetColor(255, 255, 255);
                                SakuraCircle *c = new SakuraCircle(sakuraimg);
                                //物理特性の設定
                                c->setPhysics(1.0, 0.8, 0.1);
                                //生存時間
                                c->setdeadtime(30);
                                //世界に追加
                                c->setup(box2d[devices[num].dim[m.getArgAsInt32(2)]].getWorld(), devices[num].dim_x[m.getArgAsInt32(2)],devices[num].dim_y[m.getArgAsInt32(2)],r);
                                //c->setup(box2d[0].getWorld(), x, y, r);
                                //色
                                c->setccolor(ofColor(255,255,255));
                                //力をランダムに加える
                                float force = 28;
                                c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                                //ベクターに追加
                                sakuracircle[devices[num].dim[m.getArgAsInt32(2)]].push_back(c);
                                //printf("%d,%d\n",i,j);
                            }
                        }
                        
                        
                        //効果音
                        sound[0].play();
                        
                        //cooperationtime
                        //printf("%d",meter-500);
                        if(coo==true){
                            pointtexts.push_back(new Pointtext(ofPoint(devices[num].dim_x[m.getArgAsInt32(2)],devices[num].dim_y[m.getArgAsInt32(2)]),devices[num].dim[m.getArgAsInt32(2)],num,abs(meter-500)));
                            
                            if(abs(meter-500)<75){
                                getpoint+=5;
                            }else if(abs(meter-500)>=75&&abs(meter-500)<150){
                                getpoint+=3;
                            }else if(abs(meter-500)>=150&&abs(meter-500)<300){
                                getpoint+=1;
                            }else{
                                getpoint-=1;
                            }
                            
                            if(getpoint>pointmax){
                                meter_b=0;
                                ringtime=0;
                                getpoint=0;
                                rhythm.stop();
                                cootime=0;
                                tempo=false;
                                ending=true;
                                fan.play();
                            }
                            
                            //printf("%d\n",getpoint);
                        }
                        
                        
                    }
                    
                    //printf("%d",devices[num].dim[m.getArgAsInt32(2)]);
                    //printf("%d",m.getArgAsInt32(2));
                    
                }
                 
                devices[num].piled[m.getArgAsInt32(2)]=1;
                
                //ボールつぶし
                for(int i=0;i<face_val;i++){
                    if(effect_dim[i]==2){
                        if(devices[num].dim[m.getArgAsInt32(2)]==i){
                            for(int j=0; j<movecircle[i].size(); j++) {
                                if(sqrt((movecircle[i][j]->pos.x-devices[num].dim_x[m.getArgAsInt32(2)])*(movecircle[i][j]->pos.x-devices[num].dim_x[m.getArgAsInt32(2)])+
                                        (movecircle[i][j]->pos.y-devices[num].dim_y[m.getArgAsInt32(2)])*(movecircle[i][j]->pos.y-devices[num].dim_y[m.getArgAsInt32(2)]))<15){
                                    movecircle[i][j]->dead=true;
                                    ball_count[i]--;
                                    
                                    //効果音
                                    sound[2].play();
                                    
                                }
                            }
                        }
                        
                    }
                    
                    
                    //音波形のボール
                    if(effect_dim[i]==3){
                        if(devices[num].dim[m.getArgAsInt32(2)]==i){
                            if(devices[num].dim_x[m.getArgAsInt32(2)]>0&&devices[num].dim_x[m.getArgAsInt32(2)]<250&&
                               devices[num].dim_y[m.getArgAsInt32(2)]>0&&devices[num].dim_y[m.getArgAsInt32(2)]<250){
                                if(box2d[i].getBodyCount()<40){
                                    float r = ofRandom(5,10);
                                    //Box2Dの円のインスタンスを生成
                                    MoveCircle *c = new MoveCircle();
                                    //物理特性の設定
                                    c->setPhysics(1.0, 0.8, 0.5);
                                    //世界に追加
                                    c->setup(box2d[i].getWorld(), devices[num].dim_x[m.getArgAsInt32(2)],devices[num].dim_y[m.getArgAsInt32(2)],r);
                                    //c->setup(box2d[0].getWorld(), x, y, r);
                                    //色
                                    c->setccolor(devices[num].color);
                                    //力をランダムに加える
                                    float force = 50;
                                    c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                                    //ベクターに追加
                                    movecircle[i].push_back(c);
                                    //効果音
                                    sound[3].play();
                                }else{
                                    std::vector<MoveCircle*>::iterator itr = movecircle[i].begin();
                                    (*itr)->dead=true;
                                    
                                    float r = ofRandom(5,10);
                                    //Box2Dの円のインスタンスを生成
                                    MoveCircle *c = new MoveCircle();
                                    //物理特性の設定
                                    c->setPhysics(1.0, 0.8, 0.5);
                                    //世界に追加
                                    c->setup(box2d[i].getWorld(), devices[num].dim_x[m.getArgAsInt32(2)],devices[num].dim_y[m.getArgAsInt32(2)],r);
                                    //c->setup(box2d[0].getWorld(), x, y, r);
                                    //色
                                    c->setccolor(devices[num].color);
                                    //力をランダムに加える
                                    float force = 50;
                                    c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                                    //ベクターに追加
                                    movecircle[i].push_back(c);
                                    //効果音
                                    sound[3].play();
                                }
                                
                                
                                
                                
                            }
                        
                        }
                    }
                }
                
            }else if(m.getArgAsString(1)=="move"){
                
                houtitime=0;
                //重なっている数を数える(pileの値:押されていない=0,押されている=1,重なってる=2~)
                int pile_count=1;
                for(int i=0;i<devices.size();i++){
                    for(int j=0;j<5;j++){
                        if(devices[i].mx[j]>0&&devices[i].my[j]>0){
                            for(int k=0;k<devices.size();k++){
                                if(i!=k){
                                    for(int l=0;l<5;l++){
                                         if(devices[k].mx[l]>0&&devices[k].my[l]>0){
                                             if(sqrt((devices[i].mx[j]-devices[k].mx[l])*(devices[i].mx[j]-devices[k].mx[l])+(devices[i].my[j]-devices[k].my[l])*(devices[i].my[j]-devices[k].my[l]))<20){
                                                 pile_count++;
                                             }
                                             
                                         }
                                    }
                                }
                            }
                            
                            devices[i].piled[j]=pile_count;
                            pile_count=1;
                        }
                    }
                }
                
                
                //move_count++;
                
                
                
            }else if(m.getArgAsString(1)=="up"){
                devices[num].dim_x[m.getArgAsInt32(2)]=-100;
                devices[num].dim_y[m.getArgAsInt32(2)]=-100;
                devices[num].dim[m.getArgAsInt32(2)]=0;
                devices[num].piled[m.getArgAsInt32(2)]=0;
            }
            
		}
		else if(m.getAddress() == "/color"){
			// the single argument is a string
            num=device_num(m.getArgAsString(0));
            devices[num].color.r=m.getArgAsInt32(1);
            devices[num].color.g=m.getArgAsInt32(2);
            devices[num].color.b=m.getArgAsInt32(3);
            devices[num].color.a=m.getArgAsInt32(4);
            
            //printf("%d,%d,%d\n",m.getArgAsInt32(1),m.getArgAsInt32(2),m.getArgAsInt32(3));
		}
        else if(m.getAddress() == "/effect"&&coo==false){
            //以下はエフェクト番号が変わった瞬間に一度だけ呼ばれる
            
            num=device_num(m.getArgAsString(0));
            effect=m.getArgAsInt32(1);
            //devices[num].effect_d=effect;
            //printf("%d",effect);
            effect_dim[num]=effect;
            
            
            if(effect==3){
                // x, y方向の重力を設定
                box2d[num].setGravity(0, 40);
                
                groundSpeed[num] = 0.02;
                groundTimer[num] = 0;
                groundRes[num]   = 50;
                
                ground[num].clear();
                
                for(int i=0; i<groundRes[num]; i++) {
                    float x = ofMap(i, 0, groundRes[num]-1, 0, 250);
                    float y = 250/2;
                    groundLine[num].addVertex(x, y);
                }
                
            }else{
                ground[num].clear();
                groundLine[num].clear();
                for(int i=0;i<movecircle[num].size();i++){
                    movecircle[num][i]->dead=true;
                }
                box2d[num].setGravity(0, 0);
            }
            
            if(effect==2){
                
                
                for(int i=0;i<ball_max[num];i++){
                    float r = 15;
                    //Box2Dの円のインスタンスを生成
                    MoveCircle *c = new MoveCircle();
                    //物理特性の設定-重さ、反発力、摩擦力
                    c->setPhysics(0.85, 1, 0);
                    //世界に追加
                    c->setup(box2d[num].getWorld(), ofRandom(120, 130),ofRandom(120,130),r);
                    //c->setup(box2d[0].getWorld(), x, y, r);
                    //色
                    c->setccolor(ofColor(63,251,255));
                    //力をランダムに加える
                    float force = 500;
                    c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                    //ベクターに追加
                    movecircle[num].push_back(c);
                    ball_count[num]++;
                }
            }else{
                for(int i=0;i<movecircle[num].size();i++){
                    movecircle[num][i]->dead=true;
                }
                ball_max[num]=2;
                ball_count[num]=0;
            }
            
            
            
        }else if(m.getAddress() == "/curVol"){
            num=device_num(m.getArgAsString(0));
            //effect=m.getArgAsInt32(1);
            //printf("%f\n",m.getArgAsFloat(1));
            vol[num]=m.getArgAsFloat(1);
        }
	}
    
    
    //放置
    /*
    houtitime++;
    if(houtitime>=2000){
        if (houtitime%100==0) {
            rings.push_back(new Ring(ofPoint((int)ofRandom(50,250),(int)ofRandom(50,250)),(int)ofRandom(0,6),0));
            
            //効果音
            sound[0].play();
        }
    }
    */
    
    
    
    //Ring更新
    for(vector <Ring *>::iterator it = rings.begin(); it != rings.end();){
        (*it)->update();
        if ((*it)->dead) {
            delete (*it);
            it = rings.erase(it);
        } else {
            ++it;
        }
    }
    //Pointtext更新
    for(vector <Pointtext *>::iterator it = pointtexts.begin(); it != pointtexts.end();){
        (*it)->update();
        if ((*it)->dead) {
            delete (*it);
            it = pointtexts.erase(it);
        } else {
            ++it;
        }
    }
    
    
    //一定時間応答がないと自動的にデバイスのリストから消される処理。
    //完成しているが、再接続の際データを残しておきたいため消しておく。
    /*
    if( ofGetFrameNum() % 240 == 0 ){
        
        for(int i=0;i<devices.size();i++){
            if(devices[i].heartbeat==devices[i].heartbeat_){
     
                //for(int j=i;j<devices.size();j++){
                //}
     
                devices[i].connected=false;
                
            }else{
                devices[i].heartbeat_=devices[i].heartbeat;
            }
        }
        
        int count=0;
        std::vector<Devices>::iterator itr = devices.begin();
        while (itr != devices.end()) {
            if ((*itr).connected==false) {
                itr = devices.erase(itr);
                //printf("erase");
            }else{
                itr->num=count;
                ++itr;
                count++;
            }  
        }
        
    }
    */
    
    
    
    //パーティクル、動く円の処理
    for(int i=0;i<face_val;i++){
        box2d[i].update();
        for(vector <CustomCircle *>::iterator it = costomcircle[i].begin(); it != costomcircle[i].end();){
            //CustomCircleの状態を更新
            (*it)->update();
            //もし寿命が尽きていたら、CustomCircleを消去
            if ((*it)->dead) {
                //形態をBox2Dの世界から消去(今消してるけどいるかもしれん)
                //(*it)->destroy();
                //オブジェクトを解放
                delete *it;
                //動的配列から、オブジェクトを削除
                it = costomcircle[i].erase(it);
            } else {
                ++it;
            }
        }
        for(vector <SakuraCircle *>::iterator it = sakuracircle[i].begin(); it != sakuracircle[i].end();){
            //CustomCircleの状態を更新
            (*it)->update();
            //もし寿命が尽きていたら、CustomCircleを消去
            if ((*it)->dead) {
                //形態をBox2Dの世界から消去(今消してるけどいるかもしれん)
                //(*it)->destroy();
                //オブジェクトを解放
                delete *it;
                //動的配列から、オブジェクトを削除
                it = sakuracircle[i].erase(it);
            } else {
                ++it;
            }
        }
        
        for(vector <MoveCircle *>::iterator it = movecircle[i].begin(); it != movecircle[i].end();){
            //MoveCircleの状態を更新
            (*it)->update();
            //もし寿命が尽きていたら、MoveCircleを消去
            if ((*it)->dead) {
                //形態をBox2Dの世界から消去(今消してるけどいるかもしれん)
                //(*it)->destroy();
                //オブジェクトを解放
                delete *it;
                //動的配列から、オブジェクトを削除
                it = movecircle[i].erase(it);
            } else {
                ++it;
            }
        }
        
    }
    
    //パーティクルの生成
    for(int i=0;i<devices.size();i++){
        for(int j=0;j<5;j++){
            if(devices[i].piled[j]!=0){
                if(effect_dim[devices[i].dim[j]]==1){
                    if(devices[i].dim_x[j]>0&&devices[i].dim_x[j]<250&&
                       devices[i].dim_y[j]>0&&devices[i].dim_y[j]<250){
                        //printf("%d\n",move_count);
                        if(move_count%(5-devices[i].piled[j])==0){
                            //半径をランダムに決める
                            float r = ofRandom(5, 5+devices[i].piled[j]*3);
                            //Box2Dの円のインスタンスを生成
                            CustomCircle *c = new CustomCircle();
                            //物理特性の設定
                            c->setPhysics(1.0, 0.8, 0.1);
                            //生存時間
                            c->setdeadtime(devices[i].piled[j]*10-5);
                            //世界に追加
                            c->setup(box2d[devices[i].dim[j]].getWorld(), devices[i].dim_x[j],devices[i].dim_y[j],r);
                            //c->setup(box2d[0].getWorld(), x, y, r);
                            //色
                            c->setccolor(devices[i].color);
                            //力をランダムに加える
                            float force = 150;
                            c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                            //ベクターに追加
                            costomcircle[devices[i].dim[j]].push_back(c);
                            //printf("%d,%d\n",i,j);
                            
                            
                            
                            //効果音
                            sound[1].play();
                        }
                    }
                }
            }
            //move_count++;
        }
        
    }
    move_count++;
    //ボールつぶし
    for(int i=0;i<face_val;i++){
        if(effect_dim[i]==2){
            if(ball_count[i]==0){
                ball_max[i]=ball_max[i]*2;
                
                if(ball_max[i]>33){
                    ball_max[i]=2;
                }
                for(int j=0;j<ball_max[i];j++){
                    //printf("hoge");
                    float r = 15;
                    //Box2Dの円のインスタンスを生成
                    MoveCircle *c = new MoveCircle();
                    //物理特性の設定-重さ、反発力、摩擦力
                    c->setPhysics(0.85, 1, 0);
                    //世界に追加
                    c->setup(box2d[i].getWorld(), ofRandom(120, 130),ofRandom(120,130),r);
                    //c->setup(box2d[0].getWorld(), x, y, r);
                    //色
                    c->setccolor(ofColor(63,251,255));
                    //力をランダムに加える
                    float force = 500;
                    c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                    //ベクターに追加
                    movecircle[i].push_back(c);
                    ball_count[i]++;
                }
            }
        }
        
        //音波形のupdate
        if(effect_dim[i]==3){
            ground[i].clear();
            ground[i].addVertexes(groundLine[i]);
            ground[i].create(box2d[i].getWorld());
            
            for(int j=0;j<movecircle[i].size();j++){
                if(movecircle[i][j]->pos.y>230){
                    movecircle[i][j]->dead=true;
                }
            }
            //ofRemove(movecircle[i], ofxBox2dBaseShape::shouldRemoveOffScreen);
            
        }
        
}
    
    
    
    if (coo==true) {
        ringtime++;
        //拍を取る
        meter = (ofGetElapsedTimeMillis()-cootime) % (int)(60000/bpm/ 4.0 * speed);
        //printf("%d,%d\n",meter,meter_b);
        if(meter>500&&tempo==false){
            //printf("Bang");
            //printf("%d\n",ringtime);
            ringtime=0;
            beat=true;
            tempo=true;
        }else{
            beat=false;
        }
        
        if (meter_b > meter){
            tempo=false;
        }
        meter_b = meter;
        
        
        
    }else{
        meter_b=0;
    }
    
    
    
    //printf("%d\n",box2d[0].getBodyCount());
}

//--------------------------------------------------------------
void ofApp::draw()
{
    /*
    string info = "";
	info += "Total Bodies: "+ofToString(box2d[0].getBodyCount())+"\n";
	info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
	ofSetHexColor(0x444342);
	ofDrawBitmapString(info, 30, 30);
*/
        for (int i=0;i<6; i++) {
        
        //======================== draw image into fbo.
        fbo[i].begin();
        {
            
            ofClear(255,255,255, 0);
            
            
            
            // world A
            if(i==0){
                ofSetColor(100,255,255);
            }else{
                ofSetColor(0,0,255);
            }
            
            if(effect_dim[i]==2){
                ofSetColor(255,255,255,180);
                string str;
                str=ofToString(ball_count[i]);
                //ofDrawBitmapString(str, 10,240);
                num.rect = font.getStringBoundingBox(str, 0, 0);
                font.drawString(str, 125-num.rect.width/2, 125+num.rect.height/2);
            }
            
            for(vector <CustomCircle *>::iterator it = costomcircle[i].begin(); it != costomcircle[i].end(); ++it) {
                (*it)->draw();
            }
            for(vector <MoveCircle *>::iterator it = movecircle[i].begin(); it != movecircle[i].end(); ++it) {
                (*it)->draw();
            }
            
            for (int j=0; j<devices.size(); j++) {
                ofSetColor(devices[j].color);
                /*
                string bu;
                bu = "("+ofToString(j)+") "+ofToString((int)devices[j].color.r)+","+ofToString((int)devices[j].color.g)+","+ofToString((int)devices[j].color.b);
                ofDrawBitmapString(bu, 50,50*(j+1));
                */
                /*
                printf("(%d),%d,%d,%d\n",j,devices[j].color.r,devices[j].color.g,devices[j].color.b);
                ofCircle(50,50*(j+1),30);
                */
                
                //押された場所の丸
                for (int k=0; k<5; k++) {
                    if(devices[j].dim[k]==i){
                        if(devices[j].dim_x[k]>0&&devices[j].dim_y[k]>0){
                            ofCircle(devices[j].dim_x[k],devices[j].dim_y[k], devices[j].piled[k]*3);
                        }
                    }
                }
                
                
            }
            
            for(vector <Ring *>::iterator it = rings.begin(); it != rings.end(); ++it){
                if((*it)->dimention==i){
                    //ofColor(devices[(*it)->dev].color);
                    //printf("hoge");
                    //printf("%d",devices[(*it)->dev].color.r);
                    if(houtitime>=1999){
                        (*it)->setcolor_ring(ofColor(255,255,255));
                    }else{
                        (*it)->setcolor_ring(devices[(*it)->dev].color);
                    }
                    (*it)->draw();
                }
            }
            
            for(vector <Pointtext *>::iterator it = pointtexts.begin(); it != pointtexts.end(); ++it){
                if((*it)->dimention==i){
                    //ofColor(devices[(*it)->dev].color);
                    //printf("hoge");
                    //printf("%d",devices[(*it)->dev].color.r);
                    (*it)->setcolor_ring(devices[(*it)->dev].color);
                    (*it)->draw();
                }
            }
            
            //音波形の大きさ
            if(effect_dim[i]==3){
                float v = vol[i];
                float   t = ofGetElapsedTimef() * 1.5;
                //float   y = ofSignedNoise(v) *600 ;
                //printf("%f\n",v);
                float   y =sin(ofGetElapsedTimef()*15)*ofMap(v, 0, 0.2, 0, 100);
                //groundSpeed=0.1;
                
                
                if(ofGetElapsedTimef() - groundTimer[i] > groundSpeed[i]) {
                    
                    float newHeight = 250/2 - y;
                    int n = (int)groundLine[i].size();
                    
                    groundTimer[i] = ofGetElapsedTimef();
                    
                    for(int j=n-1; j>=1; j--) {
                        groundLine[i][j].y = groundLine[i][j-1].y;
                    }
                    groundLine[i][0].y = newHeight;
                    
                }
                
                ofSetColor(255);
                ground[i].updateShape();
                ground[i].draw();
            }
            
            //cooperation time
            if(coo==true&&ending==false){
                
                ofFill();
                ofSetColor(71,234,126,180);
                ofRect(0, 250-ofMap(getpoint, 0, pointmax, 0, 250), 5, ofMap(getpoint, 0, pointmax, 0, 250));
                ofRect(245, 250-ofMap(getpoint, 0, pointmax, 0, 250), 5, ofMap(getpoint, 0, pointmax, 0, 250));
                
                
                //ofCircle(125, 125, 20);
                ofNoFill();
                ofSetColor(255,255,255);
                ofRect(125-ofMap(ringtime, 0, 57, 0, 125), 125-ofMap(ringtime, 0, 57, 0, 125), ofMap(ringtime, 0, 57, 0, 250), ofMap(ringtime, 0, 57, 0, 250));
                
                //printf("%f",ofMap(ringtime, 0, 57, 0, 125));
                
                
            }
            
            if(ending==true){
                if(i==0){
                    ofSetColor(255,127,127);
                }else if(i==1){
                    ofSetColor(255,255,127);
                }else if(i==2){
                    ofSetColor(127,255,127);
                }else if(i==3){
                    ofSetColor(127,255,255);
                }else if(i==4){
                    ofSetColor(127,127,255);
                }else if(i==5){
                    ofSetColor(255,127,255);
                }
                
                string str;
                str="Congratulations!!";
                //ofDrawBitmapString(str, 10,240);
                num.rect = font2.getStringBoundingBox(str, 0, 0);
                font2.drawString(str, 125-num.rect.width/2, 125+num.rect.height/2);
                
                ofFill();
                if (endingtime<400) {
                    ofRect(0, 0, ofMap(endingtime, 0, 400, 0, 250), 5);
                }else if(endingtime>=400&&endingtime<800){
                    ofRect(0,0, 250, 5);
                    ofRect(245, 0, 5, ofMap(endingtime, 400, 800, 0, 250));
                }else if(endingtime>=800&&endingtime<1200){
                    ofRect(0,0, 250, 5);
                    ofRect(245,0, 5, 250);
                    ofRect(250-ofMap(endingtime, 800, 1200, 0, 250), 245, ofMap(endingtime, 800, 1200, 0, 250), 245);
                }else if(endingtime>=1200&&endingtime<1600){
                    ofRect(0,0, 250, 5);
                    ofRect(245,0, 5, 250);
                    ofRect(0,245, 250, 5);
                    ofRect(0, 250-ofMap(endingtime, 1200, 1600, 0, 250), 5, ofMap(endingtime, 1200, 1600, 0, 250));
                }else{
                    ofRect(0,0, 250, 5);
                    ofRect(245,0, 5, 250);
                    ofRect(0,245, 250, 5);
                    ofRect(0,0, 5, 250);
                }
                
                
                endingtime++;
                
            }
            if(endingtime>1800){
                ending=false;
                coo=false;
            }
            
            for(vector <SakuraCircle *>::iterator it = sakuracircle[i].begin(); it != sakuracircle[i].end(); ++it) {
                (*it)->draw();
            }
            
        }
        fbo[i].end();
        
        //======================== get our quad warp matrix.
        ofMatrix4x4 mat = warper[i].getMatrix();
        
        //======================== use the matrix to transform our fbo.
        ofSetColor(255,255,255);
        
        glPushMatrix();
        glMultMatrixf( mat.getPtr() );
        {
            fbo[i].draw( (i/2)*260+10, (i%2)*260+10 );
        }
        glPopMatrix();
        
        //======================== draw quad warp ui.
        ofFill();
        ofSetColor( ofColor :: magenta);
        warper[i].draw();
    }
    
    ofSetColor( ofColor :: white );
    
    
    //情報を表示(最終的には消す)
    /*
    for(int i=0;i<devices.size();i++){
        if(devices[i].connected==true){
            //printf("hoge %d",i);
            string buf;
            buf = "listening for osc messages number "+ofToString(devices[i].num)+" on ip " + devices[i].ipAddress +" heartbeat: "+ofToString(devices[i].heartbeat);
            ofDrawBitmapString(buf, 10, 20+i*20);
            //ofDrawBitmapString("mousestate: "+devices[i].ButtonState, 10, 40+i*60);
            for (int j=0; j<5; j++) {
                
                ofDrawBitmapString("touch: "
                                   + ofToString(j,0)
                                   + " = ("
                                   + ofToString(devices[i].mx[j],0)
                                   + ", "
                                   + ofToString(devices[i].my[j],0)
                                   + ") "
                                   + ofToString(devices[i].dim[j],0)
                                   + " piled: "
                                   + ofToString(devices[i].piled[j],0),
                                   
                                   600, 12*j+12);
            }
        }
    }
    */
    /*
    for(vector <Ring *>::iterator it = rings.begin(); it != rings.end(); ++it){
        (*it)->draw();
    }
    */
    
    
    string buf;
    /*
    buf = "listening for osc messages on port" + ofToString(PORT);
    ofDrawBitmapString(buf, 10, 20);
    
    // draw mouse state
    buf = "mouse: " + ofToString(mouseX, 4) +  " " + ofToString(mouseY, 4) + " ("+ofToString(field_val_t, 4)+")";
    ofDrawBitmapString(buf, 430, 20);
    ofDrawBitmapString(mouseButtonState, 580, 20);
    
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
    }
    */
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    xx=0;
    
    if( key == 's' ){
        for (int i=0; i<face_val; i++) {
            warper[i].toggleShow();
        }
    }else if(key=='f'){
        ofToggleFullscreen();
    }else if(key=='c'){
        meter_b=0;
        ringtime=30;
        getpoint=0;
        rhythm.play();
        cootime=ofGetElapsedTimeMillis();
        tempo=false;
        coo=true;
        ending=false;
        endingtime=0;
        
        for(int i=0;i<face_val;i++){
            effect_dim[i]=0;
            
            for(int j=0;j<movecircle[i].size();j++){
                movecircle[i][j]->dead=true;
            }
            ball_max[i]=2;
            ball_count[i]=0;
            
            ground[i].clear();
            groundLine[i].clear();
            for(int j=0;j<movecircle[i].size();j++){
                movecircle[i][j]->dead=true;
            }
            
        }
        
    }else if(key=='v'){
        meter_b=0;
        ringtime=0;
        getpoint=0;
        rhythm.stop();
        cootime=0;
        tempo=false;
        coo=false;
    }else if(key=='q'){
        //ring変更
        if(ringval==0){
            ringval=1;
        }else{
            ringval=0;
        }
    }
    
    
    //printf("%d\n",key);
    
    for(int i=0;i<face_val;i++){
        if( key == 49+i ){
            //printf("%d\n",i);
            
            if(effect_dim[i]!=3){
                effect_dim[i]++;
            }else{
                effect_dim[i]=0;
            }
            //printf("%d",effect_dim[i]);
            
            
            if(effect_dim[i]==3){
                box2d[i].setGravity(0, 40);
                
                groundSpeed[i] = 0.02;
                groundTimer[i] = 0;
                groundRes[i]   = 50;
                
                ground[i].clear();
                
                for(int j=0; j<groundRes[i]; j++) {
                    float x = ofMap(j, 0, groundRes[i]-1, 0, 250);
                    float y = 250/2;
                    groundLine[i].addVertex(x, y);
                }
                
            }else{
                ground[i].clear();
                groundLine[i].clear();
                for(int j=0;j<movecircle[i].size();j++){
                    movecircle[i][j]->dead=true;
                }
            }
            
            if(effect_dim[i]==2){
                // x, y方向の重力を設定
                box2d[i].setGravity(0, 0);
                
                for(int j=0;j<ball_max[i];j++){
                    float r = 15;
                    //Box2Dの円のインスタンスを生成
                    MoveCircle *c = new MoveCircle();
                    //物理特性の設定-重さ、反発力、摩擦力
                    c->setPhysics(0.85, 0.98, 0);
                    //世界に追加
                    c->setup(box2d[i].getWorld(), ofRandom(120, 130),ofRandom(120,130),r);
                    //c->setup(box2d[0].getWorld(), x, y, r);
                    //色
                    c->setccolor(ofColor(63,251,255));
                    //力をランダムに加える
                    float force = 500;
                    c->addForce(ofPoint(ofRandom(-force, force), ofRandom(-force, force)), 1);
                    //ベクターに追加
                    movecircle[i].push_back(c);
                    ball_count[i]++;
                }
            }else{
                for(int j=0;j<movecircle[i].size();j++){
                    movecircle[i][j]->dead=true;
                }
                ball_max[i]=2;
                ball_count[i]=0;
            }
            
            
            
        }
    }
    //printf("%d",xx);
    /*
    if(xx!=0&&xxx==0){
        bgm.play();
        xxx++;
    }
    if(xx==0){
        xxx=0;
        bgm.stop();
    }
    */
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//ナンバーを返す
int ofApp::device_num(string ip){
    int count=0;
    Boolean check=false;
    
    for(int i=0;i<devices.size();i++){
        if(ip==devices[i].ipAddress){
            count=i;
            check=true;
            devices[i].connected=true;
        }
    }
    if(check==false){
        Devices d;
        d.setipAddress(ip);
        d.connected=true;
        d.num=devices.size();
        devices.push_back(d);
        //printf("hoge");
        count=devices.size()-1;
    }
    return count;
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    curVol = 0.0;
    
    for (int i = 0; i < bufferSize; i++){
        curVol += input[i]*input[i]; // 聴覚上は出力の2乗に比例するので、2乗
    }
    
    curVol /= bufferSize; // 平均
}
