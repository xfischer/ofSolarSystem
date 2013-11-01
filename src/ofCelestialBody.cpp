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

ofCelestialBody::ofCelestialBody(string _name, double _radius, double _sunDistance, double _inclination, double _rotationPeriod){
    
    name = _name;
    radius = _radius/RADIUSFACTOR;
    distance = _sunDistance/DISTFACTOR;
    inclination = _inclination;
    rotationPeriod = _rotationPeriod;
    
    
    setup();
}

ofCelestialBody::ofCelestialBody(string _name, double _radius, double _sunDistance, double _inclination, double _rotationPeriod, vector< vector<ofPoint> > &boundaries){

    name = _name;
    radius = _radius/RADIUSFACTOR;
    distance = _sunDistance/DISTFACTOR;
    inclination = _inclination;
    rotationPeriod = _rotationPeriod;
    
    setup();
    
    addToMesh(boundaries, ofFloatColor(1.0));
    
}

void ofCelestialBody::addToMesh(vector<vector<ofPoint> > &boundaries, ofFloatColor _color){
    
    mesh.setMode(OF_PRIMITIVE_LINES);
	mesh.clear();
    
    ofVec3f center = ofVec3f(0,0,radius);
    
	for(int i = 0; i < boundaries.size(); i++){
        
		ofVec3f lastPoint;
        
		for (int j = 0; j < boundaries[i].size(); j++){
            
            ofQuaternion latRot, longRot;
			latRot.makeRotate(boundaries[i][j].y, 1, 0, 0);
			longRot.makeRotate(boundaries[i][j].x+180, 0, 1, 0);
            
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

void ofCelestialBody::setup(){
    
    texture.loadImage(name +  ".jpg");
    
    sphere.setRadius( radius );
    sphere.setResolution(100);
        
    //setupGraticules
    
    ofVec3f center = ofVec3f(0,0,radius);
    
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
    
   
    
}

void ofCelestialBody::update(){

}

void ofCelestialBody::draw(bool bDrawAxis, bool bDrawGraticules, bool bDrawBoundaries){
    
    ofPushMatrix();
    
    ofRotate(inclination, 0, 0, 1);

    ofRotate(ofGetElapsedTimeMillis()*rotationPeriod*0.0002, 0, 1, 0);

	if (bDrawAxis){
		ofDrawAxis(radius);
	}
    
    if (bDrawGraticules){

        
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

    if (mesh.getNumVertices()>0 && bDrawBoundaries)
        mesh.draw();
    
    ofSetColor(255);
    
    ofTranslate(0,radius*1.1,0);
    
    ofDrawBitmapString(name, 0, 0);

    ofPopMatrix();
    
    
    
    
}