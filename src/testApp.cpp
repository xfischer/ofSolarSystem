/**
 *
 * RealDirections
 * Based on Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by Xavier Fischer on 10/01/2013
 */

#include "testApp.h"
#define SIDEBYSIDE_SEPARATION 50

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
	ofSetFrameRate(60);
	//ofEnableAlphaBlending();
    ofEnableDepthTest();
    ofSetSphereResolution(25);
	//NoFill();
	ofFill();
    
	// create little objects for each city.
	// A Lat/Lon like this:
	// Lewiston, Idaho 	W 46 24' N 117 2'
	// This translate to angles and degrees (1 degree = 1/60. of an angle)
	// West and South are negative values
    
	// here is a list of big cities and their positions
	// http://www.infoplease.com/ipa/A0001796.html
    
	
    cities.push_back( (City){ "new york", 40+47/60., -73 + 58/60.}  );
    cities.push_back( (City) { "aix-en-provence", 43.529, 5.443 });
    /*
     cities.push_back( (City){ "tokyo", 35 + 40./60, 139 + 45/60. } );
     cities.push_back( (City) { "london", 51 + 32/60., -5./60. });
	 cities.push_back( (City) { "shanghai", 31 + 10/60., 121 + 28/60. });
	 cities.push_back( (City) { "buenos aires", -34 + 35/60., -58 + 22/60. });
	 cities.push_back( (City) { "melbourne" , -37 + 47/60., 144 + 58/60. });
	 cities.push_back( (City) { "detroit", 42 + 19/60., -83 + 2 / 60. });
	 cities.push_back( (City) { "aix-en-provence", 43.529, 5.443 });
    */
    
#ifdef __APPLE__
	loadSegments( boundaries, "unix-boundaries-simple.txt" );
#else
    loadSegments( boundaries, "boundaries-simple.txt" );
#endif
    
    celestialBodies.push_back( ofCelestialBody(	"Sun", 696342, 0, 0 /* 7.25 */, 25.38 ));
    
    celestialBodies.push_back( ofCelestialBody("Mercury", 2439.7, 57909227, 0,  58.646 ));
    celestialBodies.push_back( ofCelestialBody("Venus", 6051.8, 108209475, -2.7 ,  -243.018 ));
    celestialBodies.push_back( ofCelestialBody("Earth", 6371.00, 149598262, 23.4393,  0.99726968, boundaries ));
    celestialBodies.push_back( ofCelestialBody("Mars", 3389.5, 227943824, 25.2,  1.026 ));
    celestialBodies.push_back( ofCelestialBody("Jupiter", 69911, 778340821, 3.1,  0.41354 ));
    celestialBodies.push_back( ofCelestialBody("Saturn", 58232, 1426666422, 26.7,  0.444 ));
    celestialBodies.push_back( ofCelestialBody("Uranus", 25362, 2870658186, -97.8 ,  -0.718 ));
    celestialBodies.push_back( ofCelestialBody("Neptune", 24622, 4498396441, 28.3,  0.671 ));
    
    //GUI
    bShowHelp = true;
    
    bDrawAxis = false;
    bDrawGraticules = true;
    bDrawBoundaries = true;
    camIndex = 0;
    
    easyCam.setDistance(1000);
    cam.setPosition(0, 0, 1000);
    cam.setFarClip(778412020+71492);
    easyCam.setFarClip(778412020+71492);
    
    easyCam.lookAt(ofVec3f(0,0,-1));
    cam.lookAt(ofVec3f(0,0,778412020+71492));
    
    //this slows down the rotate a little bit
	dampen = .4;
    
    glEnable(GL_DEPTH_TEST); //enable depth comparisons and update the depth buffer
	ofDisableArbTex(); //needed for textures to work with gluSphere
	
	
    //prepare quadric for sphere
	quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
    
    
    texture.loadImage("earth.jpg");
    
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
    
    //Extract the rotation from the current rotation
    ofVec3f axis;
    float angle;
    curRot.getRotate(angle, axis);
    
    //cam.rotate(angle, axis.x, axis.y, axis.z);
    
    easyCam.lookAt(cam);

    
    
    // Draw bodies side by side
    bool bSideBySide = true;

    if (bSideBySide){
        double currentDistance = 0;
        
        for(int i = 0; i < celestialBodies.size(); i++){
            
            if (currentDistance > 0)
                currentDistance += celestialBodies[i].radius + SIDEBYSIDE_SEPARATION;
            
            ofPushMatrix();
            
            ofTranslate(0, 0, currentDistance);
            
            celestialBodies[i].draw(bDrawAxis, bDrawGraticules, bDrawBoundaries);
            
            ofPopMatrix();
            
            currentDistance += celestialBodies[i].radius + SIDEBYSIDE_SEPARATION;
        
            ofFill();
  
        }
        texture.getTextureReference().bind();
        gluSphere(quadric, 60, 100, 100);
        texture.getTextureReference().unbind();
    } else {
        
        for(int i = 0; i < celestialBodies.size(); i++){
            celestialBodies[i].draw(bDrawAxis, bDrawGraticules, bDrawBoundaries);
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
		helpStream << "2: draw graticules :" << (bDrawGraticules ? "YES" : "NO") << endl;
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

void testApp::loadSegments( vector< vector<ofPoint> > &segments, string _file){
    
	ifstream fileIn;
    
	fileIn.open( ofToDataPath( _file ).c_str());
	if(fileIn.is_open()) {
        
		int lineCount = 0;
        
		vector<ofPoint> newPoints;
        
		while(fileIn != NULL) {
			string temp;
			getline(fileIn, temp);
            
			// Skip empty lines.
			if(temp.length() != 0) {
                
				vector<string> values = ofSplitString(temp, " ");
                
                if ( values[0] == "segment" || values[0].find("segment", 0) != -1){
                    
					if (lineCount != 0){
						segments.push_back( newPoints );
					}
                    
					newPoints.clear();
				} else {
					ofPoint newPoint = ofPoint();
					newPoint.y = ofToFloat( values[0] );
					newPoint.x = ofToFloat( values[1] );
                    
					newPoints.push_back(newPoint);
				}
                
				lineCount++;
			}
		}
        
		if (lineCount != 0){
			segments.push_back( newPoints );
		}
	}
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    float moveAmount = 50;

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
        bDrawGraticules = !bDrawGraticules;
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
        cam.dolly(5000);
    }

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
    ofQuaternion yRot((lastMouse.y-y)*dampen, cam.getXAxis());
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
