//
//  Triangle.h
//  Avery Rapson Jacob London
//
//  12/10/15.

#ifndef __Project4__Triangle__
#define __Project4__Triangle__


#include "Triangle.h"
#include "Triple.h"
#include "RGB.h"
#include <iostream>
#include <vector>
class Triangle {
public:
    int i[3] = {};
    int j[3] = {};
    Triple n;
    // std::vector<int> vector;
    RGB color;
    
    Triangle(int trip = 0){
        i[0] = 0;
        i[1] = 0;
        i[2] = 0;
        j[0] = 0;
        j[1] = 0;
        j[2] = 0;
        
    }
    
    
    Triangle (int i0, int j0, int i1, int j1, int i2, int j2) {
        i[0] = i0;
        i[1] = i1;
        i[2] = i2;
        j[0] = j0;
        j[1] = j1;
        j[2] = j2;
    }
};
#endif /* defined(__Project4__Triangle__) */