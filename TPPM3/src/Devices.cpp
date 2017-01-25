//
//  Devices.m
//  pmApp_receiver
//
//  Created by kayayose on 2014/06/09.
//
//

#import "Devices.h"


//constractor
Devices::Devices(){
    for (int i=0; i<5; i++) {
        mx[i]=-100;
        my[i]=-100;
        dim_x[i]=-100;
        dim_y[i]=-100;
        dim[i]=0;
        piled[i]=0;
        point[i]=0;
    }
    ButtonState="up";
    color.r=31; color.g=63; color.b=200; color.a=255;
    connected=true;
    effect_d=0;
}

void Devices::setipAddress(string ip){
    ipAddress=ip;
}

void Devices::setdim(){
    //printf("%d,%d\n",mx[0],my[0]);
    for(int i=0;i<5;i++){
        
        if(mx[i]>387&&mx[i]<637&&my[i]>9&&my[i]<259){
            dim[i]=0;
            dim_x[i]=mx[i]-387;
            dim_y[i]=my[i]-9;
        }
        else if(mx[i]>262&&mx[i]<512&&my[i]>259&&my[i]<509){
            dim[i]=1;
            dim_x[i]=mx[i]-262;
            dim_y[i]=my[i]-259;
        }
        else if(mx[i]>512&&mx[i]<762&&my[i]>259&&my[i]<509){
            dim[i]=2;
            dim_x[i]=mx[i]-512;
            dim_y[i]=my[i]-259;
        }
        else if(mx[i]>137&&mx[i]<387&&my[i]>509&&my[i]<759){
            dim[i]=3;
            dim_x[i]=mx[i]-137;
            dim_y[i]=my[i]-509;
        }
        else if(mx[i]>387&&mx[i]<637&&my[i]>509&&my[i]<759){
            dim[i]=4;
            dim_x[i]=mx[i]-387;
            dim_y[i]=my[i]-509;
        }
        else if(mx[i]>637&&mx[i]<887&&my[i]>509&&my[i]<759){
            dim[i]=5;
            dim_x[i]=mx[i]-637;
            dim_y[i]=my[i]-509;
        }else{
            dim[i]=0;
            dim_x[i]=-100;
            dim_y[i]=-100;
        }
        /*
        for(int j=0;j<6;j++){
            if(mx[i]>50+(j/2)*250&&mx[i]<50+(j/2+1)*250
               &&my[i]>120+(j%2)*250&&my[i]<120+(j%2+1)*250){
                dim[i]=j;
                dim_x[i]=mx[i]-(50+(j/2)*250);
                dim_y[i]=my[i]-(120+(j%2)*250);
                //printf("%d\n",mx[i]-(212+(j%2)*300));
            }
        }
        */
        /*
        if(mx[i]>50&&mx[i]<800&&my[i]>120&&my[i]<670){
            
        }else{
            dim_x[i]=-100;
            dim_y[i]=-100;
        }
        */
    }
}

