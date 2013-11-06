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
    
    if (mode == SIZE){
        
        //-----------------------------------------------------------------
        // put bodies one next to another
        double dist = 0;
        
        for(int i = 0; i < bodies.size(); i++){
            
            if (dist > 0)
                dist += bodies[i].radius + SIDEBYSIDE_SEPARATION;
            
            bodies[i].setPosition(ofVec3f(0, 0, -dist));
            
            dist += bodies[i].radius + SIDEBYSIDE_SEPARATION;
            
        }
    }
    
    if (mode == DISTANCE){
        
        
        //-----------------------------------------------------------------
        // put bodies with real relative distances from Sun
        double dist;
        double sunRadius;
        
        for(int i = 0; i < bodies.size(); i++){
            
            if (i == 0){
                dist = 0;
                sunRadius = bodies[i].radius;
            }
            else
                dist = bodies[i].distance/3000. + sunRadius;
            
            bodies[i].setPosition(ofVec3f(0, 0, -dist));
                
        }
    }

    
    for(int i = 0; i < bodies.size(); i++){
        bodies[i].update();
    }
}

void ofSolarSystem::draw(bool axis, bool textured, bool boundaries){

    for(int i = 0; i < bodies.size(); i++){
        bodies[i].draw(axis, textured, boundaries);
    }
    
}

void ofSolarSystem::distanceInPercentTo(float percent){
    
}