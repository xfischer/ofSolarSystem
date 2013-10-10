#pragma once

#include "ofMain.h"
#include "ofxGui.h"

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
    
	void loadSegments( vector< vector<ofPoint> > &segments, string _file);
	void addToMesh( vector< vector<ofPoint> > &segments , ofFloatColor _color );
	vector< vector<ofPoint> > boundaries;
    
	ofMesh mesh;
    
	ofEasyCam cam;
    
	float radius;
    
    bool drawAllAxes;
    bool changeCamPosition;
    
    // GUI
    bool bShowHelp;
    
	ofxVec3Slider camPos;
    ofxFloatSlider camNearClip;
    ofxToggle mouseInput;
	ofxToggle drawAxis;
    
	ofxPanel gui;
    
    void drawHelp();
    
};
