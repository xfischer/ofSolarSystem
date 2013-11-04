/**
 *
 * RealDirections
 * Based on Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by Xavier Fischer on 10/01/2013
 */

#include "testApp.h"
#define SIDEBYSIDE_SEPARATION 50
#define FAR_CLIP 778412020+71490
#ifdef __APPLE__
#define BOUNDARIES_FILE "boundaries/unix-boundaries-simple.txt"
#else
#define BOUNDARIES_FILE "boundaries/boundaries-simple.txt"
#endif


//--------------------------------------------------------------
void testApp::setup(){
    //ofEnableSmoothing();
	//ofSetFrameRate(60);
    ofSetVerticalSync(true);
	//ofEnableAlphaBlending();
    ofEnableDepthTest();
    //ofEnableAntiAliasing();
    

    
    celestialBodies.push_back( ofCelestialBody(	"Sun", 696342, 0, 0 /* 7.25 */, /* 25.38 */ 0, "sun.jpg" ));
    
    celestialBodies.push_back( ofCelestialBody("Mercury", 2439.7, 57909227, 0,  58.646, "mercury.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Venus", 6051.8, 108209475, -2.7 ,  -243.018, "venus.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Earth", 6371.00, 149598262, 23.4393,  0.99726968, "earth.jpg", BOUNDARIES_FILE ));
    celestialBodies.push_back( ofCelestialBody("Mars", 3389.5, 227943824, 25.2,  1.026, "mars.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Jupiter", 69911, 778340821, 3.1,  0.41354, "jupiter.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Saturn", 58232, 1426666422, 26.7,  0.444, "saturn.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Uranus", 25362, 2870658186, -97.8 ,  -0.718, "uranus.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Neptune", 24622, 4498396441, 28.3,  0.671, "neptune.jpg" ));
    
    
    //GUI
    bShowHelp = false;
    
    bDrawAxis = false;
    bDrawTextured = true;
    bDrawBoundaries = false;
    camIndex = 0;
    
    easyCam.setDistance(1000);
    sphereCam.setFarClip(FAR_CLIP);
    cam.setFarClip(FAR_CLIP);
    easyCam.setFarClip(FAR_CLIP);
    
    vFlip = true;
    easyCam.setVFlip(vFlip);
    sphereCam.setVFlip(vFlip);
    cam.setVFlip(vFlip);
    
    ofVec3f camPos = ofVec3f(4527.78, -143.096, -8868.1);
    easyCam.setPosition(camPos);
    easyCam.setTarget(celestialBodies[3].position);
    
    //this slows down the rotate a little bit
	dampen = .2;
}


//--------------------------------------------------------------
void testApp::update(){
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    for(int i = 0; i < celestialBodies.size(); i++){
        celestialBodies[i].update();
    }
    
    sphereCam.update();

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
    
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

    switch (camIndex) {
        case 0:    easyCam.begin(); break;
        case 1:    sphereCam.begin(); break;
        case 2:    cam.begin(); break;
    }
    
    
    double currentDistance = 0;
    
    for(int i = 0; i < celestialBodies.size(); i++){
        
        if (currentDistance > 0)
            currentDistance += celestialBodies[i].radius + SIDEBYSIDE_SEPARATION;
        
        celestialBodies[i].position = ofVec3f(0, 0, -currentDistance);
        celestialBodies[i].draw(bDrawAxis, bDrawTextured, bDrawBoundaries);
        currentDistance += celestialBodies[i].radius + SIDEBYSIDE_SEPARATION;
    
        ofFill();

    } // for

    
	if (bDrawAxis){
        sphereCam.draw();
        cam.draw();
	}
    
    switch (camIndex) {
        case 0: easyCam.end(); break;
        case 1: sphereCam.end(); break;
        case 2: cam.end(); break;
    }
    
    ofPopMatrix();
    drawHelp();
    
    
    
}

void testApp::updateCams(ofVec3f position, ofVec3f lookat, ofQuaternion orientation){
    
    cam.setPosition(position);
    easyCam.setPosition(position);
    sphereCam.moveTo(position, 0);
    
    cam.lookAt(lookat);
    easyCam.lookAt(lookat);
    sphereCam.lookAtTo(lookat, 0);
    
    cam.setOrientation(orientation);
    easyCam.setOrientation(orientation);
    sphereCam.setOrientation(orientation);
    
}


void testApp::drawHelp(){
    stringstream helpStream;
    
    ofSetColor(255);
    
    helpStream << "h: " << (bShowHelp ? "hide" : "show") << " help";

	if (bShowHelp) {
        helpStream << endl;
        helpStream << "1: draw axes: " << (bDrawAxis ? "YES" : "NO") << endl;
		helpStream << "2: draw textures: " << (bDrawTextured ? "YES" : "NO") << endl;
		helpStream << "3: draw boundaries: " << (bDrawBoundaries ? "YES" : "NO") << endl;
        helpStream << "c: cycle cameras: ";
        switch (camIndex) {
            case 0:
                helpStream << "ofEasyCam";
                break;
            
            case 1:
                helpStream << "ofxSphereCam";
                break;
                
            case 2:
                helpStream << "freecam";
                break;
                
        };
        helpStream << endl;
        helpStream << "move cameras with mouse:" << endl
                    << "  - left button: rotate" << endl
                    << "  - middle or m: xy translate" << endl
                    << "  - right: z translate" << endl;
        helpStream << "r: reset cam" << endl;
        helpStream << "f: toggle full screen";
    }
	ofDrawBitmapStringHighlight(helpStream.str(), 10, 10);
    
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    
    if (key =='f')
        ofToggleFullscreen();
    if (key == '1')
        bDrawAxis = !bDrawAxis;
    if (key == '2')
        bDrawTextured = !bDrawTextured;
    if (key == '3')
        bDrawBoundaries = !bDrawBoundaries;
    if (key =='c'){
        
        if (camIndex==0)
            updateCams(easyCam.getPosition(), easyCam.getLookAtDir()*FAR_CLIP, easyCam.getOrientationQuat());
        else if (camIndex==1)
            updateCams(sphereCam.movedTo, sphereCam.lookedAt, sphereCam.getOrientationQuat());
        else if (camIndex==2)
            updateCams(cam.getPosition(), cam.getLookAtDir()*FAR_CLIP, cam.getOrientationQuat());
        
        camIndex = (camIndex + 1) % 3;
            
        if (camIndex == 0)
            easyCam.enableMouseInput();
        else{
            easyCam.disableMouseInput();
        }
    }
    if (key == 'r')
        updateCams(ofVec3f(4527.78, -143.096, -8868.1), ofVec3f(0), ofQuaternion(120,ofVec3f(0,1,0)));
    if (key == 'h')
        bShowHelp = !bShowHelp;
    
    
    //---------------------------------------
    // tests
    static int test = 0;
    if (key == ' '){
        //        cout<< celestialBodies[5].position << endl;
        sphereCam.lookAtTo(celestialBodies[++test % celestialBodies.size()].position, 500);
        
        easyCam.setTarget(celestialBodies[5].position);
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    //every time the mouse is dragged, track the change
	//accumulate the changes inside of curRot through multiplication
    //cam.tilt((lastMouse.y-y)*dampen);
    //cam.pan((lastMouse.x-x)*dampen);
    
    if (camIndex == 2) {
        ofVec2f mouse(x,y);
        if (button == OF_MOUSE_BUTTON_MIDDLE){
            // move left/up
            cam.truck((lastMouse.x-x)/dampen);
            cam.boom((lastMouse.y-y)/dampen);
        }
        else if (button == OF_MOUSE_BUTTON_RIGHT){
            //move forward/backwards
            cam.dolly((y-lastMouse.y)/dampen);
        }
        else if (button == OF_MOUSE_BUTTON_LEFT){
            // rotate
            ofQuaternion yRot((lastMouse.y-y)*dampen, cam.getXAxis());
            ofQuaternion xRot((x-lastMouse.x)*dampen, cam.getYAxis());
            curRot = yRot*xRot;
            cam.rotate(curRot);
        }
        lastMouse = mouse;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    //store the last mouse point when it's first pressed to prevent popping
	lastMouse = ofVec2f(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	cout << msg.message;
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
    
}
