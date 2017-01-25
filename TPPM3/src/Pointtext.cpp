//
//  Pointtext.cpp
//  pmApp_receiver
//
//  Created by kayayose on 2014/06/11.
//
//

#include "Pointtext.h"

Pointtext::Pointtext(ofPoint _pos,int dim,int de,int poi){
    pos = _pos;
    radius = 0;
    radiusSpeed = 2;
    dead = false;
    dimention=dim;
    dev=de;
    point=poi;
}

void Pointtext::update(){
    radius += radiusSpeed;
    if (radius > 100) {
        dead = true;
    }
}

void Pointtext::draw(){
    ofSetColor(ring_color, 255 - radius * 127 / 75);
    string str;
    if(point<75){
        ofSetColor(ofColor(255,0,0), 255 - radius * 127 / 75);
        str="Excellent!!";
        ofDrawBitmapString(str,pos.x-40,pos.y-8);
    }else if(point>=75&&point<150){
        ofSetColor(ofColor(255,129,25), 255 - radius * 127 / 75);
        str="Great!";
        ofDrawBitmapString(str,pos.x-20,pos.y-8);
    }else if(point>=150&&point<300){
        ofSetColor(ofColor(0,255,0), 255 - radius * 127 / 75);
        str="Good";
        ofDrawBitmapString(str,pos.x-16,pos.y-8);
    }else{
        ofSetColor(ofColor(0,0,255), 255 - radius * 127 / 75);
        str="Miss...";
        ofDrawBitmapString(str,pos.x-25,pos.y-8);
    }
    
    
    //sstr= ofToString(point);
    
}

void Pointtext::setcolor_ring(ofColor col){
    ring_color=col;
}
