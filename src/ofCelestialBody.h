//
//  ofCelestialBody.h
//  ofSolarSystem
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
    ofCelestialBody(string name, double radius, double orbitDistance, double inclination, double rotationPeriod, string textureFileName);
    
    // methods, equivalent to specific functions of your class objects
    void draw(bool axis         // draw axis
            , bool textured     // draw textured or only graticules and meshes
            , bool ringsPass);  // draw only rings (need to be last drawn for texture alpha to work)
    // variables
    
    double extent; // radius + rings + moons (useful for camera views setup)
    double radius;
    double distance; // orbital distance
    double inclination; // orbit to equator inclination
    double rotationPeriod; // rotation period in Earth days
    string name;
    
    void setPosition(const ofVec3f& position);
    ofVec3f& getPosition();
    
    vector<ofCelestialBody> moons;
    
    ofSpherePrimitive sphere;
    
    void addRing(float startRadius, float endRadius, string ringTextureFile, string ringAlphaFile);
    void addMoon(const 	ofCelestialBody &body);
    
    
private: // place private functions or variables declarations here
    
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
    
    
};

#endif /* defined(__RealDirections__ofCelestialBody__) */
