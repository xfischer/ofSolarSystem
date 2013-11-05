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

    celestialBodies.push_back( ofCelestialBody(	"Sun", 696342, 0, 0 /* 7.25 */, /* 25.38 */ 0, "sun.jpg" ));
    
    celestialBodies.push_back( ofCelestialBody("Mercury", 2439.7, 57909227, 0,  58.646, "mercury.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Venus", 6051.8, 108209475, -2.7 ,  -243.018, "venus.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Earth", 6371.00, 149598262, 23.4393,  0.99726968, "earth.jpg", BOUNDARIES_FILE ));
    celestialBodies.push_back( ofCelestialBody("Mars", 3389.5, 227943824, 25.2,  1.026, "mars.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Jupiter", 69911, 778340821, 3.1,  0.41354, "jupiter.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Saturn", 58232, 1426666422, 26.7,  0.444, "saturn.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Uranus", 25362, 2870658186, -97.8 ,  -0.718, "uranus.jpg" ));
    celestialBodies.push_back( ofCelestialBody("Neptune", 24622, 4498396441, 28.3,  0.671, "neptune.jpg" ));

}

void ofSolarSystem::update(){
    for(int i = 0; i < celestialBodies.size(); i++){
        celestialBodies[i].update();
    }
}

void ofSolarSystem::draw(bool axis, bool textured, bool boundaries){

    double currentDistance = 0;
    
    for(int i = 0; i < celestialBodies.size(); i++){
        
        if (currentDistance > 0)
            currentDistance += celestialBodies[i].radius + SIDEBYSIDE_SEPARATION;
        
        celestialBodies[i].position = ofVec3f(0, 0, -currentDistance);
        celestialBodies[i].draw(axis, textured, boundaries);
        currentDistance += celestialBodies[i].radius + SIDEBYSIDE_SEPARATION;
        
        ofFill();
        
    } // for
    
}