/**
 *
 * ofSolarSystem
 * Based on Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by Xavier Fischer on 10/01/2013
 */

#include "testApp.h"

void Params::setup() {
    
	farClip =  778412020+71490;     // cameras farclip as far as possible
    bodySpacing = .075;             // spacing between planets when drawn in SIZE mode
    radiusFactor = 100000.;         // factor for real radius to world radius conversion
    distanceFactor = 100000.;       // factor for real distance to world radius conversion
    sphereResolution = 75;
    dampen = 0.001;                 // free cam mouse sensivity
    texturePath = "textures/lowres/";    // use lowres, medres, or hires
    showMoons = true;              // toggle moons
    sphereCamCycleMoons = true;    // if moons enabled, and set to true, sphereCam will stop on moons too
    
    sphereCamRotationDelayMs = 500; // sphere cam rotation delay
    sphereCamMoveDelayMs = 1000;     // sphere cam move delay

}

//--------------------------------------------------------------
void testApp::setup(){
    
	ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofEnableAlphaBlending();
    ofEnableDepthTest();
    
    // init params above
    param.setup();
    
    // add planets and moons
    solarSystem.setup();
   
    // GUI
    bShowHelp = false;
    bDrawAxis = false;
    bDrawTextured = true;
    
    setupCameras();
    
    //---------------------------------------------
    // set default mode to SIZE comparison
    solarSystem.mode = ofSolarSystem::SIZE;
    
    
}

//---------------------------------------------
void testApp::setupCameras(){
    
    //---------------------------------------------
    // init cams
    camIndex = 0;
    
    easyCam.setDistance(1000);
    sphereCam.setFarClip(param.farClip);
    cam.setFarClip(param.farClip);
    easyCam.setFarClip(param.farClip);
    
    // vFlip must be set to true. If false, sphere textures are flipped
    vFlip = true;
    easyCam.setVFlip(vFlip);
    sphereCam.setVFlip(vFlip);
    cam.setVFlip(vFlip);
    
    sphereCam.setNearClip(0.01);
    easyCam.setNearClip(0.01);
    cam.setNearClip(0.01);
    
    ofVec3f camPos = solarSystem.bodies[0].getPosition();
    float bodyRadius = solarSystem.bodies[0].radius;
    camPos.x -= bodyRadius * 6; //3;
    camPos.y -= bodyRadius * 1;
    camPos.z -= bodyRadius;
    
    easyCam.setPosition(camPos);
    easyCam.setTarget(ofVec3f(0,0,0));
    
    setActiveCam(1);

}

//--------------------------------------------------------------
void testApp::update(){
    
    // show framerate in window title
	//ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    // update moon orbits
    solarSystem.update();
    
    // sphereCam must be updated because position and target are tweened each frame
    sphereCam.update();

}

//--------------------------------------------------------------
void testApp::draw(){


	ofBackground(0);
    
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);

    // start active cam
    switch (camIndex) {
        case 0:    easyCam.begin(); break;
        case 1:    sphereCam.begin(); break;
        case 2:    cam.begin(); break;
    }
    
        // draw whole system
        solarSystem.draw(bDrawAxis, bDrawTextured);
        
        if (bDrawAxis){
            sphereCam.draw();
            cam.draw();
        }
    
    // stop active cam
    switch (camIndex) {
        case 0: easyCam.end(); break;
        case 1: sphereCam.end(); break;
        case 2: cam.end(); break;
    }
    
    ofPopMatrix();
    drawHelp();
    
}

//--------------------------------------------------------------
// copy active cam position and orientation to all other cams
void testApp::setActiveCam(int cameraIndex){
    
    // copy active cam position, lookAtDirection and orientation
    if (camIndex==0)
        updateCams(easyCam.getPosition(), easyCam.getLookAtDir()*param.farClip, easyCam.getOrientationQuat());
    else if (camIndex==1)
        updateCams(sphereCam.movedTo, sphereCam.lookedAt, sphereCam.getOrientationQuat());
    else if (camIndex==2)
        updateCams(cam.getPosition(), cam.getLookAtDir()*param.farClip, cam.getOrientationQuat());
    
    // switch cam index
    camIndex = cameraIndex;
    
    // if ofEasyCam is activated, disable its mouseInput to avoid unwanted moves
    if (camIndex == 0)
        easyCam.enableMouseInput();
    else{
        easyCam.disableMouseInput();
    }


}

//--------------------------------------------------------------
// Set position, lookAtDirection and orientation to all cameras
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

//--------------------------------------------------------------
// Draw help panel (TODO: use ofxGui)
void testApp::drawHelp(){
    stringstream helpStream;
    
    ofSetColor(255);
    
    helpStream << "h: " << (bShowHelp ? "hide" : "show") << " help";

	if (bShowHelp) {
        helpStream << endl;
        helpStream << "1: draw axes: " << (bDrawAxis ? "YES" : "NO") << endl;
		helpStream << "2: draw textures: " << (bDrawTextured ? "YES" : "NO") << endl;
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
// Handles all keyboard input
void testApp::keyPressed(int key){
    
    
    if (key =='f')
        ofToggleFullscreen();
    if (key == '1')
        bDrawAxis = !bDrawAxis;
    if (key == '2')
        bDrawTextured = !bDrawTextured;
    if (key =='c')
        setActiveCam((camIndex + 1) % 3);
    if (key == 'r')
        setupCameras();
    if (key == 'h')
        bShowHelp = !bShowHelp;
    
    
    
    
    if (key == ' ' && camIndex == 1){

        //---------------------------------------
        // sphere cam
        static int bodyIndex = 1;  // tracks current planet
        static int moonIndex = -1; // tracks current planet's moon
        static int numMoons = 0;   // tracks current planet moons count
        
        ofVec3f bodyPos;
        double bodyExtent;
        double bodyRadius;

        // get current planet
        ofCelestialBody currentBody = solarSystem.bodies[bodyIndex];

        // get current planet moon count
        // set to zero if disable cycle through requested
        numMoons = param.sphereCamCycleMoons ? currentBody.moons.size() : 0;
        
        // get planet pos, extent and radius
        bodyPos = currentBody.getPosition();
        bodyExtent = currentBody.extent;
        bodyRadius = currentBody.radius;
        
        if (moonIndex>=0){
            
            // get moon global transformation
            // by combining planet inclination and rotation
            ofQuaternion qatInclination;
            qatInclination.makeRotate(currentBody.inclination, ofVec3f(1, 0, 0));
            //ofQuaternion qatRotation;
            //qatRotation.makeRotate(ofGetElapsedTimeMillis()/currentBody.rotationPeriod*0.002, ofVec3f(0, 1, 0));
            
            // TODO: add a chase cam feature when moon orbits planet
            bodyPos += currentBody.moons[moonIndex].getPosition() /** qatRotation*/ * qatInclination;
            bodyExtent = currentBody.moons[moonIndex].extent;
            bodyRadius = currentBody.moons[moonIndex].radius;
            
        }
        
        ofVec3f sphereLookAt;
        ofVec3f sphereTarget;
        
        if (solarSystem.mode == ofSolarSystem::SIZE){
    
            // view planet by planet faced to planet along x axis
            sphereLookAt = bodyPos;
            
            sphereTarget = bodyPos;
            sphereTarget.x -= bodyRadius * 6; //3;
            sphereTarget.y -= bodyExtent * 1;
            sphereTarget.z -= bodyRadius;

        
        } else if (solarSystem.mode == ofSolarSystem::DISTANCE){
            
            if (moonIndex>=0)
                sphereLookAt = currentBody.getPosition();
            else
                sphereLookAt = solarSystem.bodies[0].getPosition();
            
            sphereTarget = bodyPos;
            sphereTarget.z -= bodyExtent * 10.;
            sphereTarget.y -= bodyExtent * 2.;
        }
        
        sphereCam.lookAtTo(sphereLookAt, param.sphereCamRotationDelayMs);
        sphereCam.moveTo(sphereTarget, param.sphereCamMoveDelayMs);
        
        if (moonIndex == numMoons-1 || numMoons == 0){
            moonIndex = -1; // reset moon counter
            bodyIndex = (bodyIndex + 1) % solarSystem.bodies.size();
        }
        else
            moonIndex++; // increase moon counter
            
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
    //cam.tilt((lastMouse.y-y)*param.dampen);
    //cam.pan((lastMouse.x-x)*param.dampen);
    
    if (camIndex == 2) {
        ofVec2f mouse(x,y);
        if (button == OF_MOUSE_BUTTON_MIDDLE){
            // move left/up
            cam.truck((lastMouse.x-x)*param.dampen);
            cam.boom((lastMouse.y-y)*param.dampen);
        }
        else if (button == OF_MOUSE_BUTTON_RIGHT){
            //move forward/backwards
            cam.dolly((y-lastMouse.y)*param.dampen);
        }
        else if (button == OF_MOUSE_BUTTON_LEFT){
            // rotate
            ofQuaternion yRot((y-lastMouse.y)*param.dampen*50, cam.getXAxis());
            ofQuaternion xRot((lastMouse.x-x)*param.dampen*50, cam.getYAxis());
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
