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
    void draw(bool axis, bool graticules, bool boundaries);    // draw method, this where you'll do the object's drawing
    
    // variables
    ofVec3f position;
    double radius;
    double distance; // distance from Sun
    double inclination; // orbit to equator inclination
    double rotationPeriod; // rotation period in Earth days
    string name;
    
    ofSpherePrimitive sphere;
    
    
    ofCelestialBody(string name, double radius, double sunDistance, double inclination, double rotationPeriod);
    ofCelestialBody(string name, double radius, double sunDistance, double inclination, double rotationPeriod, vector< vector<ofPoint> > &boundaries);
    
private: // place private functions or variables declarations here
    ofVboMesh mesh;
    ofVboMesh graticules;
    void setup();
    
    void addToMesh( vector< vector<ofPoint> > &boundaries , ofFloatColor _color );
    
    ofImage texture;
    
};

#endif /* defined(__RealDirections__ofCelestialBody__) */
