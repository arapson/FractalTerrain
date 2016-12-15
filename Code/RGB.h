//
//  RGB.h
//  Avery Rapson Jacob London
//
//  12/10/15.

#ifndef __Project4__RGB__
#define __Project4__RGB__

#include <stdio.h>

// The color of the terrain is described simply as an RGB triplet

class RGB {
public:
    double r, g, b;
    int toInt (double value) {
        return (value < 0.0) ? 0 : (value > 1.0) ? 255 :
        (int) (value * 255.0);
    }
    
    
    double toUsable(double f){
        
        float hh = (f == 1.0 ? 255 : f * 256.0);
        return hh;
    }
    
    
    RGB(){
        (*this).r = 0;
        (*this).g = 0;
        (*this).b = 0;
        
    }
    
    double getR(){
        return r;
    }
    
    double getB(){
        return b;
    }
    
    double getG(){
        return g;
    }
    
    RGB(int rgb){
        int alpha = (rgb >> 24) & 0xFF;
        (*this).r = (rgb >> 16) & 0xFF;
        (*this).g = (rgb >> 8) & 0xFF;
        (*this).b = (rgb >> 0) & 0xFF;
    }
    
    RGB (double R, double G, double B) {
        r = R;
        g = G;
        b = B;
    }
    
    RGB add (RGB rgb) {
        return *new RGB (r + rgb.r, g + rgb.g, b + rgb.b);
    }
    
    RGB subtract (RGB rgb) {
        return *new RGB (r - rgb.r, g - rgb.g, b - rgb.b);
    }
    
    RGB scale (double scale) {
        return *new RGB (r * scale, g * scale, b * scale);
    }
    
    int toRGB () {
        return (0xff << 24) | (toInt (r) << 16) |
        (toInt (g) << 8) | toInt (b);
    }
};
#endif /* defined(__LondonLab45__RGB__) */