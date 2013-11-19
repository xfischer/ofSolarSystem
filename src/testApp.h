#pragma once

#include "ofMain.h"
#include "ofxSphereCam.h"
#include "ofSolarSystem.h"
#include "rdParams.h"


class testApp : public ofBaseApp{
    
public:
	void setup();
	void update();
	void draw();
    
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    
    
    
private:
	
    ofSolarSystem solarSystem;
    
	bool bDrawAxis;
    bool bDrawTextured;
    
    int camIndex;
    bool bShiftDown;
    
    // GUI
    bool bShowHelp;
    
    void drawHelp();
    
    
    // --------------------------------------------------
    // Code from quaternionArcBall example

    //current state of the rotation
    ofQuaternion curRot;
	
    //a place to store the mouse position so we can measure incremental change
    ofVec2f lastMouse;
	
    bool vFlip;
    
    // Cameras
    ofxSphereCam sphereCam;
    ofEasyCam easyCam;
    ofCamera cam;
    
    // copy active cam position and orientation to all other cams
    void setActiveCam(int camIndex);
    
    // Set position, lookAtDirection and orientation to all cameras
    void updateCams(ofVec3f position, ofVec3f lookat, ofQuaternion orientation);
    
    // reset all cameras
    void setupCameras();
    
};


