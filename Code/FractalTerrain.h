//
//  FractalTerrain.h
//  Avery Rapson Jacob London
//
//  12/10/15.


#ifndef __LondonLab45__FractalTerrain__
#define __LondonLab45__FractalTerrain__

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <time.h>

#include "RGB.cpp"
#include <cstdlib>

using namespace std;
//template<typename T>
class FractalTerrain{
    
public:
    std::vector<vector<double> > terrain;
    double roughness, min, max;
    int divisions;
    
    
    // implement the fractal terrain map
    
    FractalTerrain (int lod, double rrr) {
        srand(time(NULL));
        roughness = rrr;
        divisions = 1 << lod;
        terrain.resize(divisions+1, vector<double>(divisions +1, 0));

        
        
        //VECTOR MIGHT NOT LIKE DOUBLE BRACKET OPERATORS
        terrain[0][0] = rnd ();
        terrain[0][divisions] = rnd ();
        terrain[divisions][divisions] = rnd ();
        terrain[divisions][0] = rnd ();
        
        double rough = roughness;
        for (int i = 0; i < lod; ++ i) {
            int q = 1 << i, r = 1 << (lod - i), s = r >> 1; //Q ISN'T USED
            for (int j = 0; j < divisions; j += r)
                for (int k = 0; k < divisions; k += r)
                    diamond (j, k, r, rough);
            if (s > 0)
                for (int j = 0; j <= divisions; j += s)
                    for (int k = (j + s) % r; k <= divisions; k += r)
                        square (j - s, k - s, r, rough);
            rough *= roughness;
        }
        
        min = max = terrain[0][0];
        for (int i = 0; i <= divisions; ++ i)
            for (int j = 0; j <= divisions; ++ j)
                if (terrain[i][j] < min) min = terrain[i][j];
                else if (terrain[i][j] > max) max = terrain[i][j];
    }
    
    void diamond (int x, int y, int side, double scale) {
        if (side > 1) {
            int half = side / 2;
            double avg = (terrain[x][y] + terrain[x + side][y] +
                          terrain[x + side][y + side] + terrain[x][y + side]) * 0.25;
            terrain[x + half][y + half] = avg + rnd () * scale;
        }
    }
    
    
    void square (int x, int y, int side, double scale) {
        int half = side / 2;
        double avg = 0.0, sum = 0.0;
        if (x >= 0)
        { avg += terrain[x][y + half]; sum += 1.0; }
        if (y >= 0)
        { avg += terrain[x + half][y]; sum += 1.0; }
        if (x + side <= divisions)
        { avg += terrain[x + side][y + half]; sum += 1.0; }
        if (y + side <= divisions)
        { avg += terrain[x + half][y + side]; sum += 1.0; }
        terrain[x + half][y + half] = avg / sum + rnd () * scale;
    }
    
    
    double rnd () {
        //        double t = rand() %100;
        //        t = t * .001;
        
        return 2. * rand()/(double)RAND_MAX-1.0;//.nextDouble () - 1.0;
    }
    
    
    double getAltitude (double i, double j) {
        double alt = terrain[(int) (i * divisions)][(int) (j * divisions)];
        return (alt - min) / (max - min);
    }
    
    
    RGB blue = *new RGB (0.0, 0.0, 1.0);
    RGB green = *new RGB (0.0, 1.0, 0.0);
    RGB white = *new RGB (1.0, 1.0, 1.0);
    
    RGB getColor (double i, double j) {
        double a = getAltitude (i, j);
        if (a < .5)
            return blue.add (green.subtract (blue).scale ((a - 0.0) / 0.5));
        else
            return green.add (white.subtract (green).scale ((a - 0.5) / 0.5));
    }
};
#endif /* defined(__LondonLab45__FractalTerrain__) */