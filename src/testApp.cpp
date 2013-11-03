/**
 *
 * RealDirections
 * Based on Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by Xavier Fischer on 10/01/2013
 */

#include "testApp.h"
#define SIDEBYSIDE_SEPARATION 50

#ifdef __APPLE__
#define BOUNDARIES_FILE "boundaries/unix-boundaries-simple.txt"
#else
#define BOUNDARIES_FILE "boundaries/unix-boundaries-simple.txt"
#endif


//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
	ofSetFrameRate(60);
	ofEnableAlphaBlending();
    ofEnableDepthTest();
    ofEnableAntiAliasing();
    

    
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
    bShowHelp = true;
    
    bDrawAxis = false;
    bDrawTextured = false;
    bDrawBoundaries = false;
    camIndex = 0;
    
    easyCam.setDistance(1000);
    cam.setPosition(0, 0, 1000);
    cam.setFarClip(778412020+71492);
    easyCam.setFarClip(778412020+71492);
    
    vFlip = true;
    easyCam.setVFlip(vFlip);
    cam.setVFlip(vFlip);
    cam.setPosition(4527.78, -143.096, -8868.1);
    cam.rotate(
            ofQuaternion(0.23867,cam.getXAxis()
                         , 90, cam.getYAxis()
                         , 1.69485, cam.getZAxis())
               );
    //this slows down the rotate a little bit
	dampen = .4;
}


//--------------------------------------------------------------
void testApp::update(){
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
    for(int i = 0; i < celestialBodies.size(); i++){
        celestialBodies[i].update();
    }

}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
    
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

    switch (camIndex) {
        case 0:     easyCam.begin(); break;
        default:    cam.begin(); break;
    }
    
    easyCam.lookAt(cam);

    
    
    // Draw bodies side by side
    bool bSideBySide = true;

    if (bSideBySide){
        double currentDistance = 0;
        
        for(int i = 0; i < celestialBodies.size(); i++){
            
            if (currentDistance > 0)
                currentDistance += celestialBodies[i].radius + SIDEBYSIDE_SEPARATION;
            
            ofPushMatrix();
            
            ofTranslate(0, 0, -currentDistance);
            
            celestialBodies[i].draw(bDrawAxis, bDrawTextured, bDrawBoundaries);
            
            ofPopMatrix();
            
            currentDistance += celestialBodies[i].radius + SIDEBYSIDE_SEPARATION;
        
            ofFill();
  
        } // for
        
    } else {
        
        for(int i = 0; i < celestialBodies.size(); i++){
            celestialBodies[i].draw(bDrawAxis, bDrawTextured, bDrawBoundaries);
        }
        
    }

    
	if (bDrawAxis){
        cam.draw();
	}
    
    switch (camIndex) {
        case 0: easyCam.end(); break;
        default: cam.end(); break;
    }
    
    ofPopMatrix();
    drawHelp();
    
    
    
}

void testApp::drawHelp(){
    stringstream helpStream;
    
    ofSetColor(255);
    
	if (bShowHelp) {
        helpStream << "Showing help (press 'h' to toggle): " << (bShowHelp ? "YES" : "NO") << endl << endl;
		helpStream << "1: draw axes :" << (bDrawAxis ? "YES" : "NO") << endl;
		helpStream << "2: draw textures :" << (bDrawTextured ? "YES" : "NO") << endl;
		helpStream << "3: draw boundaries :" << (bDrawBoundaries ? "YES" : "NO") << endl;
        helpStream << "c: cycle cameras (current: ";
        switch (camIndex) {
            case 0:
                helpStream << "ofEasyCam";
                break;
            
            case 1:
                helpStream << "freecam";
                break;
                
        };
        helpStream << ")" << endl;
        helpStream << "move cameras with:" << endl
                    << "  - left/right (x axis)" << endl
                    << "  - up/down (y axis, +shift: z axis)" << endl
                    << "  - mouse (rotation)" << endl;
        helpStream << "r: reset cam" << endl;
        helpStream << "f: toggle full screen" << endl;
    }
	ofDrawBitmapStringHighlight(helpStream.str(), 10, 10);
    
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    float moveAmount = 500;

    if (key == OF_KEY_SHIFT){
        easyCam.disableMouseInput();
        bShiftDown = true;
    }

    if (key == OF_KEY_UP){
        if (bShiftDown){
            cam.boom(moveAmount);
            //easyCam.boom(moveAmount);
        }
        else{
            cam.dolly(-moveAmount);
            //easyCam.dolly(-moveAmount);
        }
    }
    if (key == OF_KEY_DOWN){
        if (bShiftDown){
            cam.boom(-moveAmount);
            //easyCam.boom(-moveAmount);
        }
        else{
            cam.dolly(moveAmount);
            //easyCam.dolly(moveAmount);
        }
    }
    if (key == OF_KEY_LEFT){
        cam.truck(-moveAmount);
        //easyCam.truck(-moveAmount);
    }
    if (key == OF_KEY_RIGHT){
        cam.truck(moveAmount);
        //easyCam.truck(moveAmount);
    }
    if (key =='f')
        ofToggleFullscreen();
    if (key == '1')
        bDrawAxis = !bDrawAxis;
    if (key == '2')
        bDrawTextured = !bDrawTextured;
    if (key == '3')
        bDrawBoundaries = !bDrawBoundaries;
    if (key =='c'){
        
        camIndex = (camIndex + 1) % 2;
            
        if (camIndex == 0)
            easyCam.enableMouseInput();
        else
            easyCam.disableMouseInput();
    }
    if (key == 'r')
        cam.resetTransform();
    if (key == 'h')
        bShowHelp = !bShowHelp;
    
    if (key == ' '){
        vFlip = !vFlip;
        easyCam.setVFlip(vFlip);
        cam.setVFlip(vFlip);
    }
    
    
    cout<<cam.getPosition() << endl;
    cout<<"roll: " << cam.getRoll()
        << "pitch: " << cam.getPitch()
        << "heading: " << cam.getHeading() << endl;
    
    

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if (key == OF_KEY_SHIFT){
        bShiftDown = false;
        easyCam.enableMouseInput();
    }
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
    
    if (camIndex == 1) {
    ofVec2f mouse(x,y);
    ofQuaternion yRot((y-lastMouse.y)*dampen, cam.getXAxis());
    ofQuaternion xRot((lastMouse.x-x)*dampen, cam.getYAxis());
    curRot = yRot*xRot;
    cam.rotate(curRot);
        lastMouse = mouse;}
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
