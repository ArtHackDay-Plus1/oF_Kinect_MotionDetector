#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetFrameRate(30);
    kinect.setup();
    kinect.setRegister(true);
    kinect.addDepthGenerator();
    kinect.start();
    horizonY = ofGetHeight()/2;

    for (int x = 0; x < ofGetWidth()/2; x++)
        backZ[x] = 10000;
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    unsigned short *depthData = kinect.getDepthRawPixels().getData();
    for (int x = 0; x < ofGetWidth()/2; x++) {
        XnProject[x].X = x;
        XnProject[x].Y = horizonY;
        XnProject[x].Z = depthData[horizonY * ofGetWidth()/2 + x];
        
        unsigned short depth = depthData[horizonY * ofGetWidth()/2 + x];
        XnProject[x].Z = (backZ[x] - depth >= 250)? depth: 0;
    }
    kinect.getDepthGenerator().ConvertProjectiveToRealWorld(640, XnProject, XnWorld);
}

//--------------------------------------------------------------

void ofApp::draw(){
    ofSetColor(255, 255, 255);
    kinect.drawDepth(0, 0, ofGetWidth()/2, ofGetHeight());
    
    int maxX = 0;
    int maxY = ofGetHeight();
    
    ofSetColor(100, 200, 100);
    for (int x = 0; x < ofGetWidth()/2; x++) {
        XnPoint3D world = XnWorld[x];
        float scrX = world.X * 0.1 + 320, scrY = 479 - world.Z * 0.1;
        if (0 <= scrX && scrX < ofGetWidth()/2 && 0 <= scrY && scrY < ofGetHeight()-20) {
            if (maxX < scrX) {
                maxX = scrX;
            }
            if (maxY > scrY) {
                maxY = scrY;
            }
        }
    }
    
    ofSetColor(255, 255, 255);
    ofDrawLine(0, horizonY, ofGetWidth()/2, horizonY);
    ofDrawBitmapString("locX: " + ofToString(maxX), 10, 15);
    ofDrawBitmapString("depth: " + ofToString(ofGetHeight() - maxY), 10, 30);
    
    //  draw grid
    ofSetColor(150, 150, 150);
    for (int x = -300; x <= 600; x += 100)
        ofDrawLine(640 + x + 320, 0, 640 + x + 320, 479);
    for (int y = 0; y < 480; y += 100)
        ofDrawLine(640, 480 - y, 640 + 639, 480 - y);
    //  draw sensing area
    ofSetColor(200, 200, 0);
    float offset = 480 * tan(29.0 * M_PI / 180.0);
    ofDrawLine(640 + 320 - offset, 0, 640 + 320, 480);
    ofDrawLine(640 + 320 + offset, 0, 640 + 320, 480);
    
    for (int x = 0; x < 640; x++) {
        XnPoint3D world = XnWorld[x];
        float scrX = world.X * 0.1 + 320, scrY = 479 - world.Z * 0.1;
        if (0 <= scrX && scrX < 640 && 0 <= scrY && scrY < 480)
            ofDrawCircle(640 + scrX, scrY, 3);
    }
    ofSetColor(150, 10, 10);
    ofDrawCircle(640 + maxX, maxY, 5);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 's') {
        unsigned short *depthData = kinect.getDepthRawPixels().getData();
        for (int x = 0; x < ofGetWidth()/2; x++)
            backZ[x] = depthData[horizonY * ofGetWidth()/2 + x];
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
