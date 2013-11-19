//
//  ofCelestialBody.cpp
//  ofSolarSystem
//
//  Created by Xavier Fischer on 20/10/13.
//
//

#include "ofCelestialBody.h"
#include "rd3DUtils.h"
#include "rdParams.h"


//--------------------------------------------------------------
ofCelestialBody::ofCelestialBody(string _name, double _radius, double _orbitDistance, double _inclination, double _rotationPeriod, string textureFileName){
    
    name = _name;
    radius = _radius/param.radiusFactor; // reduce radius and sun distance to avoid too large numbers
    extent = radius;
    distance = _orbitDistance/param.distanceFactor;
    inclination = _inclination;
    rotationPeriod = _rotationPeriod;
    
    texture.loadImage(param.texturePath + textureFileName);
    
    setup();
    
}

//--------------------------------------------------------------
void ofCelestialBody::setup(){
    
    sphere.setRadius( radius );
    
    if (name == "Sun")
        sphere.setResolution(param.sphereResolution);
    else
        sphere.setResolution(param.sphereResolution / 1.5); // reduce number of vertices if planet
    
    setupGraticules();
    
}

//--------------------------------------------------------------
void ofCelestialBody::addMoon(const ofCelestialBody &moon){
    
    if (param.showMoons)
        moons.push_back(moon);
    //extent += moon.radius + param.bodySpacing;
}

//--------------------------------------------------------------
// graticules are better in wireframe mode than wireframe spheres meshes
void ofCelestialBody::setupGraticules(){
    //setupGraticules
    
    ofVec3f center = ofVec3f(0,0,radius);
    
    graticulesMesh.setMode(OF_PRIMITIVE_LINES);
    graticulesMesh.clear();
    
    ofQuaternion latRot, longRot;
    
    // meridians every 15°
    for (int lon = 0; lon <=360; lon+=15) {
        for (int lat = 90; lat <=90+360; lat+=1) {
            
            latRot.makeRotate(lat, -1, 0, 0);
            longRot.makeRotate(lon, 0, 1, 0);
            
            graticulesMesh.addVertex(latRot * longRot * center);
            
            latRot.makeRotate(lat+1, -1, 0, 0);
            graticulesMesh.addVertex(latRot * longRot * center);
        }
    }
    
    // parallels every 15°
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

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void ofCelestialBody::addRing(float startRadius, float endRadius, string ringTextureFile, string ringAlphaFile){
    
    // Ring files comes in two parts : a color file and a transparency file
    ringTexture = rd3DUtils::combineColorAlpha(param.texturePath + ringTextureFile, param.texturePath + ringAlphaFile);
    
	setupRingMesh(startRadius, endRadius);
}

//--------------------------------------------------------------
// Create a ring mesh
void ofCelestialBody::setupRingMesh(float startRadius, float endRadius){
    
    ringMesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    ringMesh.clear();
    startRadius /= param.radiusFactor; // start of ring measured from planet center
    endRadius /= param.radiusFactor;   // end of ring measured from planet center
    extent += endRadius-radius;        // increase planet extent
    
    int w = ringTexture.width;
    int h = ringTexture.height;
    
    ofVec3f radIn = ofVec3f(0,0,startRadius);
    ofVec3f radOut = ofVec3f(0,0,endRadius);
    
    ofQuaternion rot;
    
    for (int x = 0; x <=360; x+=2) {
        
        rot.makeRotate(x, 0, 1, 0); // rotate along planet vertical axis
        
        // add vertex in outer ring
        ringMesh.addVertex(rot * radOut);
        ringMesh.addTexCoord( ofPoint(0, (x*h)/360));
        
        // add vertex in inner ring
        ringMesh.addVertex(rot * radIn);
        ringMesh.addTexCoord( ofPoint(w, (x*h)/360));
		
    }
}

//--------------------------------------------------------------
void ofCelestialBody::setPosition(const ofVec3f& _position){
    
    position = _position;
    
    setupOrbitMesh();
}

//--------------------------------------------------------------
ofVec3f& ofCelestialBody::getPosition(){
    return position;
}


//--------------------------------------------------------------
void ofCelestialBody::draw(bool bDrawAxis, bool bDrawTextured, bool bRingPass){
    
    // Draw only transparent related stuff : rings, orbits
    // this will be drawed at last
    if (bRingPass)
    {
        ofSetColor(255);
        ofPushMatrix();
        
        ofTranslate(position); // set planet distance
        ofRotate(inclination, 1, 0, 0); // inclinate planet
        
        // draw moons (here only orbits will be drawn)
        for(int i = 0; i < moons.size(); i++){
            moons[i].draw(bDrawAxis, bDrawTextured, bRingPass);
        }
        
        // draw rings
        if (ringMesh.getNumVertices()>0){
            
            // ring does rotate but its useless here
            //ofRotate(ofGetElapsedTimeMillis()/rotationPeriod*0.002, 0, 1, 0);
            
            if (bDrawTextured){
                
                //rd3DUtils::setNormals(ringMesh);
                ringTexture.getTextureReference().bind();
                ringMesh.draw();
                ringTexture.getTextureReference().unbind();
            }
            else{
                
                ofSetColor(255);
                ringMesh.drawWireframe();
            }
        }
        
        ofPopMatrix();
        
        // draw orbit
        ofSetColor(255, 255, 255, 64);
        orbitMesh.draw();
        ofSetColor(255);
        
    }
    else{
    
        ofSetColor(255);
        ofPushMatrix();
        
        ofTranslate(position); // set planet distance
        
        // draw planet name
        ofPushMatrix();
        ofTranslate(0,-radius*1.5,0);
        ofDrawBitmapString(name, 0, 0);
        ofPopMatrix();
        
        // rotate planet
        ofRotate(inclination, 1, 0, 0);
        
        // draw moons
        for(int i = 0; i < moons.size(); i++){
            moons[i].draw(bDrawAxis, bDrawTextured, bRingPass);
        }
        
        // rotate planet
        ofRotate(ofGetElapsedTimeMillis()/rotationPeriod*0.002, 0, 1, 0);
       
        if (bDrawAxis)
            ofDrawAxis(radius);
        
        if (bDrawTextured){

            sphere.mapTexCoordsFromTexture( texture.getTextureReference() );
            texture.getTextureReference().bind();
            sphere.draw();
            //sphere.drawNormals(20,false);
            texture.getTextureReference().unbind();
        }
        else{
            
            ofSetColor(255);
            sphere.drawWireframe();
            //graticulesMesh.draw();
        }
        
        ofPopMatrix();
        

    }
    
}