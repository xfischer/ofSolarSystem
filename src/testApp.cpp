/**
 *
 * RealDirections
 * Based on Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by Xavier Fischer on 10/01/2013
 */

#include "testApp.h"

void Params::setup() {
    
	farClip =  778412020+71490;
    bodySpacing = 50;
    radiusFactor = 100;
    sphereResolution = 75;
	
}

//--------------------------------------------------------------
void testApp::setup(){
    
    //ofEnableSmoothing();
	//ofSetFrameRate(60);
    ofSetVerticalSync(true);
	//ofEnableAlphaBlending();
    ofEnableDepthTest();
    //ofEnableAntiAliasing();

    
    
    param.setup();
    solarSystem.setup();

    
    
    //GUI
    bShowHelp = false;
    
    bDrawAxis = false;
    bDrawTextured = true;
    bDrawBoundaries = false;
    camIndex = 0;
    
    easyCam.setDistance(1000);
    sphereCam.setFarClip(param.farClip);
    cam.setFarClip(param.farClip);
    easyCam.setFarClip(param.farClip);
    
    vFlip = true;
    easyCam.setVFlip(vFlip);
    sphereCam.setVFlip(vFlip);
    cam.setVFlip(vFlip);
    
    ofVec3f camPos = ofVec3f(-10000, 0, -10000);
    easyCam.setPosition(camPos);
    easyCam.setTarget(ofVec3f(0,0,-10000));
    
    //this slows down the rotate a little bit
	dampen = .2;
    
    solarSystem.mode = ofSolarSystem::SIZE;
    
    
    
}


//--------------------------------------------------------------
void testApp::update(){
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    solarSystem.update();
    
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
    
    solarSystem.draw(bDrawAxis, bDrawTextured, bDrawBoundaries);
    
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
            updateCams(easyCam.getPosition(), easyCam.getLookAtDir()*param.farClip, easyCam.getOrientationQuat());
        else if (camIndex==1)
            updateCams(sphereCam.movedTo, sphereCam.lookedAt, sphereCam.getOrientationQuat());
        else if (camIndex==2)
            updateCams(cam.getPosition(), cam.getLookAtDir()*param.farClip, cam.getOrientationQuat());
        
        camIndex = (camIndex + 1) % 3;
            
        if (camIndex == 0)
            easyCam.enableMouseInput();
        else{
            easyCam.disableMouseInput();
        }
    }
    if (key == 'r')
        updateCams(ofVec3f(4527.78, -143.096, 8868.1), ofVec3f(0), ofQuaternion(120,ofVec3f(0,1,0)));
    if (key == 'h')
        bShowHelp = !bShowHelp;
    
    
    //---------------------------------------
    // tests
    static int test = 0;
    if (key == ' '){
        
        if (camIndex == 1){
            
            ofCelestialBody currentBody = solarSystem.bodies[++test % solarSystem.bodies.size()];
            ofVec3f sphereLookAt;
            ofVec3f sphereTarget;
            
            if (solarSystem.mode == ofSolarSystem::SIZE){
        
                // view planet by planet faced to planet along x axis
                sphereLookAt = currentBody.getPosition();
                sphereTarget = sphereLookAt;
                sphereTarget.x -= currentBody.radius * 3;
                sphereTarget.y -= currentBody.extent * 1;
                sphereTarget.z -= currentBody.radius;
            
            }
            
            if (solarSystem.mode == ofSolarSystem::DISTANCE){
                
                sphereLookAt = solarSystem.bodies[0].getPosition();
                
                sphereTarget = currentBody.getPosition();
                sphereTarget.z -= currentBody.extent * 10.;
                sphereTarget.y -= currentBody.extent * 2.;

                
            }
            
            sphereCam.lookAtTo(sphereLookAt, 1000);
            sphereCam.moveTo(sphereTarget, 5000);
            
            
        }
    }
    
    if (key == 'd'){
		solarSystem.mode = ofSolarSystem::DISTANCE;
    }
    if (key == 's'){
        solarSystem.mode = ofSolarSystem::SIZE;
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
            ofQuaternion yRot((y-lastMouse.y)*dampen, cam.getXAxis());
            ofQuaternion xRot((lastMouse.x-x)*dampen, cam.getYAxis());
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
