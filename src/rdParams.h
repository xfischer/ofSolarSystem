//
//  rdParams.h
//  RealDirections
//
//  Created by Xavier Fischer on 11/11/2013.
//
//  Global parameters class
//

#ifndef __RealDirections__rdParams__
#define __RealDirections__rdParams__

//Control parameters class
class Params {
public:
    void setup();
    
	double farClip;
    float bodySpacing;
    float radiusFactor;     // 100
    float sphereResolution; // 75
    
};

extern Params param; //Declaration of a global variable

#endif /* defined(__RealDirections__rdParams__) */
