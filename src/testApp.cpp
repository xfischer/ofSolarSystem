/**
 *
 * RealDirections
 * Based on Quaternion Example for plotting latitude and longitude onto a sphere
 *
 * Created by Xavier Fischer on 10/01/2013
 */

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableSmoothing();
	ofSetFrameRate(60);
	//ofEnableAlphaBlending();
    ofEnableDepthTest();
    ofSetSphereResolution(200);
	//NoFill();
	ofFill();
    
	radius = 300;
    
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
    
	mesh.setMode(OF_PRIMITIVE_LINES);
	mesh.clear();
    
#ifdef __APPLE__
	loadSegments( boundaries, "unix-boundaries-simple.txt" );
#else
    loadSegments( boundaries, "boundaries-simple.txt" );
#endif
	addToMesh( boundaries, ofFloatColor(1.0) /* ofColor::black */);
    
    setupGraticules();
    
    //GUI
    bShowHelp = true;
    
    bDrawAxis = false;
    bDrawGraticules = true;
    bDrawBoundaries = true;
    camIndex = 0;
    
    cam.setPosition(0, 0, 1000);
    camTarget = ofVec3f(0);
    
}


//--------------------------------------------------------------
void testApp::update(){
	//ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
    ofPushMatrix();
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    
    if (camIndex == 0)
        easyCam.begin();
    else
        cam.begin();
        
    
	if (bDrawAxis){
        
		ofDrawAxis(radius);
		
		ofQuaternion latRot, longRot;
        
        // rotate along x and y axis and multiply by radius to place the point on the sphere
        latRot.makeRotate(cities[1].latitude, -1, 0, 0);
        longRot.makeRotate(cities[1].longitude, 0, 1, 0);
        ofVec3f newOrigin = latRot * longRot * ofVec3f(0,0,radius);
        
        ofNode node = ofNode();
        node.setPosition(newOrigin);
        node.setOrientation(latRot * longRot);
        node.draw();
        
        cam.draw();
        
//		if (camIndex == 1) {
//            
//			cam.setPosition(node.getPosition());
//			cam.setOrientation(node.getOrientationQuat());
//		}
		
	}
    
    
    if (bDrawGraticules){
        
        //translate so that the center of the screen is 0,0
        ofSetColor(64);
        graticules.draw();
    }
    
	ofSetColor(255);
    //ofSetColor(ofColor::black);
	for(unsigned int i = 0; i < cities.size(); i++){
        
		//three rotations
		//two to represent the latitude and lontitude of the city
		//a third so that it spins along with the spinning sphere
		ofQuaternion latRot, longRot, spinQuat;
		latRot.makeRotate(cities[i].latitude, -1, 0, 0);
		longRot.makeRotate(cities[i].longitude, 0, 1, 0);
		//spinQuat.makeRotate(ofGetFrameNum(), 0, 1, 0);
        
		//our starting point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
		ofVec3f center = ofVec3f(0,0,radius);
		//multiplying a quat with another quat combines their rotations into one quat
		//multiplying a quat to a vector applies the quat's rotation to that vector
		//so to to generate our point on the sphere, multiply all of our quaternions together then multiple the centery by the combined rotation
		ofVec3f worldPoint = latRot * longRot /* * spinQuat*/ * center;
        
		//draw it and label it
		ofLine(ofVec3f(0), worldPoint);
        
		//set the bitmap text mode billboard so the points show up correctly in 3d
		ofDrawBitmapString(cities[i].name, worldPoint );
	}
    
    if (bDrawBoundaries)
        mesh.draw();
    
    if (camIndex == 0)
        easyCam.end();
    else
        cam.end();
    
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
                
            case 2:
                helpStream << "city freecam";
                break;
                
            default:
                break;
        };
        helpStream << ")" << endl;
        helpStream << "move cameras with:" << endl
                    << "  - left/right (x axis)" << endl
                    << "  - up/down (y axis, +shift: z axis)" << endl
                    << "  - ctrl key for rotation" << endl;
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

void testApp::addToMesh( vector< vector<ofPoint> > & segments, ofFloatColor _color ){
	ofVec3f center = ofVec3f(0,0,300);
    
	for(int i = 0; i < segments.size(); i++){
        
		ofVec3f lastPoint;
        
		for (int j = 0; j < segments[i].size(); j++){
            
            ofQuaternion latRot, longRot;
			latRot.makeRotate(segments[i][j].y, -1, 0, 0);
			longRot.makeRotate(segments[i][j].x, 0, 1, 0);
            
			ofVec3f worldPoint = latRot * longRot * center;
            
			if ( j > 0 ){
				mesh.addColor( _color );
				mesh.addVertex(lastPoint);
				mesh.addColor( _color );
				mesh.addVertex(worldPoint);
			}
            
			lastPoint = worldPoint;
		}
	}
}

//--------------------------------------------------------------
void testApp::setupGraticules(){
    
    ofVec3f center = ofVec3f(0,0,300);
    
    graticules.setMode(OF_PRIMITIVE_LINES);
    graticules.clear();
    
    ofQuaternion latRot, longRot;
    
    // meridians
    for (int lon = 0; lon <=360; lon+=15) {
        for (int lat = 90; lat <=90+360; lat+=1) {
            
			latRot.makeRotate(lat, -1, 0, 0);
			longRot.makeRotate(lon, 0, 1, 0);
            
            graticules.addVertex(latRot * longRot * center);
            
            latRot.makeRotate(lat+1, -1, 0, 0);
            graticules.addVertex(latRot * longRot * center);
        }
    }
    
    // parallels
    for (int lat = 75; lat >=-75; lat-=15) {  // north to south
        for (int lon = 0; lon <=360; lon+=1) {
            
			latRot.makeRotate(lat, -1, 0, 0);
			longRot.makeRotate(lon, 0, 1, 0);
            
            graticules.addVertex(latRot * longRot * center);
            
            longRot.makeRotate(lon+1, 0, 1, 0);
            graticules.addVertex(latRot * longRot * center);
        }
    }

    /*
    for (int lat = -75; lat <=75; lat+=15) {
        for (int lon = -180; lon <=180; lon+=1) {
            
            ofQuaternion latRot, longRot;
			latRot.makeRotate(lat, -1, 0, 0);
			longRot.makeRotate(lon, 0, 1, 0);
            
			ofVec3f worldPoint = latRot * longRot * center;
            
            //graticules.addColor( color );
            graticules.addVertex(worldPoint);
        }
    }
     */

    
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

    if (key == OF_KEY_SHIFT){
        easyCam.disableMouseInput();
        bShiftDown = true;
    }
    if (key == OF_KEY_CONTROL)
        bCtrlDown = true;
    
    if (key == OF_KEY_UP){
        if (bShiftDown)
            cam.boom(10);
        else if (bCtrlDown)
            cam.tilt(1);
        else
            cam.dolly(-10);
    }
    if (key == OF_KEY_DOWN){
        if (bShiftDown)
            cam.boom(-10);
        else if (bCtrlDown)
            cam.tilt(-1);
        else
            cam.dolly(10);
    }
    if (key == OF_KEY_LEFT){
        if (bShiftDown && bCtrlDown)
            cam.roll(1);
        else if (bCtrlDown)
            cam.pan(1);
        else
            cam.truck(-10);
    }
    if (key == OF_KEY_RIGHT){
        if (bShiftDown && bCtrlDown)
            cam.roll(-1);
        else if (bCtrlDown)
            cam.pan(-1);
        else
            cam.truck(10);
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
        
        camIndex = (camIndex + 1) % 3;
            
        if (camIndex == 0)
            easyCam.enableMouseInput();
        else
            easyCam.disableMouseInput();
    }
    if (key == 'r')
        cam.resetTransform();
    if (key == 'h')
        bShowHelp = !bShowHelp;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if (key == OF_KEY_SHIFT){
        bShiftDown = false;
        easyCam.enableMouseInput();
    }
    if (key == OF_KEY_CONTROL)
        bCtrlDown = false;
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

//    if ((camIndex == 0 && bShiftDown) || camIndex==1){
//    float amount = 2;
//    float rotateAmountX = ofMap(ofGetMouseX(), 0, ofGetWidth(), -amount, amount);
//    float rotateAmountY = ofMap(ofGetMouseY(), 0, ofGetHeight(), -amount, amount);
//    
//    cam.pan(-rotateAmountX);
//    cam.tilt(-rotateAmountY);
//    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
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
