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
    
	ofSetFrameRate(60);
	ofEnableAlphaBlending();
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
    
	City newyork = { "new york", 40+47/60., -73 + 58/60. };
	City tokyo = { "tokyo", 35 + 40./60, 139 + 45/60. };
	City london = { "london", 51 + 32/60., -5./60. };
	City shanghai = { "shanghai", 31 + 10/60., 121 + 28/60. };
	City buenosaires = { "buenos aires", -34 + 35/60., -58 + 22/60. };
	City melbourne = { "melbourne" , -37 + 47/60., 144 + 58/60. };
	City detroit = { "detroit", 42 + 19/60., -83 + 2 / 60. };
	City aix = { "aix-en-provence", 43.529, 5.443 };
    
	cities.push_back( newyork );
	cities.push_back( aix );
    //	cities.push_back( tokyo );
    //	cities.push_back( london );
    //	cities.push_back( shanghai );
    //	cities.push_back( buenosaires );
    //	cities.push_back( melbourne );
    //	cities.push_back( detroit );
	
    
	mesh.setMode(OF_PRIMITIVE_LINES);
	mesh.clear();
#ifdef __APPLE__
	loadSegments( boundaries, "unix-boundaries-simple.txt" );
#else
    //loadSegments( boundaries, "boundaries-simple.txt" );
#endif
	addToMesh( boundaries, ofFloatColor(1.0) /* ofColor::black */);
    
    createGraticules();
    
    //GUI
    bShowHelp = true;
    
    gui.setup(); // most of the time you don't need a name
	//gui.add(camPos.setup("camera position", ofVec3f(0,0,500), ofVec3f(0,0,radius), ofVec3f(500,500,500)));
    gui.add(camNearClip.setup("near plane", 0, 0, radius*3.));
    drawAllAxes = false;
    changeCamPosition = false;
    
}



//--------------------------------------------------------------
void testApp::update(){
	//ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
	cam.begin();
    
	if (drawAllAxes){
        
		ofDrawAxis(radius);
		
		float lat = cities[1].latitude;
        float lon = cities[1].longitude;
		ofQuaternion latRot, longRot, spinRot, spinRot2;
        latRot.makeRotate(lat, -1, 0, 0);
        longRot.makeRotate(lon, 0, 1, 0);
        
        spinRot.makeRotate(90,1,0,0);
        spinRot2.makeRotate(90,0,0,1);
        
		float axisRadius = radius/4.;
        
		//our origin point is 0,0, on the surface of our sphere, this is where the meridian and equator meet
		ofVec3f origin = ofVec3f(0,0,radius);
		ofVec3f newOrigin = latRot * longRot * ofVec3f(0,0,radius);
		
        ofNode node = ofNode();
        node.setPosition(newOrigin);
        node.setOrientation(spinRot * spinRot2 * latRot * longRot);
        node.draw();
        
		if (changeCamPosition) {
            
			cam.setPosition(newOrigin);
			cam.setOrientation(spinRot * spinRot2 * latRot * longRot );
			changeCamPosition = false;
		}
		
	}
    
    
    cam.setNearClip(camNearClip);
    
	//translate so that the center of the screen is 0,0
	ofSetColor(255, 255, 255, 20);
    //ofSetColor(ofColor::white);
    
	//draw a translucent wireframe sphere (ofNoFill() is on)
	//add an extra spin at the rate of 1 degree per frame
	//ofRotate(ofGetFrameNum(), 0, 1, 0);
	//ofDrawSphere(0, 0, 0, radius);
    ofSetColor(64);
    graticules.draw();
    
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
    
    //mesh.draw();
    
	cam.end();
    
    // auto draw?
	// should the gui control hiding?
    gui.draw();
	
    
    drawHelp();
    
}

void testApp::drawHelp(){
    ofSetColor(255);
	string msg = string();
	if (bShowHelp) {
        msg = string("\nShowing help (press 'h' to toggle): ")+ (bShowHelp ? "YES" : "NO");
		msg += "\n\n1: test";
	}
	msg += "\nfps: " + ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapStringHighlight(msg, 10, 10);
    
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
                
				if ( values[0] == "segment"){
                    
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
void testApp::createGraticules(){
    ofVec3f center = ofVec3f(0,0,300);
    ofColor color = ofColor(40,40,40);
    
    graticules.setMode(OF_PRIMITIVE_LINE_LOOP);
    graticules.clear();
    
    for (int lon = -90; lon <=90; lon+=15) {
        for (int lat = 0; lat <=360; lat+=15) {
         
            ofQuaternion latRot, longRot;
			latRot.makeRotate(lat, -1, 0, 0);
			longRot.makeRotate(lon, 0, 1, 0);
            
			ofVec3f worldPoint = latRot * longRot * center;

            //graticules.addColor( color );
            graticules.addVertex(worldPoint);
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
	if (key == 'f'){
		ofToggleFullscreen();
	}
    if (key == ' '){
        
        //float lat = cities[0].latitude;
        //float lon = cities[0].longitude;
        ////cam.setOrientation(ofVec3f(30,0,0));
        //ofQuaternion latRot, longRot;
        //latRot.makeRotate(lat, -1, 0, 0);
        //longRot.makeRotate(lon, 0, 1, 0);
        //
        //ofVec3f worldPoint = latRot * longRot * ofVec3f(0,0,301);
        ////cam.setGlobalPosition(worldPoint);
        //cam.setPosition(worldPoint);
        //
        //if (lon<0) lon = lon + 360;
        //cam.setOrientation(ofVec3f(90.-lat,lon,-lon));
        
        
    }
    if (key == '1'){
        drawAllAxes = !drawAllAxes;
    }
    if (key == '2'){
        changeCamPosition = !changeCamPosition;
    }
    if( key == 'h' ){
		bShowHelp = !bShowHelp;
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
