#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofBackground(255, 255, 255);
    ofSetLogLevel (OF_LOG_VERBOSE);
    
    screenWidth = 640;
    screenHeight = 480;
    
    verdana.loadFont("verdana.ttf", 12);
    
    grabber = shared_ptr<ofVideoGrabber>(new ofVideoGrabber());
    grabber->setPixelFormat(OF_PIXELS_RGB);
    DEVICE_ID = grabber->listDevices().size() - 1;
    grabber->setDeviceID(DEVICE_ID);
    grabber->initGrabber(screenWidth, screenHeight);
    
    image.allocate(screenWidth, screenHeight, OF_IMAGE_COLOR);
    
    setupMode = true;
    
    circleRadius = 5;
    xMargin = 15;
    yMargin = 15;
    
    maxTimesDrawn = 1000;
    
    minRed = minGreen = minBlue = 255;
    
    maxRed = maxGreen = maxBlue = 0;
    
    avgRed = avgGreen = avgBlue = 0;
    
    background = ofColor(0,0,0);
    circleColor = ofColor(avgRed, avgGreen, avgBlue);
    
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
        
        if (!setupMode) {
            
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
                    
                    if (x < circleRadius) {
                        x = circleRadius;
                    }
                    if (y < circleRadius + yMargin) {
                        y = circleRadius + yMargin;
                    }
                    if (x > screenWidth - circleRadius - xMargin) {
                        x = screenWidth - circleRadius - xMargin;
                    }
                    if (y > screenHeight - circleRadius - yMargin) {
                        y = screenHeight - circleRadius - yMargin;
                    }
                    
                    int z = 50;
                    Circle * circle = new Circle();
                    
                    circle->x = x;
                    circle->y = y;
                    circle->z = z;
                    circle->radius = circleRadius;
                    circle->color = circleColor;
                    
                    circles.push_back(circle);
                }
            }
            
        }
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (setupMode) {
        ofSetHexColor(0xFFFFFF);
        image.draw(0,0);
        
        ofPushStyle();
        ofSetHexColor(0x000000);
        verdana.drawString("Hold up the object you want to use and click on it.", 20, screenHeight + 25);
        verdana.drawString("Make sure the object is a unique color.", 20, screenHeight + 45);
        ofSetHexColor(0xFF0000);
        verdana.drawString("'ESC': Quit AirDraw", 20, screenHeight + 70);
        ofPopStyle();
    } else {
        ofSetColor(background);
        ofRect(0, 0, screenWidth, screenHeight);
        
        ofSetColor(circleColor);
        
        for (std::vector<Circle>::size_type i = 0; i != circles.size(); i++) {
            Circle * circle = circles[i];
            circle->drawCircle();
        }
        
        if (colorFound()) {
            int x = (xLow + xHigh)/2;
            int y = (yLow + yHigh)/2;
            
            if (x < circleRadius) {
                x = circleRadius;
            }
            if (y < circleRadius) {
                y = circleRadius;
            }
            if (x > screenWidth - circleRadius - xMargin) {
                x = screenWidth - circleRadius - xMargin;
            }
            if (y > screenHeight - circleRadius - yMargin) {
                y = screenHeight - circleRadius - yMargin;
            }
            
            int radius = circleRadius;
            int z = 50;
            ofPushStyle();
            ofSetColor(circleColor);
            ofCircle(x, y, z, radius);
            ofPopStyle();
        }
        
        
        ofSetHexColor(0x000000);
        verdana.drawString("Hold up the selected object to webcam and draw!", 20, screenHeight + 25);
        verdana.drawString("'SPACE': Toggle Draw Mode", 20, screenHeight + 45);
        verdana.drawString("'C': Clear Drawing", 20, screenHeight + 65);
        verdana.drawString("'S': Save Drawing (Desktop)", 20, screenHeight + 85);
        verdana.drawString("'RIGHT': Change Background (Black/White)", 20, screenHeight + 105);
        verdana.drawString("'LEFT': Change Background (Selected Color)", 20, screenHeight + 125);
        verdana.drawString("'UP/DOWN': Change Circle Radius", 20, screenHeight + 145);
        verdana.drawString("'z': Step Back", 20, screenHeight + 165);
        ofSetHexColor(0xFF0000);
        verdana.drawString("'ESC': Quit AirDraw", 20, screenHeight + 190);
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    
    switch (key){
        case ' ':
            drawMode ? stopDraw() : startDraw();
            break;
        case 'c':
            stopDraw();
            circles.clear();
            break;
        case 's':
            saveImage();
            break;
        case OF_KEY_RIGHT:
            changeBackground(false);
            break;
        case OF_KEY_LEFT:
            changeBackground(true);
            break;
        case OF_KEY_UP:
            if (circleRadius+5 < (screenHeight/2 - xMargin)) {
                circleRadius += 5;
            }
            break;
        case OF_KEY_DOWN:
            if (circleRadius >= 10) {
                circleRadius -= 5;
            }
            break;
        case 'z':
            stepBack();
            break;
        case 'x':
            stepForward();
            break;
    }
}

void ofApp::keyReleased(int key) {

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
    if (setupMode) {
        setColor(x, y);
        setupMode = false;
        
    } else {
        drawMode = false;
        setupMode = true;
    }
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
    string filename =ofFilePath::getUserHomeDir() + "/Desktop/" + ss.str() + ".jpg";
    ofSaveScreen(filename);
    image.loadImage(filename);
    image.crop(0, 0, screenWidth, screenHeight);
    image.saveImage(filename);
}

void ofApp::setColor(int x, int y) {
    xLow = x - 5;
    xHigh = x + 5;
    yLow = y - 5;
    yHigh = y + 5;
    
    minRed = 255;
    minGreen = 255;
    minBlue = 255;
    maxRed = 0;
    maxGreen = 0;
    maxBlue = 0;
    
    for (int a = xLow; a < xHigh; a++) {
        for (int b = yLow; b < yHigh; b++) {
            ofColor color = image.getColor(a,b);
            float red = float(color.r);
            float green = float(color.g);
            float blue = float(color.b);
            
            if (red < minRed) {
                minRed = red;
            }
            if (green < minGreen) {
                minGreen = green;
            }
            if (blue < minBlue) {
                minBlue = blue;
            }
            
            if (red > maxRed) {
                maxRed = red;
            }
            if (green > maxGreen) {
                maxGreen = green;
            }
            if (blue > maxBlue) {
                maxBlue = blue;
            }
            
            
        }
    }
    
    avgRed = (minRed + maxRed)/2;
    avgGreen = (minGreen + maxGreen)/2;
    avgBlue = (minBlue + maxBlue)/2;
    
    minRed = avgRed - 15;
    minGreen = avgGreen - 15;
    minBlue = avgBlue - 15;
    
    maxRed = avgRed + 15;
    maxGreen = avgGreen + 15;
    maxBlue = avgBlue + 15;
    
    circleColor = ofColor(avgRed, avgGreen, avgBlue);
    
    ofLogNotice() << "minRed: " << minRed << ", minGreen: " << minGreen << ", minBlue: " << minBlue;
    ofLogNotice() << "maxRed: " << maxRed << ", maxGreen: " << maxGreen << ", maxBlue: " << maxBlue;
    
    resetCoordinates();
}

void ofApp::changeBackground(bool selectedColor) {
    if (selectedColor) {
        background = circleColor;
    } else {
        background == ofColor(255,255,255) ? background = ofColor(0,0,0) : background = ofColor(255,255,255);
    }
    
}

void ofApp::startDraw() {
    if (!setupMode && !drawMode) {
        drawMode = true;
    }
}

void ofApp::stopDraw() {
    
    drawMode = false;
    float drawID = ofGetElapsedTimef();
    for (std::vector<Circle>::size_type i = 0; i != circles.size(); i++) {
        Circle * circle = circles[i];
        
        if (circle->drawID == 0) {
            circle ->drawID = drawID;
        }
    }
    drawHistory.push_back(drawID);
    previous.clear();
    
}

void ofApp::stepBack() {
    
    if (drawHistory.size() > 0) {
        float drawID = drawHistory[drawHistory.size() - 1];
        std::vector<Circle*> deletedCircles;
        
        for(std::vector<Circle*>::iterator circle = circles.begin(); circle != circles.end();) {
            if ((*circle)->drawID == drawID) {
                deletedCircles.push_back(*circle);
                circle = circles.erase(circle);
            }
            else {
                ++ circle;
            }
        }
        drawHistory.pop_back();
        previous.push_back(deletedCircles);
        ofLogNotice() << "Previous Size: " << previous.size();
    }
}

void ofApp::stepForward() {
    if (previous.size() > 0) {
        std::vector<Circle*> deletedCircles = previous[previous.size() - 1];
        float drawID = -1;
        for(std::vector<Circle*>::iterator circle = deletedCircles.begin(); circle != deletedCircles.end(); ++circle) {
            circles.push_back(*circle);
            
            if (drawID == -1) {
                drawID = (*circle)->drawID;
            }
            
        }
        previous.pop_back();
        drawHistory.push_back(drawID);
    }
}