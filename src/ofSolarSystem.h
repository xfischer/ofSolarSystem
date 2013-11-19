//
//  ofSolarSystem.h
//  ofSolarSystem
//
//  Created by Xavier Fischer on 05/11/2013.
//
//

#ifndef __RealDirections__ofSolarSystem__
#define __RealDirections__ofSolarSystem__

#include "ofMain.h"
#include "ofCelestialBody.h"


class ofSolarSystem {
    
public:
    
	enum ComparisonMode {
        SIZE = 0        // Planets are drawn so close that their relative size is obviously HUGE
        ,DISTANCE = 1   // Planets are drawn at their real distances. Not so good to realize how FAAAAAAR they are
        ,NOT_SET = -1 };

    void setup(); // whole setup
    void update();
    void draw(bool axis, bool textured);
        
    ofSolarSystem();
    
    vector<ofCelestialBody> bodies;
    
    ComparisonMode mode;
    
    
};


#endif /* defined(__RealDirections__ofSolarSystem__) */
