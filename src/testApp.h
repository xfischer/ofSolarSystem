#pragma once

#include "ofMain.h"
#include "ofxSphereCam.h"
#include "ofSolarSystem.h"
#include "rdParams.h"

typedef struct {
	string name;
	float latitude;
	float longitude;
} City;


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
	vector<City> cities;
    	
	vector< vector<ofPoint> > boundaries;
    
    ofSolarSystem solarSystem;
    
	ofxSphereCam sphereCam;
    ofEasyCam easyCam;
    ofCamera cam;
    
	bool bDrawAxis;
    bool bDrawTextured;
    
    bool bDrawBoundaries;
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
    
    void setActiveCam(int camIndex);
    void updateCams(ofVec3f position, ofVec3f lookat, ofQuaternion orientation);
    
};


