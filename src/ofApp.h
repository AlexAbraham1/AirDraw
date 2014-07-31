#pragma once

#include "ofMain.h"
#include "ofxCvHaarFinder.h"
#include "ofxTwitter.h"

#include "Circle.h"

class ofApp : public ofBaseApp{
    
public:
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed  (int key);
    void keyReleased (int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    void cleanupCircles();
    bool isBlankSpace(int x, int y);
    void switchCamera();
    void switchInvert();
    void resetCoordinates();
    bool colorFound();
    void saveImage();
    void saveImageTweet();
    void setColor(int x, int y);
    void changeBackground(bool selectedColor);
    void startDraw();
    void stopDraw();
    void stepBack();
    void stepForward();
    
    shared_ptr<ofVideoGrabber> grabber;
    
    ofImage image;
    
    bool setupMode;
    bool drawMode;
    
    
    std::vector<Circle*> circles;
    std::vector<float> drawHistory;
    std::vector< std::vector<Circle*> > previous;
    
    int circleRadius;
    int xMargin, yMargin;
    int maxTimesDrawn;
    float minRed, minGreen, minBlue, maxRed, maxGreen, maxBlue, avgRed, avgGreen, avgBlue;
    int xLow, xHigh, yLow, yHigh;
    
    int DEVICE_ID;
    
    int drawNumber;
    
    int screenWidth;
    int screenHeight;
    
    ofColor background;
    ofColor circleColor;
    
    ofTrueTypeFont verdana;
    
    
    ofxTwitter twitterClient;
    //ofxTwitterTweet tweet;
    int actualTweet;
};