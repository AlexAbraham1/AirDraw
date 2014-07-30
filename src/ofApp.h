#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"

#include "Circle.h"

class ofApp : public ofBaseApp{
    
public:
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void cleanupCircles();
    bool isBlankSpace(int x, int y);
    void switchCamera();
    void switchInvert();
    void fixZIndex();
    void resetCoordinates();
    bool colorFound();
    void saveImage();
    
    shared_ptr<ofVideoGrabber> grabber;
    
    ofImage image;
    
    
    bool firstRun;
    bool invertColors;
    bool drawMode;
    
    
    
    std::vector<Circle*> circles;
    
    int minRadius;
    int maxRadius;
    int maxTimesDrawn;
    float minRed, minGreen, minBlue, maxRed, maxGreen, maxBlue;
    int xLow, xHigh, yLow, yHigh;
    
    int DEVICE_ID;
    
    int screenWidth;
    int screenHeight;
    int haarWidth;
    int haarHeight;
    
    int orientation;
    
};