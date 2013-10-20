//
//  ofCelestialBody.cpp
//  RealDirections
//
//  Created by Xavier Fischer on 20/10/13.
//
//

#include "ofCelestialBody.h"

ofCelestialBody::ofCelestialBody(string _name, double _radius, double _sunDistance){
    
    name = _name;
    radius = _radius/100.;
    distance = _sunDistance/500000.;
    
    setup();
}

ofCelestialBody::ofCelestialBody(string _name, double _radius, double _sunDistance, vector< vector<ofPoint> > &boundaries){

    name = _name;
    radius = _radius/100.;
    distance = _sunDistance/500000.;
    
    setup();
    
    addToMesh(boundaries, ofFloatColor(1.0));
    
}

void ofCelestialBody::addToMesh(vector<vector<ofPoint> > &boundaries, ofFloatColor _color){
    
    mesh.setMode(OF_PRIMITIVE_LINES);
	mesh.clear();
    
    ofVec3f center = ofVec3f(0,0,radius);
    ofMatrix4x4 trans;
    trans.makeTranslationMatrix(ofVec3f(0,0,distance));
    
	for(int i = 0; i < boundaries.size(); i++){
        
		ofVec3f lastPoint;
        
		for (int j = 0; j < boundaries[i].size(); j++){
            
            ofQuaternion latRot, longRot;
			latRot.makeRotate(boundaries[i][j].y, -1, 0, 0);
			longRot.makeRotate(boundaries[i][j].x, 0, 1, 0);
            
			ofVec3f worldPoint = latRot * longRot * center * trans;
            
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
    
    //setupGraticules
    
    ofVec3f center = ofVec3f(0,0,radius);
    
    graticules.setMode(OF_PRIMITIVE_LINES);
    graticules.clear();
    
    ofQuaternion latRot, longRot;
    ofMatrix4x4 trans;
    trans.makeTranslationMatrix(ofVec3f(0,0,distance));
    
    // meridians
    for (int lon = 0; lon <=360; lon+=15) {
        for (int lat = 90; lat <=90+360; lat+=1) {
            
            latRot.makeRotate(lat, -1, 0, 0);
            longRot.makeRotate(lon, 0, 1, 0);
            
            graticules.addVertex(latRot * longRot * center * trans);
            
            latRot.makeRotate(lat+1, -1, 0, 0);
            graticules.addVertex(latRot * longRot * center * trans);
        }
    }
    
    // parallels
    for (int lat = 75; lat >=-75; lat-=15) {  // north to south
        for (int lon = 0; lon <=360; lon+=1) {
            
            latRot.makeRotate(lat, -1, 0, 0);
            longRot.makeRotate(lon, 0, 1, 0);
            
            graticules.addVertex(latRot * longRot * center * trans);
            
            longRot.makeRotate(lon+1, 0, 1, 0);
            graticules.addVertex(latRot * longRot * center * trans);
        }
    }
    
}

void ofCelestialBody::update(){

}

void ofCelestialBody::draw(bool bDrawAxis, bool bDrawGraticules, bool bDrawBoundaries){
    
	if (bDrawAxis){
		ofDrawAxis(radius);
	}
    
    if (bDrawGraticules){
        
        //translate so that the center of the screen is 0,0
        ofSetColor(64);
        graticules.draw();
    }

    if (mesh.getNumVertices()>0 && bDrawBoundaries)
        mesh.draw();
    
    ofSetColor(255);
    ofMatrix4x4 trans;
    trans.makeTranslationMatrix(ofVec3f(0,radius*1.1,distance));
    ofVec3f pos =ofVec3f(0,0,0) * trans;
    
    ofDrawBitmapString(name, pos);
    //ofDrawBitmapStringHighlight(name, pos);

}