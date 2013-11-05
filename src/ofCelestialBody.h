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
    
    // methods, equivalent to specific functions of your class objects
    void update();  // update method, used to refresh your objects properties
    void draw(bool axis, bool textured, bool boundaries);    // draw method
    // variables
    
    double radius;
    double distance; // distance from Sun
    double inclination; // orbit to equator inclination
    double rotationPeriod; // rotation period in Earth days
    string name;
    
    void setPosition(const ofVec3f& position);
    ofVec3f& getPosition();
    
    ofSpherePrimitive sphere;
    
    ofCelestialBody(string name, double radius, double sunDistance, double inclination, double rotationPeriod, string textureFileName, string boundariesFileName);
    ofCelestialBody(string name, double radius, double sunDistance, double inclination, double rotationPeriod, string textureFileName);
    
private: // place private functions or variables declarations here
    
    ofVboMesh boundariesMesh;
    ofVboMesh graticulesMesh;
    ofVboMesh orbitMesh;
    
    void setup();
    void setupGraticules();
    void setupOrbitMesh();
    
    ofVec3f position;
    
    ofImage texture;
    
    void loadSegments( vector< vector<ofPoint> > &segments, string _file);
    void addToMesh( vector< vector<ofPoint> > &boundaries , ofFloatColor _color );

    
};

#endif /* defined(__RealDirections__ofCelestialBody__) */
