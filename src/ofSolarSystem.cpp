//
//  ofSolarSystem.cpp
//  RealDirections
//
//  Created by Xavier Fischer on 05/11/2013.
//
//

#include "ofSolarSystem.h"

#ifdef __APPLE__
#define BOUNDARIES_FILE "boundaries/unix-boundaries-simple.txt"
#else
#define BOUNDARIES_FILE "boundaries/boundaries-simple.txt"
#endif

#define SIDEBYSIDE_SEPARATION 50



ofSolarSystem::ofSolarSystem(){

    bodies.push_back( ofCelestialBody(	"Sun", 696342, 0, 0 /* 7.25 */, /* 25.38 */ 0, "sun.jpg" ));
    
    bodies.push_back( ofCelestialBody("Mercury", 2439.7, 57909227, 0,  58.646, "mercury.jpg" ));
    bodies.push_back( ofCelestialBody("Venus", 6051.8, 108209475, -2.7 ,  -243.018, "venus.jpg" ));
    bodies.push_back( ofCelestialBody("Earth", 6371.00, 149598262, 23.4393,  0.99726968, "earth.jpg", BOUNDARIES_FILE ));
    bodies.push_back( ofCelestialBody("Mars", 3389.5, 227943824, 25.2,  1.026, "mars.jpg" ));
    bodies.push_back( ofCelestialBody("Jupiter", 69911, 778340821, 3.1,  0.41354, "jupiter.jpg" ));
    bodies.push_back( ofCelestialBody("Saturn", 58232, 1426666422, 26.7,  0.444, "saturn.jpg" ));
    bodies.push_back( ofCelestialBody("Uranus", 25362, 2870658186, -97.8 ,  -0.718, "uranus.jpg" ));
    bodies.push_back( ofCelestialBody("Neptune", 24622, 4498396441, 28.3,  0.671, "neptune.jpg" ));

}

void ofSolarSystem::update(){
    for(int i = 0; i < bodies.size(); i++){
        bodies[i].update();
    }
}

void ofSolarSystem::draw(bool axis, bool textured, bool boundaries){

    //-----------------------------------------------------------------
    // Draw bodies
    double currentDistance = 0;
    
    for(int i = 0; i < bodies.size(); i++){
        
        if (currentDistance > 0)
            currentDistance += bodies[i].radius + SIDEBYSIDE_SEPARATION;
        
        bodies[i].setPosition(ofVec3f(0, 0, -currentDistance));
        
        bodies[i].draw(axis, textured, boundaries);
        currentDistance += bodies[i].radius + SIDEBYSIDE_SEPARATION;
        
    }
    
    
    
//    //-----------------------------------------------------------------
//    // Draw orbits
//    for(int i = 0; i < bodies.size(); i++){
//        
//        bodies[i].setPosition(ofVec3f(0, 0, -currentDistance));
//        
//                
//        
//    }

    
}