//
//  Triple.h
//  Avery Rapson Jacob London
//
//  12/10/15.

#ifndef __Project4__Triple__
#define __Project4__Triple__


#include <stdio.h>
#include <cmath>
#include <math.h>
#include <iostream>



using namespace std;
class Triple {
    
public:
    double x, y, z;
    
    
    Triple( int j = 0, int l = 0, int k = 0){ //Triple Initialized to Zero
        x = 0;
        y = 0;
        z = 0;
    }
    
    
    Triple (double x1, double y1, double z1) {
        x = x1;
        y = y1;
        z = z1;
    }
    
    
    Triple add (Triple t) {
        return *new Triple (x + t.x, y + t.y, z + t.z);
    }
    
    Triple subtract (Triple t) {
        return *new Triple (x - t.x, y - t.y, z - t.z);
    }
    
    Triple cross (Triple t) {
        return *new Triple (y * t.z - z * t.y, z * t.x - x * t.z,
                            x * t.y - y * t.x);
    }
    
    double dot (Triple t) {
        return x * t.x + y * t.y + z * t.z;
    }
    
    double dot2 () {
        return x * x + y * y + z * z;
    }
    
    double length2 () {
        return dot(*this);
    }
    
    Triple normalize () {
        return scale (1.0 / sqrt(length2()));
    }
    
    Triple scale (double scale) {
        return *new Triple (x * scale, y * scale, z * scale);
    }
};
#endif /* defined(__Project4__Triple__) */