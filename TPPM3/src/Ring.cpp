//
//  Ring.cpp
//  pmApp_receiver
//
//  Created by kayayose on 2014/06/11.
//
//

#include "Ring.h"

Ring::Ring(ofPoint _pos,int dim,int de,int ri){
    pos = _pos;
    radius = 0;
    radiusSpeed = 2;
    dead = false;
    dimention=dim;
    dev=de;
    ringval=ri;
}

void Ring::update(){
    radius += radiusSpeed;
    
    if(ringval==0){
        if (radius > 100) {
            dead = true;
        }
    }else{
        if (radius > 80) {
            dead = true;
        }
    }
    
}

void Ring::draw(){
    ofSetCircleResolution(64);
    ofEnableSmoothing();
    ofSetLineWidth(3);
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    if(ringval==0){
        ofNoFill();
    }
    if(ringval==0){
        ofSetColor(ring_color, 255 - radius * 127 / 75);
    }else{
        ofSetColor(ring_color, 180 - radius * 127 / 75);
    }
    ofCircle(0, 0, radius);
    ofPopMatrix();
    ofSetLineWidth(1);
}

void Ring::setcolor_ring(ofColor col){
    ring_color=col;
}
