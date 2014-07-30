#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(255, 255, 255);
    ofSetLogLevel (OF_LOG_VERBOSE);
    
    screenWidth = 960;
    screenHeight = 720;
    
    grabber = shared_ptr<ofVideoGrabber>(new ofVideoGrabber());
    grabber->setPixelFormat(OF_PIXELS_RGB);
    DEVICE_ID = grabber->listDevices().size() - 1;
    grabber->setDeviceID(DEVICE_ID);
    grabber->initGrabber(screenWidth, screenHeight);
    
    image.allocate(screenWidth, screenHeight, OF_IMAGE_COLOR);
    
    firstRun = true;
    
    minRadius = 10;
    maxRadius = 30;
    
    maxTimesDrawn = 1000;
    
    minRed = 215;
    minGreen = 95;
    minBlue = 155;
    
    maxRed = 255;
    maxGreen = 135;
    maxBlue = 195;
    
    drawMode = false;
    resetCoordinates();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    grabber->update();
    if (grabber->isFrameNew()) {
        
        resetCoordinates();
        
        image.setFromPixels(grabber->getPixelsRef());
        image.mirror(false, true);
        
        
        for (int x = 0; x < screenWidth; x++) {
            for (int y = 0; y < screenHeight; y++) {
                ofColor color = image.getColor(x,y);
                float red = float(color.r);
                float green = float(color.g);
                float blue = float(color.b);
                
                if (ofInRange(red, minRed, maxRed) &&
                    ofInRange(green, minGreen, maxGreen) &&
                    ofInRange(blue, minBlue, maxBlue)) {
                    
                    
                    if (x < xLow) {
                        xLow = x;
                    }
                    if (x > xHigh) {
                        xHigh = x;
                    }
                    if (y < yLow) {
                        yLow = y;
                    }
                    if (y > yHigh) {
                        yHigh = y;
                    }
                    
                }
                
                
            }
        }
        
        if (drawMode) {
            if (colorFound()) {
                int x = (xLow + xHigh)/2;
                int y = (yLow + yHigh)/2;
                int radius = 5;
                int z = 50;
                ofColor color = ofColor(255,105,180);
                
                Circle * circle = new Circle();
                
                circle->x = x;
                circle->y = y;
                circle->z = z;
                circle->radius = radius;
                circle->color = color;
                
                circles.push_back(circle);
            }
        }
        
        
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofSetHexColor(0x000000);
    ofRect(0, 0, screenWidth, screenHeight);
    
    ofSetHexColor(0xFF69B4);
    
    
    for (std::vector<Circle>::size_type i = 0; i != circles.size(); i++) {
        Circle * circle = circles[i];
        circle->drawCircle();
    }
    
    if (colorFound()) {
        int x = (xLow + xHigh)/2;
        int y = (yLow + yHigh)/2;
        int radius = 5;
        int z = 50;
        ofPushStyle();
        ofSetColor(255, 105, 180);
        ofCircle(x, y, z, radius);
        ofPopStyle();
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    switch (key){
        case ' ':
            drawMode = !drawMode;
            break;
        case 'c':
            drawMode = false;
            circles.clear();
            break;
        case 's':
            saveImage();
            break;
    }
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

void ofApp::cleanupCircles() {
    ofLog() << "cleanupCircles() started";
    stringstream ss;
    ss << circles.size();
    ofLog() << "NUMBER OF CIRCLES: " + ss.str();
    std::random_shuffle(circles.begin(), circles.end());
    ofLog() << "cleanupCircles() ended";
    ss.str("");
    ss << circles.size();
    ofLog() << "NUMBER OF CIRCLES: " + ss.str();
}

bool ofApp::isBlankSpace(int x, int y) {
    for (std::vector<Circle>::size_type i = 0; i != circles.size(); i++) {
        
        Circle * circle = circles[i];
        
        
        
        float x2 = static_cast<int>(pow(static_cast<double>(circle->x - x), 2)+.005);
        float y2 = static_cast<int>(pow(static_cast<double>(circle->y - y), 2)+.005);
        float radius2 = static_cast<int>(pow(static_cast<double>(circle->radius), 2)+.005);
        
        if ((x2 + y2) < (radius2)/2) {
            return false;
        }
        
    }
    return true;
    
}

void ofApp::fixZIndex() {
    for (std::vector<Circle>::size_type i = 0; i != circles.size(); i++) {
        Circle * circle = circles[i];
        
        float smallRadius = (minRadius + ((minRadius + maxRadius)/2))/2;
        if (circle->radius <= smallRadius) {
            circle->z = 50;
        }
    }
}

void ofApp::resetCoordinates() {
    xLow = screenWidth;
    xHigh = 0;
    yLow = screenHeight;
    yHigh = 0;
}

bool ofApp::colorFound() {
    return (xLow != screenWidth && xHigh != 0 && yLow != screenHeight && yHigh != 0);
}

void ofApp::saveImage() {
    stringstream ss;
    ss << ofGetElapsedTimef();
    ofSaveScreen(ofFilePath::getUserHomeDir() + "/Desktop/" + ss.str() + ".jpg");
}