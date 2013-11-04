//
//  ofCelestialBody.cpp
//  RealDirections
//
//  Created by Xavier Fischer on 20/10/13.
//
//

#include "ofCelestialBody.h"
#define RADIUSFACTOR 100.
#define DISTFACTOR 50000.
#define SPHERE_RES 100

ofCelestialBody::ofCelestialBody(string _name, double _radius, double _sunDistance, double _inclination, double _rotationPeriod, string textureFileName, string boundariesFileName){

    name = _name;
    radius = _radius/RADIUSFACTOR;
    distance = _sunDistance/DISTFACTOR;
    inclination = _inclination;
    rotationPeriod = _rotationPeriod;
    
    texture.loadImage("textures/" + textureFileName);
    
    setup();
    
    vector< vector<ofPoint> > boundaries;
    loadSegments(boundaries, boundariesFileName);
    addToMesh(boundaries, ofFloatColor(1.0));
    
}


ofCelestialBody::ofCelestialBody(string _name, double _radius, double _sunDistance, double _inclination, double _rotationPeriod, string textureFileName){
    
    name = _name;
    radius = _radius/RADIUSFACTOR;
    distance = _sunDistance/DISTFACTOR;
    inclination = _inclination;
    rotationPeriod = _rotationPeriod;
    
    texture.loadImage("textures/" + textureFileName);
    
    setup();
    
}

void ofCelestialBody::loadSegments( vector< vector<ofPoint> > &segments, string _file){
    
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
void ofCelestialBody::addToMesh(vector<vector<ofPoint> > &boundaries, ofFloatColor _color){
    
    boundariesMesh.setMode(OF_PRIMITIVE_LINES);
	boundariesMesh.clear();
    
    ofVec3f center = ofVec3f(0,0,radius);
    
	for(int i = 0; i < boundaries.size(); i++){
        
		ofVec3f lastPoint;
        
		for (int j = 0; j < boundaries[i].size(); j++){
            
            ofQuaternion latRot, longRot;
			latRot.makeRotate(boundaries[i][j].y, 1, 0, 0);
			longRot.makeRotate(boundaries[i][j].x+180, 0, 1, 0);
            
			ofVec3f worldPoint = latRot * longRot * center;
            
			if ( j > 0 ){
				boundariesMesh.addColor( _color );
				boundariesMesh.addVertex(lastPoint);
				boundariesMesh.addColor( _color );
				boundariesMesh.addVertex(worldPoint);
			}
            
			lastPoint = worldPoint;
		}
	}
}

void ofCelestialBody::setup(){
    
    sphere.setRadius( radius );
    sphere.setResolution(SPHERE_RES);
    
    setupGraticules();
    
}

void ofCelestialBody::setupGraticules(){
    //setupGraticules
    
    ofVec3f center = ofVec3f(0,0,radius);
    
    graticulesMesh.setMode(OF_PRIMITIVE_LINES);
    graticulesMesh.clear();
    
    ofQuaternion latRot, longRot;
    
    // meridians
    for (int lon = 0; lon <=360; lon+=15) {
        for (int lat = 90; lat <=90+360; lat+=1) {
            
            latRot.makeRotate(lat, -1, 0, 0);
            longRot.makeRotate(lon, 0, 1, 0);
            
            graticulesMesh.addVertex(latRot * longRot * center);
            
            latRot.makeRotate(lat+1, -1, 0, 0);
            graticulesMesh.addVertex(latRot * longRot * center);
        }
    }
    
    // parallels
    for (int lat = 75; lat >=-75; lat-=15) {  // north to south
        for (int lon = 0; lon <=360; lon+=1) {
            
            latRot.makeRotate(lat, -1, 0, 0);
            longRot.makeRotate(lon, 0, 1, 0);
            
            graticulesMesh.addVertex(latRot * longRot * center);
            
            longRot.makeRotate(lon+1, 0, 1, 0);
            graticulesMesh.addVertex(latRot * longRot * center);
        }
    }
    
}

void ofCelestialBody::update(){

}

void ofCelestialBody::draw(bool bDrawAxis, bool bDrawTextured, bool bDrawBoundaries){
    
    ofPushMatrix();
    
    ofTranslate(position);
    
    ofRotate(inclination, 0, 0, 1);
    
    ofRotate(ofGetElapsedTimeMillis()*rotationPeriod*0.0002, 0, 1, 0);

	if (bDrawAxis){
		ofDrawAxis(radius);
	}
    
    if (bDrawTextured){

        ofFill();
        ofSetColor(255);
        sphere.mapTexCoordsFromTexture( texture.getTextureReference() );
        texture.getTextureReference().bind();
        
        sphere.draw();
        //sphere.drawWireframe();
        //sphere.drawNormals(20,false);
        texture.getTextureReference().unbind();

        //
//        //ofSetSphereResolution(25);
//        ofFill();
//        ofSetColor(255);
//        ofDrawSphere(radius);
//        ofFill();
//        //graticules.draw();
    }
    else{
        ofSetColor(255);
        graticulesMesh.draw();

    }
        

    if (boundariesMesh.getNumVertices()>0 && bDrawBoundaries)
        boundariesMesh.draw();
    
    ofSetColor(255);
    
    ofTranslate(0,radius*1.1,0);
    
    ofDrawBitmapString(name, 0, 0);

    ofPopMatrix();
    
    
    
    
}