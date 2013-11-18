//
//  rdParams.h
//  ofSolarSystem
//
//  Created by Xavier Fischer on 11/11/2013.
//
//  Global parameters class
//

#ifndef __RealDirections__rdParams__
#define __RealDirections__rdParams__

#include "ofMain.h"

//Control parameters class
class Params {
public:
    void setup();
    
	double farClip;
    float bodySpacing;
    double radiusFactor;     // 100
    double distanceFactor;   // = radiusFactor
    float sphereResolution; // 75

    //slows down the rotation 1 = 1 degree per pixel
	float dampen;
    
    string texturePath;      // texture path and quality "textures/lowres/", "textures/medres/", "textures/hires/"
    
    bool showMoons;
    bool sphereCamCycleMoons;   // if true, sphereCam will stop on each moon
    
};

extern Params param; //Declaration of a global variable

#endif /* defined(__RealDirections__rdParams__) */
