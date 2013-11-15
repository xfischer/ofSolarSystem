//
//  rd3DUtils.h
//  RealDirections
//
//  Created by Xavier Fischer on 01/11/2013.
//
//

#ifndef __RealDirections___rd3DUtils__
#define __RealDirections___rd3DUtils__

#include <iostream>
#include "ofMain.h"

class rd3DUtils{
    
public:
    
    //Universal function which sets normals
    //for the triangle mesh
    static void setNormals( ofMesh &mesh );

    // Combine a 24bpp color image with a 4bpp GIF image to produce an ARGB image
    // - Both images must have the same size
    // - Black color in GIF means transparent
    static ofImage combineColorAlpha(string colorImageFile, string transpImageFile);
    
};

#endif /* defined(__RealDirections___rd3DUtils__) */
