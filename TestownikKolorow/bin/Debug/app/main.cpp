#include <SFML/Graphics.hpp>
#include "HSV.h"
#include "DesktopResolution.h"
#include <math.h>
#include <wtypes.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <time.h>

using namespace sf;
using namespace std;

int main(int argc, char*argv[]){
    // =========================================== INIT ==========================================
    //Window of application
    /*
    int screenWidth=1920;
    int screenHeight=1080;
    GetDesktopResolution(screenWidth, screenHeight);
    RenderWindow window(VideoMode(screenWidth,screenHeight), "Pomiar kolorow",Style::Fullscreen);
    */
    Vector2f resolution;
    resolution.x=VideoMode::getDesktopMode().width;
    resolution.y=VideoMode::getDesktopMode().height;
    cout<<"Detected resolution: "<<resolution.x<<" x "<<resolution.y<<endl;
    RenderWindow window(VideoMode(resolution.x, resolution.y), "Pomiar kolorow",Style::Fullscreen);
    window.setMouseCursorVisible(false);

    //Variables
    enum class Side{
        Startup,LT,LP,PP,PT
    };
    Side side=Side::Startup;
    Clock clock;
    Font font;
    font.loadFromFile("fonts/dosis.otf");
    Text slideText;
    slideText.setFont(font);
    slideText.setFillColor(Color(255,255,255,50));
    slideText.setCharacterSize(20);
    slideText.setPosition(resolution.x/2-20,resolution.y-60);
    slideText.setString("Start");
    Text startupText;
    startupText.setFont(font);
    startupText.setFillColor(Color::White);
    startupText.setCharacterSize(50);
    startupText.setPosition(100,100);
    startupText.setLineSpacing(2);
    startupText.setString("Podczas pomiaru patrz na\nUstaw wszystkie kola na kolor czerwony\nMiedzy panelami przelaczaj sie za pomoca\nWybieraj kolo uzywajac\nZmieniaj kolor kola uzywajac\nNa koniec zapisz wyniki przyciskiem");

    const double PI=3.141592653589793238462643383279502884;
    int NUM_CIRCLES=8;
    int blurLevel=25;
    int blurRate=1;
    int colorMode=1;
    int edgeType=0;
    int pointMode=2;
    float treshold = 40;
    int pointerNum=-1;
    float *axis=nullptr;
    axis = new float[7];
    float crosshairScale=1;
    int scaleDirection=1;
    float faceDistance=0.30;
    float degRatio=2.*PI/360.;
    float realScreenHeight=28.5;

    //Joytick check
    if(Joystick::isConnected(0))
        cout<<"Controller connected. "<<Joystick::getButtonCount(0)<<" buttons aviable."<<endl;
    if(Joystick::hasAxis(0,Joystick::X))
        cout<<"X axis supported"<<endl;
    if(Joystick::hasAxis(0,Joystick::Y))
        cout<<"Y axis supported"<<endl;
    if(Joystick::hasAxis(0,Joystick::Z))
        cout<<"Z axis supported"<<endl;
    if(Joystick::hasAxis(0,Joystick::R))
        cout<<"R axis supported"<<endl;
    if(Joystick::hasAxis(0,Joystick::U))
        cout<<"U axis supported"<<endl;
    if(Joystick::hasAxis(0,Joystick::V))
        cout<<"V axis supported"<<endl;
    if(Joystick::hasAxis(0,Joystick::PovX))
        cout<<"PovX axis supported"<<endl;
    if(Joystick::hasAxis(0,Joystick::PovY))
        cout<<"PovY axis supported"<<endl;

    //Crosshair
    Texture textureCrosshair,textureLB,textureRB,textureLS,textureRS,textureStart;
    textureCrosshair.loadFromFile("graphics/crosshair.png");
    textureLB.loadFromFile("graphics/4.png");
    textureRB.loadFromFile("graphics/5.png");
    textureLS.loadFromFile("graphics/8.png");
    textureRS.loadFromFile("graphics/9.png");
    textureStart.loadFromFile("graphics/7.png");
    Sprite spriteCrosshair,spriteLB,spriteRB,spriteLS,spriteRS,spriteStart;
    spriteCrosshair.setTexture(textureCrosshair);
    spriteLB.setTexture(textureLB);
    spriteRB.setTexture(textureRB);
    spriteLS.setTexture(textureLS);
    spriteRS.setTexture(textureRS);
    spriteStart.setTexture(textureStart);
    float scale=0.022;
    spriteLB.setScale(scale,scale);
    spriteRB.setScale(scale,scale);
    spriteLS.setScale(scale,scale);
    spriteRS.setScale(scale,scale);
    spriteStart.setScale(scale,scale);
    spriteCrosshair.setOrigin(12,12);
    spriteLB.setPosition(960,350);
    spriteRB.setPosition(1070,350);
    spriteLS.setPosition(600,475);
    spriteRS.setPosition(720,600);
    spriteStart.setPosition(840,720);
    spriteCrosshair.setPosition(resolution.x/2,resolution.y/2);


    //Name of user to save data
    string imie="NoName";

    //C# Window application arguments
    //NUM_CIRCLES: liczba kol na pojedynczym slajdzie
    //blurLevel: liczba pixeli na ktorych kolor ma zaniknac od Max do 0
    //imie: Imie badanego
    //colorMode: 1-Full 0-RedOnly
    //edgeType: 1-Gauss 0-linear
    //pointMode: 1-pulsing 2-Rotating and pulsing 3-Rotating
    //faceDistance: odleglosc glowy badanego od ekranu
    if(argc>1){
        NUM_CIRCLES=atoi(argv[1]);
        blurLevel=atoi(argv[2]);
        imie=argv[3];
        colorMode=atoi(argv[4]);
        edgeType=atoi(argv[5]);
        pointMode=atoi(argv[6]);
        faceDistance=atof(argv[7]);
        realScreenHeight=atof(argv[8]);
    }
    cout<<"asdf"<<endl;
    cout<<faceDistance<<endl;
    //Distance variables
    float cmToPixRatio = resolution.y/realScreenHeight;
    float circlesSizeInCm=(tan(5.*degRatio)*faceDistance)*50;
    float sideCircleSizeInCm=(tan(1.*degRatio)*faceDistance)*50;
    float sideDistanceinCm=(tan(15.*degRatio)*faceDistance)*50;
    float distanceInCm=(tan(7.*degRatio)*faceDistance)*100+2*circlesSizeInCm;
    int sideCircleSizeInPx = cmToPixRatio*sideCircleSizeInCm;
    int circlesSizeInPx = cmToPixRatio*circlesSizeInCm;
    int distanceInPx = cmToPixRatio*distanceInCm;
    int sideDistanceinPx = cmToPixRatio*sideDistanceinCm;
    float pointersDistance = distanceInPx * 1.5;

    //Color Buffor
    int RGB[3];

    //Circles color randomize
    KolorHSV circlesLTColors[NUM_CIRCLES];
    KolorHSV circlesLPColors[NUM_CIRCLES];
    KolorHSV circlesPPColors[NUM_CIRCLES];
    KolorHSV circlesPTColors[NUM_CIRCLES];
    if(colorMode){
        for(int i=0;i<NUM_CIRCLES;i++){
            circlesLPColors[i].UpLimitH=360; circlesLPColors[i].DownLimitH=0;
            circlesLTColors[i].UpLimitH=360; circlesLTColors[i].DownLimitH=0;
            circlesPPColors[i].UpLimitH=360; circlesPPColors[i].DownLimitH=0;
            circlesPTColors[i].UpLimitH=360; circlesPTColors[i].DownLimitH=0;
        }
    }else{
        for(int i=0;i<NUM_CIRCLES;i++){
            circlesLPColors[i].UpLimitH=30; circlesLPColors[i].DownLimitH=330;
            circlesLTColors[i].UpLimitH=30; circlesLTColors[i].DownLimitH=330;
            circlesPPColors[i].UpLimitH=30; circlesPPColors[i].DownLimitH=330;
            circlesPTColors[i].UpLimitH=30; circlesPTColors[i].DownLimitH=330;
        }
    }
    for(int i=0;i<NUM_CIRCLES*4;i++){
        double tempS,tempV;
        srand(time(NULL)*rand());
        float tempVal=rand()%201/100.;
        if(tempVal<=1){
            tempS=tempVal;
            tempV=1;
        }else{
            tempS=1;
            tempV=tempVal-1;
        }
        if(colorMode){
            if(i<NUM_CIRCLES){
                circlesLTColors[i].setKolorHSV(180+(rand()%360-180),tempS,tempV);
            }else if(i<NUM_CIRCLES*2){
                circlesLPColors[i-NUM_CIRCLES].setKolorHSV(180+(rand()%360-180),tempS,tempV);
            }else if(i<NUM_CIRCLES*3){
                circlesPPColors[i-NUM_CIRCLES*2].setKolorHSV(180+(rand()%360-180),tempS,tempV);
            }else if(i<NUM_CIRCLES*4){
                circlesPTColors[i-NUM_CIRCLES*3].setKolorHSV(180+(rand()%360-180),tempS,tempV);
            }
        }else{
            if(i<NUM_CIRCLES){
                circlesLTColors[i].setKolorHSV((rand()%60-30),tempS,tempV);
            }else if(i<NUM_CIRCLES*2){
                circlesLPColors[i-NUM_CIRCLES].setKolorHSV((rand()%60-30),tempS,tempV);
            }else if(i<NUM_CIRCLES*3){
                circlesPPColors[i-NUM_CIRCLES*2].setKolorHSV((rand()%60-30),tempS,tempV);
            }else if(i<NUM_CIRCLES*4){
                circlesPTColors[i-NUM_CIRCLES*3].setKolorHSV((rand()%60-30),tempS,tempV);
            }
        }
    }

    //Circles
    CircleShape circles[NUM_CIRCLES][blurLevel];
    CircleShape pointingCircles[NUM_CIRCLES][blurLevel];
    CircleShape circleCenter[blurLevel];
    CircleShape circleSideCenter[blurLevel];
    for(int i=0;i<NUM_CIRCLES;i++){
        HSVToRGB(circlesLTColors[i],RGB);
        for(int j=0;j<blurLevel;j++){
            if(i==0){
                //Center circle
                circleCenter[j]=CircleShape(circlesSizeInPx+blurRate*j,512);
                if(edgeType){
                    circleCenter[j].setFillColor(Color(255,0,0,int(255*exp(-1*j*j/2/blurLevel))));
                }else{
                    circleCenter[j].setFillColor(Color(255,0,0,255-int(j*255/(blurLevel-1))));
                }
                circleCenter[j].setOrigin(circlesSizeInPx+blurRate*j,circlesSizeInPx+blurRate*j);
                circleCenter[j].setPosition(window.getSize().x/2,window.getSize().y/2);
                //Side center circle
                circleSideCenter[j]=CircleShape(sideCircleSizeInPx+blurRate*j,512);
                if(edgeType){
                    circleSideCenter[j].setFillColor(Color(255,0,0,int(100*exp(-1*j*j/2/blurLevel))));
                }else{
                    circleSideCenter[j].setFillColor(Color(255,0,0,100-int(j*255/(blurLevel-1))));
                }
                circleSideCenter[j].setOrigin(sideCircleSizeInPx+blurRate*j,sideCircleSizeInPx+blurRate*j);
                circleSideCenter[j].setPosition(window.getSize().x/2,window.getSize().y/2);
            }
            HSVToRGB(circlesLTColors[i],RGB);
            circles[i][j]=CircleShape(circlesSizeInPx+blurRate*j,512);
            if(edgeType){
                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
            }else{
                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
            }
            circles[i][j].setOrigin(circlesSizeInPx+blurRate*j,circlesSizeInPx+blurRate*j);
            circles[i][j].setPosition(circleCenter[0].getPosition().x+sin((i+0.5)*360./NUM_CIRCLES/180.*PI)*distanceInPx,circleCenter[0].getPosition().y-cos((i+0.5)*360./NUM_CIRCLES/180.*PI)*distanceInPx);
            //Pointers
            pointingCircles[i][j]=CircleShape(sideCircleSizeInPx/2+blurRate*j,512);
            if(edgeType){
                pointingCircles[i][j].setFillColor(Color(255,255,255,int(255*exp(-1*j*j/2/blurLevel))));
            }else{
                pointingCircles[i][j].setFillColor(Color(255,255,255,255-int(j*255/(blurLevel-1))));
            }
            pointingCircles[i][j].setOrigin(sideCircleSizeInPx/2+blurRate*j,sideCircleSizeInPx/2+blurRate*j);
            pointingCircles[i][j].setPosition(circleCenter[0].getPosition().x+sin((i+0.5)*360./NUM_CIRCLES/180.*PI)*pointersDistance,circleCenter[0].getPosition().y-cos((i+0.5)*360./NUM_CIRCLES/180.*PI)*pointersDistance);
        }
    }

    while(window.isOpen())
    {
        // =========================================== INPUT ==========================================
        Event event;
        while(window.pollEvent(event))
        {
            if(event.type == Event::Closed || Joystick::isButtonPressed(0,7) || Keyboard::isKeyPressed(Keyboard::Escape)){
                window.close();
            }
            if(side==Side::Startup){
                if(Joystick::isButtonPressed(0,4)){
                    side=Side::LP;
                    pointerNum=-1;
                    spriteCrosshair.setPosition(resolution.x/2,resolution.y/2);
                    slideText.setString("LP \nZaslonic prawe oko");
                    for(int i=0;i<NUM_CIRCLES;i++){
                        HSVToRGB(circlesLPColors[i],RGB);
                        for(int j=0;j<blurLevel;j++){
                            if(edgeType){
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                            }else{
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                            }
                        }
                    }
                }else if(Joystick::isButtonPressed(0,5)){
                    side=Side::PP;
                    pointerNum=-1;
                    spriteCrosshair.setPosition(resolution.x/2,resolution.y/2);
                    slideText.setString("PP \nZaslonic lewe oko");
                    for(int i=0;i<NUM_CIRCLES;i++){
                        HSVToRGB(circlesPPColors[i],RGB);
                        for(int j=0;j<blurLevel;j++){
                            if(edgeType){
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                            }else{
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                            }
                        }
                    }
                }
            }else if(side==Side::LT){
                if(Joystick::isButtonPressed(0,5)){
                    side=Side::LP;
                    pointerNum=-1;
                    spriteCrosshair.setPosition(resolution.x/2,resolution.y/2);
                    slideText.setString("LP \nZaslonic prawe oko");
                    for(int i=0;i<NUM_CIRCLES;i++){
                        HSVToRGB(circlesLPColors[i],RGB);
                        for(int j=0;j<blurLevel;j++){
                            if(edgeType){
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                            }else{
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                            }
                        }
                    }
                }
            }else if(side==Side::LP){
                if(Joystick::isButtonPressed(0,4)){
                    side=Side::LT;
                    pointerNum=-1;
                    spriteCrosshair.setPosition(resolution.x/2+sideDistanceinPx*2,resolution.y/2);
                    slideText.setString("LT \nZaslonic prawe oko");
                    for(int i=0;i<NUM_CIRCLES;i++){
                        HSVToRGB(circlesLTColors[i],RGB);
                        for(int j=0;j<blurLevel;j++){
                            if(edgeType){
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                            }else{
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                            }
                        }
                    }
                }else if(Joystick::isButtonPressed(0,5)){
                    side=Side::PP;
                    pointerNum=-1;
                    spriteCrosshair.setPosition(resolution.x/2,resolution.y/2);
                    slideText.setString("PP \nZaslonic lewe oko");
                    for(int i=0;i<NUM_CIRCLES;i++){
                        HSVToRGB(circlesPPColors[i],RGB);
                        for(int j=0;j<blurLevel;j++){
                            if(edgeType){
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                            }else{
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                            }
                        }
                    }
                }
            }else if(side==Side::PP){
                if(Joystick::isButtonPressed(0,4)){
                    side=Side::LP;
                    pointerNum=-1;
                    spriteCrosshair.setPosition(resolution.x/2,resolution.y/2);
                    slideText.setString("LP \nZaslonic prawe oko");
                    for(int i=0;i<NUM_CIRCLES;i++){
                        HSVToRGB(circlesLPColors[i],RGB);
                        for(int j=0;j<blurLevel;j++){
                            if(edgeType){
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                            }else{
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                            }
                        }
                    }
                }else if(Joystick::isButtonPressed(0,5)){
                    side=Side::PT;
                    pointerNum=-1;
                    spriteCrosshair.setPosition(resolution.x/2-sideDistanceinPx*2,resolution.y/2);
                    slideText.setString("PT \nZaslonic lewe oko");
                    for(int i=0;i<NUM_CIRCLES;i++){
                        HSVToRGB(circlesPTColors[i],RGB);
                        for(int j=0;j<blurLevel;j++){
                            if(edgeType){
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                            }else{
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                            }
                        }
                    }
                }
            }else if(side==Side::PT){
                if(Joystick::isButtonPressed(0,4)){
                    side=Side::PP;
                    pointerNum=-1;
                    spriteCrosshair.setPosition(resolution.x/2,resolution.y/2);
                    slideText.setString("PP \nZaslonic lewe oko");
                    for(int i=0;i<NUM_CIRCLES;i++){
                        HSVToRGB(circlesPPColors[i],RGB);
                        for(int j=0;j<blurLevel;j++){
                            if(edgeType){
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                            }else{
                                circles[i][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                            }
                        }
                    }
                }
            }
        }
        axis[0]=Joystick::getAxisPosition(0,Joystick::X); //LS x
        axis[1]=Joystick::getAxisPosition(0,Joystick::Y); //LS y
        axis[2]=Joystick::getAxisPosition(0,Joystick::Z); //RT/LT
        axis[3]=Joystick::getAxisPosition(0,Joystick::U); //RS x
        axis[4]=Joystick::getAxisPosition(0,Joystick::V); //RS y
        axis[5]=Joystick::getAxisPosition(0,Joystick::PovX); //Dpad x
        axis[6]=Joystick::getAxisPosition(0,Joystick::PovY); //Dpad y reverse

        // =========================================== CHANGE ==========================================
        //When we use left stick
        Time dt=clock.restart();
        float dtAsSeconds=dt.asSeconds();
        if(side==Side::Startup){
            circleCenter[0].setPosition(980,260);
            spriteCrosshair.setPosition(670,135);
        }else{
            circleCenter[0].setPosition(resolution.x/2,resolution.y/2);
        }
        if(side==Side::LT || side==Side::LP || side==Side::PP || side==Side::PT){
            if(fabs(axis[0])>treshold || fabs(axis[1])>treshold || pointerNum>=0){
                if(fabs(axis[0])>treshold || fabs(axis[1])>treshold){
                    float angle=(atan2(-axis[0],axis[1])*180.)/PI+180;
                    int selectedCircle=angleToNum(angle,NUM_CIRCLES);
                    pointerNum=selectedCircle;
                }
                if(fabs(axis[3])>treshold){
                    if(side==Side::LT){
                        if(circlesLTColors[pointerNum].isSMax()){
                            circlesLTColors[pointerNum].addV(-axis[3]*dtAsSeconds/200);
                        }
                        if(circlesLTColors[pointerNum].isVMax()){
                            circlesLTColors[pointerNum].addS(axis[3]*dtAsSeconds/200);
                        }
                    }else if(side==Side::LP){
                        if(circlesLPColors[pointerNum].isSMax()){
                            circlesLPColors[pointerNum].addV(-axis[3]*dtAsSeconds/200);
                        }
                        if(circlesLPColors[pointerNum].isVMax()){
                            circlesLPColors[pointerNum].addS(axis[3]*dtAsSeconds/200);
                        }
                    }else if(side==Side::PP){
                        if(circlesPPColors[pointerNum].isSMax()){
                            circlesPPColors[pointerNum].addV(-axis[3]*dtAsSeconds/200);
                        }
                        if(circlesPPColors[pointerNum].isVMax()){
                            circlesPPColors[pointerNum].addS(axis[3]*dtAsSeconds/200);
                        }
                    }else if(side==Side::PT){
                        if(circlesPTColors[pointerNum].isSMax()){
                            circlesPTColors[pointerNum].addV(-axis[3]*dtAsSeconds/200);
                        }
                        if(circlesPTColors[pointerNum].isVMax()){
                            circlesPTColors[pointerNum].addS(axis[3]*dtAsSeconds/200);
                        }
                    }
                }
                if(fabs(axis[4])>treshold){
                    if(side==Side::LT){
                        circlesLTColors[pointerNum].addH(axis[4]*dtAsSeconds/2);
                    }else if(side==Side::LP){
                        circlesLPColors[pointerNum].addH(axis[4]*dtAsSeconds/2);
                    }else if(side==Side::PP){
                        circlesPPColors[pointerNum].addH(axis[4]*dtAsSeconds/2);
                    }else if(side==Side::PT){
                        circlesPTColors[pointerNum].addH(axis[4]*dtAsSeconds/2);
                    }
                    circlesLTColors[pointerNum].addH(axis[4]*dtAsSeconds/2);
                }
                if(side==Side::LT){
                    HSVToRGB(circlesLTColors[pointerNum],RGB);
                }else if(side==Side::LP){
                    HSVToRGB(circlesLPColors[pointerNum],RGB);
                }else if(side==Side::PP){
                    HSVToRGB(circlesPPColors[pointerNum],RGB);
                }else if(side==Side::PT){
                    HSVToRGB(circlesPTColors[pointerNum],RGB);
                }
                for(int j=0;j<blurLevel;j++){
                    if(edgeType){
                        circles[pointerNum][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],int(255*exp(-1*j*j/2/blurLevel))));
                    }else{
                        circles[pointerNum][j].setFillColor(Color(RGB[0],RGB[1],RGB[2],255-int(j*255/(blurLevel-1))));
                    }
                }
            }
        }
        if(pointMode<=2){
            if(crosshairScale>1.5){
                scaleDirection=-1;
            }
            if(crosshairScale<1){
                scaleDirection=1;
            }
            crosshairScale+=scaleDirection*dtAsSeconds;
            spriteCrosshair.setScale(crosshairScale,crosshairScale);
        }
        if(pointMode>=2){
            spriteCrosshair.rotate(dtAsSeconds*90);
        }

        // =========================================== DRAW ==========================================
        window.clear();
        if(side!=Side::Startup){
            for(int i=0;i<NUM_CIRCLES;i++){
                for(int j=blurLevel-1;j>=0;j--){
                    window.draw(circles[i][j]);
                    if(i==0){
                        if(side==Side::LP || side==Side::PP){
                            window.draw(circleCenter[j]);
                        }else if(side==Side::LT || side==Side::PT){
                            window.draw(circleSideCenter[j]);
                        }
                        if(pointerNum>=0){
                            window.draw(pointingCircles[pointerNum][j]);
                        }
                    }
                }
            }
        }else{
            window.draw(circleCenter[0]);
            window.draw(startupText);
            window.draw(spriteLB);
            window.draw(spriteRB);
            window.draw(spriteLS);
            window.draw(spriteRS);
            window.draw(spriteStart);
        }
        window.draw(slideText);
        window.draw(spriteCrosshair);
        window.display();
    }

    time_t now = time(0);
    tm *ltm = localtime(&now);

    stringstream ss;
    ss<<"wyniki/"<<1900+ltm->tm_year<<"_"<<1+ltm->tm_mon<<"_"<<ltm->tm_mday<<","<<ltm->tm_hour<<";"<<ltm->tm_min<<";"<<ltm->tm_sec<<";"<<imie<<";"<<colorMode<<";"<<edgeType<<";"<<pointMode<<".dat";

    ofstream plik;
    cout<<ss.str()<<endl;

    plik.open(ss.str().c_str(),ios::out);
    if(plik.good()){
        plik<<"LT:"<<endl;
        for(int i=0;i<NUM_CIRCLES;i++){
            plik<<"Kolo "<<i<<": "<<circlesLTColors[i].H<<" "<<circlesLTColors[i].S<<" "<<circlesLTColors[i].V<<endl;
        }
        plik<<"LP:"<<endl;
        for(int i=0;i<NUM_CIRCLES;i++){
            plik<<"Kolo "<<i<<": "<<circlesLPColors[i].H<<" "<<circlesLPColors[i].S<<" "<<circlesLPColors[i].V<<endl;
        }
        plik<<"PP:"<<endl;
        for(int i=0;i<NUM_CIRCLES;i++){
            plik<<"Kolo "<<i<<": "<<circlesPPColors[i].H<<" "<<circlesPPColors[i].S<<" "<<circlesPPColors[i].V<<endl;
        }
        plik<<"PT:"<<endl;
        for(int i=0;i<NUM_CIRCLES;i++){
            plik<<"Kolo "<<i<<": "<<circlesPTColors[i].H<<" "<<circlesPTColors[i].S<<" "<<circlesPTColors[i].V<<endl;
        }
        plik<<"Zakres kolorow: ";
        if(colorMode){
            plik<<"pelen"<<endl;
        }else{
            plik<<"czerwien"<<endl;
        }
        plik<<"Tryb zanikania krawedzi: ";
        if(edgeType){
            plik<<"Gauss"<<endl;
        }else{
            plik<<"liniowy"<<endl;
        }
        plik<<"Tryb punktu fiksacyjnego: ";
        if(pointMode==1){
            plik<<"pulsujacy"<<endl;
        }else if(pointMode==2){
            plik<<"pulsujacy i rotujacy"<<endl;
        }else if(pointMode==3){
            plik<<"rotujacy"<<endl;
        }
        plik<<"Imie badanego: "<<imie<<endl;
    }else{
        cout<<"Nie mozna zapisac wynikow."<<endl;
    }

    plik.close();

    return 0;
}

/*
                    if(side==Side::LT){

                    }else if(side==Side::LP){

                    }else if(side==Side::PP){

                    }else if(side==Side::PT){

                    }
*/

/*
    Vector2f resolution;
    resolution.x=VideoMode::getDesktopMode().width;
    resolution.y=VideoMode::getDesktopMode().height;
    RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena",Style::Fullscreen);
*/
