//
//  ofCelestialBody.cpp
//  RealDirections
//
//  Created by Xavier Fischer on 20/10/13.
//
//

#include "ofCelestialBody.h"
#include "rd3DUtils.h"
#define RADIUSFACTOR 100.
#define SPHERE_RES 75


ofCelestialBody::ofCelestialBody(string _name, double _radius, double _sunDistance, double _inclination, double _rotationPeriod, string textureFileName, string boundariesFileName){

    name = _name;
    radius = _radius/RADIUSFACTOR;
    extent = radius;
    distance = _sunDistance/RADIUSFACTOR;
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
    extent = radius;
    distance = _sunDistance/RADIUSFACTOR;
    inclination = _inclination;
    rotationPeriod = _rotationPeriod;
    
    texture.loadImage("textures/" + textureFileName);
    
    setup();
    
}

void ofCelestialBody::setPosition(const ofVec3f& _position){
    
    position = _position;
    
    setupOrbitMesh();
}

ofVec3f& ofCelestialBody::getPosition(){
    return position;
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
//    sphere.setResolution(    ofMap(radius/RADIUSFACTOR, 2000/RADIUSFACTOR, 696342/RADIUSFACTOR, 100, 1000));
    
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

void ofCelestialBody::setupOrbitMesh(){
    
    ofVec3f center = ofVec3f(0,0,position.z);
    
    orbitMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    orbitMesh.clear();
    
    ofQuaternion rot;
    
    for (int x = 0; x <=360; x+=1) {
        
        rot.makeRotate(x, 0, 1, 0);
        
        orbitMesh.addVertex(rot * center);
        
    }
    

}

void ofCelestialBody::setupRingMesh(float startRadius, float endRadius){
    
    ringMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    ringMesh.clear();
    startRadius /= RADIUSFACTOR;
    endRadius /= RADIUSFACTOR;
    extent += endRadius-radius;
    
    int w = ringTexture.width;
    int h = ringTexture.height;
    
    ofVec3f radIn = ofVec3f(0,0,startRadius);
    ofVec3f radOut = ofVec3f(0,0,endRadius);
    
    ofQuaternion rot;
    
    for (int x = 0; x <=360; x+=1) {
        
        rot.makeRotate(x, 0, 1, 0);
        
        
        ringMesh.addVertex(rot * radOut);
        ringMesh.addTexCoord( ofPoint(0, (x*h)/360));
        
        ringMesh.addVertex(rot * radIn);
        ringMesh.addTexCoord( ofPoint(w, (x*h)/360));
		
    }
}

void ofCelestialBody::addRing(float startRadius, float endRadius, string ringTextureFile, string ringAlphaFile){
    
    ringTexture = rd3DUtils::combineColorAlpha("textures/" + ringTextureFile, "textures/" + ringAlphaFile);
    
	setupRingMesh(startRadius, endRadius);
}

void ofCelestialBody::update(){

}

void ofCelestialBody::draw(bool bDrawAxis, bool bDrawTextured, bool bDrawBoundaries){
    
    ofPushMatrix();
    
    ofTranslate(position);
    
    ofPushMatrix();
    
    ofSetColor(255);
    ofTranslate(0,-radius*1.5,0);
    ofDrawBitmapString(name, 0, 0);

    ofPopMatrix();
    
    ofRotate(inclination, 1, 0, 0);
    
    ofRotate(ofGetElapsedTimeMillis()*rotationPeriod*0.0002, 0, 1, 0);

	if (bDrawAxis){
		ofDrawAxis(radius);
	}
    
    if (bDrawTextured){

        sphere.mapTexCoordsFromTexture( texture.getTextureReference() );
        texture.getTextureReference().bind();
        sphere.draw();
        //sphere.drawNormals(20,false);
        texture.getTextureReference().unbind();
        
        
        if (ringMesh.getNumVertices()>0)
        {
            ringTexture.getTextureReference().bind();
            ringMesh.draw();
            ringTexture.getTextureReference().unbind();
        }

    }
    else{
        
        ofSetColor(255);
        graticulesMesh.draw();
        

//        ofSetColor(ofColor::green);
        if (ringMesh.getNumVertices()>0)
            ringMesh.drawWireframe();

        //sphere.drawWireframe();

    }
        

    if (boundariesMesh.getNumVertices()>0 && bDrawBoundaries)
        boundariesMesh.draw();
    
    
    ofPopMatrix();
    
    //ofSetColor(64);
    ofSetColor(255, 255, 255, 64);
    orbitMesh.draw();
    
    
}