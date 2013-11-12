//
//  ofSolarSystem.cpp
//  RealDirections
//
//  Created by Xavier Fischer on 05/11/2013.
//
//

#include "ofSolarSystem.h"
#include "rdParams.h"

#ifdef __APPLE__
#define BOUNDARIES_FILE "boundaries/unix-boundaries-simple.txt"
#else
#define BOUNDARIES_FILE "boundaries/boundaries-simple.txt"
#endif


ofSolarSystem::ofSolarSystem(){
}

void ofSolarSystem::setup(){
    
    param.showMoons = true;
    
    // -----------------------------------------------------
    // Moons
    
    // Earth
    ofCelestialBody moon = ofCelestialBody("Moon", 1737.4, 384400, 6.687, 27.321582, "moon.jpg");
    
    // Jupiter
    ofCelestialBody io = ofCelestialBody("Io", 1821.6, 421800, 0.036, 1.769, "io.jpg");
    ofCelestialBody europa = ofCelestialBody("Europa", 1560.8, 671100, 0.466, 3.551, "europa.jpg");
    ofCelestialBody ganymede = ofCelestialBody("Ganymede", 2631.2, 1070400, 0.177, 7.155, "ganymede.jpg");
    ofCelestialBody callisto = ofCelestialBody("Callisto", 2410.3, 1882700, 0.192, 16.6890184, "callisto.jpg");
    
    // Saturn
    ofCelestialBody enceladus   = ofCelestialBody("Enceladus", 252.1, 238037, 0.009, 0.1, "enceladus.jpg");
    ofCelestialBody tethys      = ofCelestialBody("Tethys", 533, 294672, 1.091, 0.1, "tethys.jpg");
    ofCelestialBody dione       = ofCelestialBody("Dione", 561.7, 377415, 0.028, 0.1, "dione.jpg");
    ofCelestialBody rhea        = ofCelestialBody("Rhea", 764.3, 527068, 0.333, 0.1, "rhea.jpg");
    ofCelestialBody titan       = ofCelestialBody("Titan", 2574.7, 1221865, 0.312, 0.1, "titan.jpg");
    ofCelestialBody iapetus     = ofCelestialBody("Iapetus", 735.6, 3560851, 8.313, 0.1, "iapetus.jpg");
    
    // Uranus
    ofCelestialBody miranda = ofCelestialBody("Miranda", 235.8, 129900, 4.338, 0.1, "miranda.jpg");
    ofCelestialBody ariel   = ofCelestialBody("Ariel", 578.9, 190900, 0.041, 0.1, "ariel.jpg");
    ofCelestialBody umbriel = ofCelestialBody("Umbriel", 584.7, 266000, 0.128, 0.1, "umbriel.jpg");
    ofCelestialBody titania = ofCelestialBody("Titania", 788.9, 436300, 0.079, 0.1, "titania.jpg");
    ofCelestialBody oberon  = ofCelestialBody("Oberon", 761.4, 583500, 0.068, 0.1, "oberon.jpg");
    
    // Neptune
    ofCelestialBody triton = ofCelestialBody("Triton", 1353.4, 354759, 156.865, 0.1, "triton.jpg");
    
    //
    // -----------------------------
    
    // Sun
    ofCelestialBody sun = ofCelestialBody(	"Sun", 696342, 0, 0 /* 7.25 */, 25.38, "th_sun.png" );
    bodies.push_back( sun );
    
    bodies.push_back( ofCelestialBody("Mercury", 2439.7, 57909227, 0,  58.646, "mercury.jpg" ));
    bodies.push_back( ofCelestialBody("Venus", 6051.8, 108209475, -2.7 ,  -243.018, "venusclouds.jpg" ));
    
    ofCelestialBody earth =  ofCelestialBody("Earth", 6371.00, 149598262, 23.4393,  0.99726968, "earth.jpg", BOUNDARIES_FILE );
    earth.addMoon( moon);
    bodies.push_back( earth );
    
    bodies.push_back( ofCelestialBody("Mars", 3389.5, 227943824, 25.2,  1.026, "mars.jpg" ));
    
    ofCelestialBody jupiter = ofCelestialBody("Jupiter", 69911, 778340821, 3.1,  0.41354, "jupiter.jpg" );
        jupiter.addMoon( io );
        jupiter.addMoon( europa );
        jupiter.addMoon( ganymede );
        jupiter.addMoon( callisto );
    bodies.push_back( jupiter);
    
    ofCelestialBody saturn = ofCelestialBody("Saturn", 58232, 1426666422, 26.7,  0.444, "saturn.jpg" );
    saturn.addRing(74500, 140000, "saturnringcolor.jpg", "saturnringpattern.gif");
        saturn.addMoon(enceladus);
        saturn.addMoon(tethys);
        saturn.addMoon(dione);
        saturn.addMoon(rhea);
        saturn.addMoon(titan);
        saturn.addMoon(iapetus);
    bodies.push_back( saturn );
    
    ofCelestialBody uranus = ofCelestialBody("Uranus", 25362, 2870658186, -97.8 ,  -0.718, "uranus.jpg" );
        uranus.addRing(38000, 51140, "uranusringcolor.jpg", "uranusringtrans.gif");
        uranus.addMoon(miranda);
    	uranus.addMoon(ariel);
        uranus.addMoon(umbriel);
        uranus.addMoon(titania);
        uranus.addMoon(oberon);
    
    bodies.push_back( uranus );
    
    ofCelestialBody neptune = ofCelestialBody("Neptune", 24622, 4498396441, 28.3,  0.671, "neptune.jpg" );
        neptune.addMoon(triton);
    bodies.push_back( neptune);
    
    
    
}

void ofSolarSystem::update(){
    
    
    //-----------------------------------------------------------------
    // Set planets and moons positions in SIZE mode
    //
    if (mode == SIZE){
        
        //-----------------------------------------------------------------
        // put bodies one next to another
        double dist = 0;
        
        for(int i = 0; i < bodies.size(); i++){
            
            if (dist > 0)
                dist += bodies[i].extent + param.bodySpacing;
            
            // Moons
            double moonDist = 0;
            if (bodies[i].moons.size() > 0){
                
                moonDist = bodies[i].extent;
                for(int j = 0; j < bodies[i].moons.size(); j++ ){

                    moonDist += bodies[i].moons[j].radius + param.bodySpacing;
                    
                    bodies[i].moons[j].setPosition(ofVec3f(0, 0, -moonDist));
                    
                }
                
                dist += moonDist - bodies[i].extent/2;

            }
            
            
            bodies[i].setPosition(ofVec3f(0, 0, -dist));
            
            dist += moonDist + bodies[i].extent + param.bodySpacing;
            
        }
    }
    
    //-----------------------------------------------------------------
    // Set planets and moons positions in DISTANCE mode
    //
    if (mode == DISTANCE){
        
        
        //-----------------------------------------------------------------
        // put bodies with real relative distances from Sun's surface
        double dist;
        double sunRadius;
        float factor = param.radiusFactor == param.distanceFactor ? 0 : 1;
        
        for(int i = 0; i < bodies.size(); i++){

            
            if (i == 0){
                dist = 0;
                sunRadius = bodies[i].extent;
            }
            else{
                dist = bodies[i].distance + factor*sunRadius;
            }
            
            bodies[i].setPosition(ofVec3f(0, 0, -dist));
            for(int j = 0; j < bodies[i].moons.size(); j++ ){
                
                float moonDist = bodies[i].moons[j].distance + factor*bodies[i].radius;
                bodies[i].moons[j].setPosition(ofVec3f(0, 0, -moonDist));
                
            }

            
        }
    }
   
}

void ofSolarSystem::draw(bool axis, bool textured, bool boundaries){

    for(int i = 0; i < bodies.size(); i++){
        bodies[i].draw(axis, textured, boundaries);
    }
    
}

void ofSolarSystem::distanceInPercentTo(float percent){
    
}