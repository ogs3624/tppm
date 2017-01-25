//
//  Devices.h
//  pmApp_receiver
//
//  Created by kayayose on 2014/06/09.
//
//

#pragma once
#include "ofMain.h"

class Devices{
private:
    int z;
    
public:
    Devices();
    string ipAddress;
    string ButtonState;
    int mx[5],my[5],dim[5],dim_x[5],dim_y[5],piled[5],point[5];
    Boolean connected;
    int heartbeat=0;
    int heartbeat_=0;
    int num;
    ofColor color;
    int move_v=0;
    int effect_d=0;
    
    void setipAddress(string ip);
    void setdim();
    
};
