//
//  ofSolarSystem.h
//  RealDirections
//
//  Created by Maison on 05/11/2013.
//
//

#ifndef __RealDirections__ofSolarSystem__
#define __RealDirections__ofSolarSystem__

#include "ofMain.h"
#include "ofCelestialBody.h"

class ofSolarSystem {
    
public:
    
    void update();
    void draw(bool axis, bool textured, bool boundaries);
        
    ofSolarSystem();
    
    vector<ofCelestialBody> celestialBodies;
    
private: // place private functions or variables declarations here
    
    void setup();
    
    
};


#endif /* defined(__RealDirections__ofSolarSystem__) */
