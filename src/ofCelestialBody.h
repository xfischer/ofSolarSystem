//
//  ofCelestialBody.h
//  RealDirections
//
//  Created by Xavier Fischer on 20/10/13.
//
//

#ifndef __RealDirections__ofCelestialBody__
#define __RealDirections__ofCelestialBody__

#include "ofMain.h"

class ofCelestialBody {
    
public: // place public functions or variables declarations here
    
    // Constructors
    ofCelestialBody(string name, double radius, double sunDistance, double inclination, double rotationPeriod, string textureFileName, string boundariesFileName);
    ofCelestialBody(string name, double radius, double sunDistance, double inclination, double rotationPeriod, string textureFileName);
    
    // methods, equivalent to specific functions of your class objects
    void update();  // update method, used to refresh your objects properties
    void draw(bool axis, bool textured, bool boundaries);    // draw method
    // variables
    
    double extent;
    double radius;
    double distance; // distance from Sun
    double inclination; // orbit to equator inclination
    double rotationPeriod; // rotation period in Earth days
    string name;
    
    void setPosition(const ofVec3f& position);
    ofVec3f& getPosition();
    
    ofSpherePrimitive sphere;
    
    void addRing(float startRadius, float endRadius, string ringTextureFile, string ringAlphaFile);
    
    
    
private: // place private functions or variables declarations here
    
    ofVboMesh boundariesMesh;
    ofVboMesh graticulesMesh;
    ofVboMesh orbitMesh;
    ofVboMesh ringMesh;
    
    void setup();
    void setupGraticules();
    void setupOrbitMesh();
	void setupRingMesh(float startRadius, float endRadius);
    
    ofVec3f position;
    
    ofImage texture;
    ofImage ringTexture;

    
    void loadSegments( vector< vector<ofPoint> > &segments, string _file);
    void addToMesh( vector< vector<ofPoint> > &boundaries , ofFloatColor _color );

    
    
};

#endif /* defined(__RealDirections__ofCelestialBody__) */
