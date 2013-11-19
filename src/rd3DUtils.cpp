//
//  rd3DUtils.cpp
//  ofSolarSystem
//
//  Created by Xavier Fischer on 01/11/2013.
//
//

#include "rd3DUtils.h"
#include "ofMain.h"

//--------------------------------------------------------------
// Universal function which sets normals for the triangle mesh
//
void rd3DUtils::setNormals( ofMesh &mesh ){
    
	//The number of the vertices
	int nV = mesh.getNumVertices();
	
	//The number of the triangles
	int nT = mesh.getNumIndices() / 3;
    
	vector<ofPoint> norm( nV ); //Array for the normals
    
	//Scan all the triangles. For each triangle add its
	//normal to norm's vectors of triangle's vertices
	for (int t=0; t<nT; t++) {
        
		//Get indices of the triangle t
		int i1 = mesh.getIndex( 3 * t );
		int i2 = mesh.getIndex( 3 * t + 1 );
		int i3 = mesh.getIndex( 3 * t + 2 );
		
		//Get vertices of the triangle
		const ofPoint &v1 = mesh.getVertex( i1 );
		const ofPoint &v2 = mesh.getVertex( i2 );
		const ofPoint &v3 = mesh.getVertex( i3 );
		
		//Compute the triangle's normal
		ofPoint dir = ( (v2 - v1).crossed( v3 - v1 ) ).normalized();
		
		//Accumulate it to norm array for i1, i2, i3
		norm[ i1 ] += dir;
		norm[ i2 ] += dir;
		norm[ i3 ] += dir;
	}
    
	//Normalize the normal's length
	for (int i=0; i<nV; i++) {
		norm[i].normalize();
	}
    
	//Set the normals to mesh
	mesh.clearNormals();
	mesh.addNormals( norm );
}

//--------------------------------------------------------------
// Function that combines a color image and a grayscale image
// with alpha components to output an ARGB image
//
ofImage rd3DUtils::combineColorAlpha(string colorImageFile, string transpImageFile){

    ofImage colorImg;   // color image
    ofImage transpImg;  // alpha component image
    ofImage finalImg;   // final image
    
    // load images
    colorImg.loadImage(colorImageFile);
    transpImg.loadImage(transpImageFile);
    
    int w = colorImg.width;
    int h = colorImg.height;
    
    // Check if image have the same size
    if (transpImg.width != w || transpImg.height != h){
        ofLogError("3DUtils.combineColorAlpha", "Images must be same size");
     
        return colorImg;
    }
    
    // pixel data
    unsigned char *data = new unsigned char[w * h * 4];     // final pixels
    unsigned char *colorData = colorImg.getPixels();        // color image pixels
    unsigned char *transpData = transpImg.getPixels();      // alpha component pixels
    
    // gets number of bytes per pixel
    int colorBpp = colorImg.bpp / 8;
    int transBpp = transpImg.bpp / 8;
    
    for (int y=0; y<h; y++) {
        for (int x=0; x<w; x++) {
            
            int index       = 4 * ( x + w * y);         // output index
            int indexColor  = colorBpp * ( x + w * y);  // index in color image
            int indexTransp = transBpp * ( x + w * y);  // index in grayscale image
            
            data[ index ]       = colorData[ indexColor ];      // copy red
            data[ index + 1 ]   = colorData[ indexColor + 1 ];  // copy green
            data[ index + 2 ]   = colorData[ indexColor + 2 ];  // copy blue
            
            // take alpha component
            // assumes transImg is grayscale so rgb components are all the same
            data[ index + 3 ]   = transpData[ indexTransp ];    // red component from alpha img
        }
    }
    
    // load image from pixel data
    finalImg.setFromPixels( data, w, h, OF_IMAGE_COLOR_ALPHA);
    
    delete [] data;
        
    return finalImg;
    
}