

// Avery Rapson and Jacob London
// Dec. 11, 2015


/* ------------- */

/*   Project 4   */

/* ------------- */


/*
 ---------------------------------------------------------------------------------------------------------------------------------
 Objective of Program:
 
 
 
 + In OpenGl, create a Fractal Generated Terrain using the Diamond-Square algorithm
 + Code provided is Java and if used must be translated to C or C++
 + Fractal terrain should be shaded
 
 
 ---------------------------------------------------------------------------------------------------------------------------------
 
 Controls:
 
 + Q or "Esq" to quit the program
 + Left mouse to swivel terrain
 
 ---------------------------------------------------------------------------------------------------------------------------------
 
 Bugs:
 
 + Vector application on line 444 is not working, however program/lighting works well without it
 
 ---------------------------------------------------------------------------------------------------------------------------------
 
 
 */



#include <iostream>
#include "RGB.h"
#include "Triple.h"
#include "Triangle.h"
#include "FractalTerrain.h"
#include <vector>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GLUT/glut.h>
#include <iostream>
#include <math.h>
#include <array>

using namespace std;

#define SCREEN_WIDTH 640 // We want our screen width 640 pixels
#define SCREEN_HEIGHT 480

int iteration = 0;
int c = 0;

int zz;
int xx;
int yy;
int zTranslated = 0;
int xTranslated = -.5;
int yTranslated = -.7;

//for trackbal
static GLfloat lAngle = 0.0, rAngle = 0.0;   /* in degrees */ //Angle variables for mouse input.
static GLfloat lAngle2 = 0.0, rAngle2 = 0.0;   /* in degrees */ //Angle variables for mouse input.
static int lMoving = 0, rMoving = 0,lStartx, lStarty, rStartx, rStarty; //Additional variables for mouse input.

float trackLocation = 0;






Triple sun;



/* -------------------------------- */
/*    Ambient - Diffuse Lighting    */
/* -------------------------------- */


GLfloat light_ambient_intensity[] = {1.5, 1, 1, 1.0};; //ambient light intensity

GLfloat light_diffuse_intensity[] = { 4,  1, 2, 1}; //diffuse light intensity

GLfloat mat_ambient_color[] = {1, 1 , 1 , 1.0}; //ambient color

GLfloat mat_diffuse_color[] = {1, 1 , 1 ,1}; // diffuse color

GLfloat light_position[] = {0, 5, -5, 0.0};


double exaggeration = 1.3;
int lod = 5;
int steps = 1 << lod;
std::vector<vector<Triple> > map;

std::vector<vector<RGB> > colors;
std::vector<vector<Triple> > normals;
FractalTerrain terrain = *new FractalTerrain (lod, .5);

std::vector<vector<double> > shade;

int numTriangles = (steps * steps * 2);
vector<Triangle> triangles;
int triangle = 0;


/* ------------------------- */
/*     TrackBall Function    */
/* ------------------------- */

void mouse(int button, int state, int x, int y) {
    /* Move the scene with the left mouse button. */
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            lMoving = 1;
            lStartx = x;
            lStarty = y;
        }
        if (state == GLUT_UP) {
            lMoving = 0;
        }
    }
    
}



/*
 * Function used to register mouse motion.
 * Pre: correct capture and initialization of variables from mouse();
 * Post: glutPostRedisplay() is called and mouse input is shown on monitor.
 */
void motion(int x, int y)
{
    if (lMoving) {
        lAngle = lAngle + (x - lStartx);
        lAngle2 = lAngle2 + (y - lStarty);
        lStartx = x;
        lStarty = y;
        glutPostRedisplay();
    }
    else if (rMoving) {
        rAngle = rAngle + (x - rStartx);
        rAngle2 = lAngle2 + (y - rStarty);
        rStartx = x;
        rStarty = y;
        glutPostRedisplay();
    }
}




/* -------------------- */
/*     Init Function    */
/* -------------------- */

void init(void)
{
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);
    glClearColor(0.3, .3, .3, 1 );
    glColor3ub(255, 255, 0);
    glViewport(0,0,width,height);
    // gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f, 150.0f);
    
    glOrtho(-1, 1, -1, 1, -20, 20 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    
    
    //light enabling/disabling
    
    glDisable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    
}


/* ------------------------------------- */
/*     Shadow Function From Java Code    */
/* ------------------------------------- */

void shadow(){
    //    std::vector<vector<double> > shade; //double[][] shade = new double[steps + 1][steps + 1];
    //    shade.resize(steps+1, vector<double>(steps+1, 0));
    for (int i = 0; i <= steps; ++ i) {
        for (int j = 0; j <= steps; ++ j) {
            shade[i][j] = 1.0;
            Triple vertex = map[i][j];
            Triple ray = sun.subtract (vertex);
            double distance = steps * sqrt (ray.x * ray.x + ray.z * ray.z);
            /* step along ray in horizontal units of grid width */
            for (double place = 1.0; place < distance; place += 1.0) { //replaced for loop semi colon with paranthesis
                Triple sample = vertex.add (ray.scale (place / distance));
                double sx = sample.x, sy = sample.y, sz = sample.z;
                if ((sx < 0.0) || (sx > 1.0) || (sz < 0.0) || (sz > 1.0))
                    break; /* steppd off terrain */
                double ground = exaggeration * terrain.getAltitude (sx, sz);
                if (ground >= sy) {
                    shade[i][j] = 0.0;
                    break;
                }
            }
        }
    }
}


/* -------------------------------------- */
/*     Lighting Function From Java Code   */
/* -------------------------------------- */

void lLighting(){
    
    double ambient = .3;
    double diffuse = 4.0;
    sun = *new Triple (3.6, 3.9, 0.6);
    shadow();
    for (int i = 0; i < 3; ++ i) //numtriangles -> 3
        for (int j = 0; j < 3; ++ j)
            normals[i][j] = *new Triple (0.0, 0.0, 0.0);
    /* compute triangle normals and vertex averaged normals */
    for (int i = 0; i < numTriangles; ++ i) {
        Triple v0 = map[triangles[i].i[0]][triangles[i].j[0]],
        v1 = map[triangles[i].i[1]][triangles[i].j[1]],
        v2 = map[triangles[i].i[2]][triangles[i].j[2]];
        Triple normal = v0.subtract (v1).cross (v2.subtract (v1)).normalize ();
        triangles[i].n = normal;
        for (int j = 0; j < 3; ++ j) {
            normals[triangles[i].i[j]][triangles[i].j[j]] =
            normals[triangles[i].i[j]][triangles[i].j[j]].add (normal);
        }
    }
    /* compute vertex colors and triangle average colors */
    for (int i = 0; i < numTriangles; ++ i) {
        RGB avg = *new RGB (0.0, 0.0, 0.0);
        for (int j = 0; j < 3; ++ j) {
            int k = triangles[i].i[j], l = triangles[i].j[j];
            Triple vertex = map[k][l];
            RGB color1 = colors[k][l];
            Triple normal = normals[k][l].normalize ();
            Triple light = vertex.subtract (sun);
            double distance2 = light.length2 ();
            double dot = light.normalize ().dot (normal);
            double shadow = shade[k][l];
            double lighting = ambient + diffuse * ((dot < 0.0) ? - dot : 0.0) /
            distance2 * shadow;
            //            double lighting = ambient + diffuse * ((dot < 0.0) ? - dot : 0.0) / distance2;
            color1 = color1.scale (lighting);
            triangles[i].color = color1;
            //Operator doesn't work triangles[i].color[j] = color1;
            avg = avg.add (color1);
        }
        //triangles[i].color = new Color (avg.scale (1.0 / 3.0).toRGB ());
        triangles[i].color = *new RGB (avg.scale (1.0 / 3.0).toRGB ());
    }
    
    
}



// Following code populates the elements of the terrain grid with fractal terrain data

void start(){
    shade.resize(steps+1, vector<double>(steps+1, 0));
    normals.resize(steps+1, vector<Triple>(steps+1, 0));
    map.resize(steps+1, vector<Triple>(steps+1, 0));
    colors.resize(steps+1, vector<RGB>(steps+1, 0));
    for (int i = 0; i <= steps; ++ i) {
        for (int j = 0; j <= steps; ++ j) {
            double x = 1.0 * i / steps, z = 1.0 * j / steps;
            double altitude = terrain.getAltitude (x, z);
            map[i][j] = *new Triple (x, altitude * exaggeration, z);
            colors[i][j] = terrain.getColor (x, z); //MIGHT NEED TO BE TRIPLE
        }
    }
    
    triangles.resize(numTriangles);
    for (int i = 0; i < steps; ++ i) {
        for (int j = 0; j < steps; ++ j) {
            triangles[triangle ++] = *new Triangle(i, j, i + 1, j, i, j + 1);
            triangles[triangle ++] = *new Triangle (i + 1, j, i + 1, j + 1, i, j + 1);
        }
    }
    lLighting();
}

/* ------------------------ */
/*     Keyboard Function    */
/* ------------------------ */
void processNormalKeys(unsigned char key, int x, int y)
{
    switch ( key )
    {
        case 'q': // q to quit the program
            
            
            exit (0);
            break;
            
        case 27: // also esq to quit the program
            exit(0);
            break;
            
        case ' ': //resets display
            
            glutPostRedisplay();
            break;
            
            
        default:
            break;
    }
    
}

/* ------------------------ */
/*     Reshape Function     */
/* ------------------------ */

void reshape(int w, int h)
{
    // Prevent a divide by zero, when window is too short
    // (you cant make a window of zero width).
    if(h == 0)
    {
        h = 1;
    }
    float ratio = 1.0* w / h;
    
    glViewport(0, 0, w, h);
    // Use the Projection Matrix
    glMatrixMode(GL_PROJECTION);
    // Reset Matrix
    glLoadIdentity();
    // Set the viewport to be the entire window
    
    
    glMatrixMode(GL_MODELVIEW);
    
    // recall identity
    glLoadIdentity();
}





/* ---------------------- */
/*     Display Function   */
/* ---------------------- */
void display(void)
{
    
    //set up lighting
    glDisable(GL_LIGHTING);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    
    //TRACKBALL
    //LEFT MOUSE
    glPushMatrix();
    glTranslated(xTranslated, yTranslated, zTranslated);
    glRotatef(lAngle2, 1.0, 0.0, 0.0);
    glRotatef(lAngle, 0.0, 1.0, 0.0);
    glTranslated(-xTranslated, 0, -zTranslated);
    
    
    // begin drawing
    {
        glEnable(GL_LIGHTING);
        
        
        glPushMatrix();
        
        // set up lighting
        
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient_intensity);
        
        
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse_intensity);
        
        glEnable(GL_LIGHT0);
        
        
        glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient_color);
        
        glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse_color);
        
        glEnable(GL_LIGHT0);
        
        glTranslated(xTranslated, yTranslated, zTranslated);
        
        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
        
        
        glTranslated(.5, -.7, 0);
        glRotated(220,0,1,0);
        
        // populate screen with triangles
        for (int i = 0; i < numTriangles; ++i) {
            for (int j = 0; j < 3; ++ j) {
                
                
                int k = triangles[i].i[j], l = triangles[i].j[j];
                
                Triple vertex = map[k][l];
                
                // change lighting values to reflect depth
                GLfloat mat_ambient_color2[] = { (GLfloat)triangles[i].color.r/240, (GLfloat)triangles[i].color.g/300, (GLfloat)triangles[i].color.b/255 , 0};
                
                // draw
                glBegin(GL_TRIANGLES);
                
                glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE, mat_ambient_color2);
                
                
                // vectors not working :(
                //glNormal3f(normals[j][i].x, normals[j][i].y, normals[j][i].z);
                
                iteration ++;
                glVertex3d(vertex.x, vertex.y, vertex.z);
                
            }
            
            
            
            
            iteration = 0;
        }
        glEnd();
        
        glDisable(GL_LIGHT0);
        
        glPopMatrix();
        
    }
    glPopMatrix();
    glutSwapBuffers();
}




/* -------------------------- */
/*     Main Function          */
/* -------------------------- */

int main(int argc, char *argv[])
{
    start();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(100,100);
    glutCreateWindow("Project 4 Rapson and London");
    
    init();
    glutDisplayFunc(display);
    //    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    //    glutSpecialFunc(processSpecialKeys);
    glutKeyboardFunc(processNormalKeys);
    glutMouseFunc(mouse);
    glEnable(GL_DEPTH_TEST);
    glutMotionFunc(motion);
    glutMainLoop();
    return 0;
}