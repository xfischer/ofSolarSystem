//
//  ofSolarSystem.h
//  RealDirections
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
    
	
	enum ComparisonMode { SIZE = 0 , DISTANCE = 1 };

    void update();
    void draw(bool axis, bool textured, bool boundaries);
        
    ofSolarSystem();
    
    vector<ofCelestialBody> bodies;
    
    ComparisonMode mode;
    
    void distanceInPercentTo(float percent); // change planet distances using easing (ofxTween)
    
    void setup();
    
};


#endif /* defined(__RealDirections__ofSolarSystem__) */
