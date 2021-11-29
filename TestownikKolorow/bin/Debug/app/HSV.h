#pragma once

class KolorHSV{
    public:
        const int MaxH=360;
        const int MinH=0;
        const int MaxSV=1;
        const int MinSV=0;
        int UpLimitH,DownLimitH;
        double H,S,V;
        void setKolorHSV(double h,double s, double v);
        void addH(float value);
        void addS(float value);
        void addV(float value);
        bool isSMax();
        bool isVMax();
};

void HSVToRGB(KolorHSV hsv,int output[3]);
int angleToNum(float angle, int number);
