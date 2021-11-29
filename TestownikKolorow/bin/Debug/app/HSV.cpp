#include "HSV.h"
#include <cmath>

void KolorHSV::setKolorHSV(double h,double s, double v){
    if(h<MinH){
        h+=MaxH;
    }
    H=h;
    S=s;
    V=v;
}

void KolorHSV::addH(float value){
    H+=value;
    //przejscie 0-360
    if(H>MaxH){
        H-=MaxH;
    }
    if(H<MinH){
        H+=MaxH;
    }
    //limit czerwienii
    if(UpLimitH!=MaxH && DownLimitH!=MinH){
        if(H<DownLimitH && H>MaxH/2){
            H=DownLimitH;
        }else if(H>UpLimitH && H<MaxH/2){
            H=UpLimitH;
        }
    }
}

void KolorHSV::addS(float value){
    S+=value;
    if(S>MaxSV){
        S=MaxSV;
    }
    if(S<MinSV){
        S=MinSV;
    }
}

void KolorHSV::addV(float value){
    V+=value;
    if(V>MaxSV){
        V=MaxSV;
    }
    if(V<MinSV){
        V=MinSV;
    }
}

bool KolorHSV::isSMax(){
    if (S==MaxSV){
        return true;
    }else{
        return false;
    }
}

bool KolorHSV::isVMax(){
    if (V==MaxSV){
        return true;
    }else{
        return false;
    }
}

void HSVToRGB(KolorHSV hsv,int output[3]) {
    double c = 0.0, m = 0.0, x = 0.0;
    double h=hsv.H;
    c = hsv.V * hsv.S;
    x = c * (1.0 - fabs(fmod(hsv.H / 60.0, 2) - 1.0));
    m = hsv.V - c;
    double r,g,b;

    if (h >= 0.0 && h < 60.0)
    {
        r=c+m;
        g=x+m;
        b=m;
    }
    else if (h >= 60.0 && h < 120.0)
    {
        r=x+m;
        g=c+m;
        b=m;
    }
    else if (h >= 120.0 && h < 180.0)
    {
        r=m;
        g=c+m;
        b=x+m;
    }
    else if (h >= 180.0 && h < 240.0)
    {
        r=m;
        g=x+m;
        b=c+m;
    }
    else if (h >= 240.0 && h < 300.0)
    {
        r=x+m;
        g=m;
        b=c+m;
    }
    else if (h >= 300.0 && h < 360.0)
    {
        r=c+m;
        g=m;
        b=x+m;
    }
    else
    {
        r=m;
        g=m;
        b=m;
    }

	output[0] = int(r* 255);
	output[1] = int(g* 255);
	output[2] = int(b* 255);
}

int angleToNum(float angle, int number){
    float part=360/number;
    for(int i=0;i<number;i++){
        if(angle>i*part && angle<=(i+1)*part){
            return i;
        }
    }
    return -1;
}



























