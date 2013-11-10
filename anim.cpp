////////////////////////////////////////////////////
// anim.cpp version 4.1
// Template code for drawing an articulated figure.
// CS 174A 
////////////////////////////////////////////////////

#ifdef WIN32
#include <windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#ifdef WIN32
#include "GL/freeglut.h"
#else
#include <GLUT/glut.h>
#endif

#include "Ball.h"
#include "FrameSaver.h"
#include "Timer.h"
#include "Shapes.h"
#include "tga.h"

#include "Angel/Angel.h"

#ifdef __APPLE__
#define glutInitContextVersion(a,b)
#define glutInitContextProfile(a)
#define glewExperimental int glewExperimentalAPPLE
#define glewInit()
#endif

FrameSaver FrSaver ;
Timer TM ;

BallData *Arcball = NULL ;
int Width = 500 ;
int Height = 500 ;
int Button = -1 ;
float Zoom = 1 ;
int PrevY = 0 ;

int Animate = 0 ;
int Recording = 0 ;

float prologue = 0; // SET THIS TO ZERO
float timeline = 0; // SET THIS TO ZERO
float rotation = 0;
float rotateX = 0;
float rotateZ = 0;
float grassSway = 0;
float leafSway = 0;
float moveLegs = 0;
float prevX = 0;
float prevY = 0;
float temp = 0;
float temp2 = 0;
float temp3 = 0;
float pathBeeHor = 0;
float pathBeeVert = 0;
float pathWings = 0;
float pathLegs = 0;

void resetArcball() ;
void save_image();
void instructions();
void set_colour(float r, float g, float b) ;

const int STRLEN = 100;
typedef char STR[STRLEN];

#define PI 3.1415926535897
#define X 0
#define Y 1
#define Z 2

// Structs that hold the Vertex Array Object index and number of vertices of each shape.
ShapeData grassData;
ShapeData heartData;
ShapeData cubeData;
ShapeData sphereData;
ShapeData coneData;
ShapeData cylData;

// Matrix stack that can be used to push and pop the modelview matrix.
class MatrixStack {
    int    _index;
    int    _size;
    mat4*  _matrices;

   public:
    MatrixStack( int numMatrices = 32 ):_index(0), _size(numMatrices)
        { _matrices = new mat4[numMatrices]; }

    ~MatrixStack()
	{ delete[]_matrices; }

    void push( const mat4& m ) {
        assert( _index + 1 < _size );
        _matrices[_index++] = m;
    }

    mat4& pop( void ) {
        assert( _index - 1 >= 0 );
        _index--;
        return _matrices[_index];
    }
};

MatrixStack  mvstack;
mat4         model_view;
GLint        uModelView, uProjection, uView;
GLint        uAmbient, uDiffuse, uSpecular, uLightPos, uShininess;
GLint        uTex, uEnableTex;

// The eye point and look-at point.
// Currently unused. Use to control a camera with LookAt().
double eye[4] = {0.0, 0.0, 0.0, 1.0};
double ref[4] = {0.0, 0.0, 0.0, 1.0};
double up[4] = {0.0, 1.0, 0.0, 0.0};

vec4 cam_eye = vec4(0,0,0,1);
vec4 cam_ref = vec4(0,0,0,1);
vec4 cam_up = vec4(0,1,0,0);

double TIME = 0.0 ;

/////////////////////////////////////////////////////
//    PROC: drawCylinder()
//    DOES: this function 
//          render a solid cylinder  oriented along the Z axis. Both bases are of radius 1. 
//          The bases of the cylinder are placed at Z = 0, and at Z = 1.
//
//          
// Don't change.
//////////////////////////////////////////////////////
void drawCylinder(void)
{
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cylData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cylData.numVertices );
}

//////////////////////////////////////////////////////
//    PROC: drawCone()
//    DOES: this function 
//          render a solid cone oriented along the Z axis with base radius 1. 
//          The base of the cone is placed at Z = 0, and the top at Z = 1. 
//         
// Don't change.
//////////////////////////////////////////////////////
void drawCone(void)
{
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( coneData.vao );
    glDrawArrays( GL_TRIANGLES, 0, coneData.numVertices );
}


//////////////////////////////////////////////////////
//    PROC: drawCube()
//    DOES: this function draws a cube with dimensions 1,1,1
//          centered around the origin.
// 
// Don't change.
//////////////////////////////////////////////////////

void drawCube(void)
{
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
}


//////////////////////////////////////////////////////
//    PROC: drawSphere()
//    DOES: this function draws a sphere with radius 1
//          centered around the origin.
// 
// Don't change.
//////////////////////////////////////////////////////

void drawSphere(void)
{ 
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( sphereData.vao );
    glDrawArrays( GL_TRIANGLES, 0, sphereData.numVertices );
}

void drawHcube(void)
{
	glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( heartData.vao );
    glDrawArrays( GL_TRIANGLES, 0, heartData.numVertices );
}

void drawGcube(void)
{
	glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( grassData.vao );
    glDrawArrays( GL_TRIANGLES, 0, grassData.numVertices );
}

void drawGrass(void)
{
	mvstack.push(model_view);
		set_colour(0.5f, 1.0f, 0.5f);
		model_view *= RotateZ(grassSway);
		drawGcube();
		model_view *= RotateZ(-grassSway);
		model_view *= Translate(1.0f, 0.0f, 0.0f);
		model_view *= RotateZ(grassSway);
		drawGcube();
		model_view *= RotateZ(-grassSway);
		model_view *= Translate(1.0f, 0.0f, 0.0f);
		model_view *= RotateZ(grassSway);
		drawGcube();
	model_view = mvstack.pop();
}

void drawHeart(void)
{
	mvstack.push(model_view);
		set_colour(1.0f, 0.0f, 1.0f);
		model_view *= RotateZ(-45);
		drawHcube();
	model_view = mvstack.pop();
}

void drawWall(void)
{
    glUniform1i( uEnableTex, 1 );
    glUniformMatrix4fv( uModelView, 1, GL_TRUE, model_view );
    glBindVertexArray( cubeData.vao );
    glDrawArrays( GL_TRIANGLES, 0, cubeData.numVertices );
    glUniform1i( uEnableTex, 0 );
}

void drawPlank(void)
{
	mvstack.push(model_view);
		set_colour(0.5f, 0.35f, 0.05f);
		model_view *= Scale(16.0f, 1.0f, 0.25f);
		drawCube();
	model_view = mvstack.pop();
}

void drawBar(void)
{
	mvstack.push(model_view);
		set_colour(0.33f, 0.33f, 0.33f);
		model_view *= Scale(0.25f, 3.0f, 0.25f);
		drawCube();
	model_view = mvstack.pop();
}

void drawBench(void)
{
	mvstack.push(model_view);
		model_view *= Translate(0.0f, 2.25f, 0.0f);
		drawPlank();
		model_view *= Translate(0.0f, -1.25f, 0.0f);
		drawPlank();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		model_view *= RotateX(90);
		model_view *= Translate(0.0f, 1.0f, 0.0f);
		drawPlank();
		model_view *= Translate(0.0f, 1.25f, 0.0f);
		drawPlank();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		model_view *= Scale(1.0f, 1.5f, 1.0f);
		model_view *= Translate(-5.0f, 0.0f, -0.25f);
		drawBar();
		model_view *= Translate(10.0f, 0.0f, 0.0f);
		drawBar();
	model_view = mvstack.pop();

	mvstack.push(model_view);
		model_view *= RotateX(90);
		model_view *= Translate(-5.0f, 1.25f, 0.25f);
		drawBar();
		model_view *= Translate(10.0f, 0.0f, 0.0f);
		drawBar();
		model_view *= Translate(0.0f, 1.36f, 1.0f);
		model_view *= Scale(1.0f, 1.0f, 0.66f);
		model_view *= RotateX(-90);
		drawBar();
		model_view *= Translate(-10.0f, 0.0f, 0.0f);
		drawBar();
	model_view = mvstack.pop();
}

void drawScene(void)
{
	// Draw Ground
	mvstack.push(model_view);
		set_colour(0.5,1,0.5);
		model_view *= Translate(0.0f, -4.75f, 0.0f);
		model_view *= Scale(100.0f, 1.0f, 100.0f);
		drawCube();
	model_view = mvstack.pop();

	// Draw Grass
	mvstack.push(model_view);
		model_view *= Translate(10.0f, -4.0f, 0.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(12.0f, -4.0f, 3.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(-11.0f, -4.0f, 4.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(-9.0f, -4.0f, -2.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(-4.0f, -4.0f, 5.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(-3.0f, -4.0f, 9.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(-5.0f, -4.0f, -15.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(6.0f, -4.0f, -14.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(7.0f, -4.0f, -25.0f);
		drawGrass();
	model_view = mvstack.pop();
	mvstack.push(model_view);
		model_view *= Translate(-8.0f, -4.0f, -18.0f);
		drawGrass();
	model_view = mvstack.pop();
	
	// Draw Walls
	mvstack.push(model_view);

		set_colour(0.9,0.9,1);

		mvstack.push(model_view);
			model_view *= RotateX(90);
			model_view *= RotateY(90);
			model_view *= Translate(20.0f, -50.0f, 0.0f);
			model_view *= Scale(50.0f, 1.0f, 100.0f);
			drawWall();
			model_view *= Translate(0.0f, 100.0f, 0.0f);
			drawWall();
		model_view = mvstack.pop();

		mvstack.push(model_view);
			model_view *= Translate(50.0f, 20.0f, 0.0f);
			model_view *= Scale(1.0f, 50.0f, 100.0f);
			drawWall();
			model_view *= Translate(-100.0f, 0.0f, 0.0f);
			drawWall();
		model_view = mvstack.pop();

	model_view = mvstack.pop();

    // Draw Bench
	mvstack.push(model_view);
		model_view *= Translate(0.0f, -2.0f, 0.0f);
		drawBench();
	model_view = mvstack.pop();
}

void drawLeaf(void)
{
	mvstack.push(model_view);
		set_colour(0.4f, 1.0f, 0.4f);
		model_view *= Scale(1.0f, 0.1f, 0.5f);
		model_view *= RotateY(45);
		drawCube();
	model_view = mvstack.pop();
}

void drawExclam(void)
{
	mvstack.push(model_view);
		set_colour(1.0f, 1.0f, 0.0f);
		model_view *= Scale(0.5f, 1.5f ,0.5f);
		drawCube();
		model_view *= Scale(0.5f, 1.0f/6.0f, 0.5f);
		model_view *= Translate(0.0f, -5.0f, 0.0f);
		drawSphere();
	model_view = mvstack.pop();
}

void drawHearts(void)
{
	mvstack.push(model_view);

		if(timeline > 56)
			model_view *= RotateY(-90);

		if((timeline > 15.5f && timeline < 16.0f) ||
			(timeline > 56 && timeline < 56.5)) {
			mvstack.push(model_view);
				model_view *= Translate(0.0f, 2.0f, 0.0f);
				drawHeart();
			model_view = mvstack.pop();
		}
		if((timeline > 15.75f && timeline < 16.25f) ||
			(timeline > 56.25 && timeline < 56.75)) {
			mvstack.push(model_view);
				model_view *= Translate(1.0f, 2.0f, 0.0f);
				model_view *= RotateZ(-15);
				drawHeart();
			model_view = mvstack.pop();
		}
		if((timeline > 16.0f && timeline < 16.5f) ||
			(timeline > 56.5 && timeline < 57.0)) {
			mvstack.push(model_view);
				model_view *= Translate(-2.0f, 2.5f, 0.0f);
				model_view *= RotateZ(15);
				drawHeart();
			model_view = mvstack.pop();
		}
		if((timeline > 16.25f && timeline < 16.75f) || 
			(timeline > 56.75)) {
			mvstack.push(model_view);
				if(timeline > 56.75)
					model_view *= Translate(0.0f, 3.0f+temp3, 0.0f);
				else
					model_view *= Translate(0.0f, 3.0f, 0.0f);
				drawHeart();
			model_view = mvstack.pop();
		}
	model_view = mvstack.pop();
}

void drawBulb(void)
{
	mvstack.push(model_view);
		set_colour(1.0f, 1.0f, 0.0f);
		drawSphere();

			mvstack.push(model_view);
				model_view *= Translate(0.0f, 2.0f, 0.0f);
				model_view *= Scale(0.25f, 1.0f, 0.25f);
				drawCube();
			model_view = mvstack.pop();

			mvstack.push(model_view);
				model_view *= RotateZ(45);
				model_view *= Translate(0.0f, 2.0f, 0.0f);
				model_view *= Scale(0.25f, 1.0f, 0.25f);
				drawCube();
			model_view = mvstack.pop();

			mvstack.push(model_view);
				model_view *= RotateZ(-45);
				model_view *= Translate(0.0f, 2.0f, 0.0f);
				model_view *= Scale(0.25f, 1.0f, 0.25f);
				drawCube();
			model_view = mvstack.pop();

			mvstack.push(model_view);
				model_view *= RotateZ(90);
				model_view *= Translate(0.0f, 2.0f, 0.0f);
				model_view *= Scale(0.25f, 1.0f, 0.25f);
				drawCube();
				model_view *= Translate(0.0f, -4.0f, 0.0f);
				drawCube();
			model_view = mvstack.pop();

		model_view *= Translate(0.0f, -1.0f, 0.0f);
		set_colour(0.5f, 0.5f, 0.5f);
		drawCube();
	model_view = mvstack.pop();
}

void drawFlower(void)
{
	mvstack.push(model_view);
		drawHeart();
		model_view *= Translate(0.0f, -1.5f, 0.0f);
		set_colour(0.0f, 0.75f, 0.0f);
		model_view *= Scale(0.25f, 2.0f, 0.25f);
		drawCube();
	model_view = mvstack.pop();
}

void drawBouquet(void)
{
	mvstack.push(model_view);
		model_view *= RotateX(-90);
		drawCone();
		model_view *= RotateX(90);
		model_view *= Translate(0.0f, 2.0f, 0.0f);
		drawFlower();

		mvstack.push(model_view);
			model_view *= RotateZ(25);
			model_view *= Translate(-1.0f, 0.25f, 0.0f);
			drawFlower();
		model_view = mvstack.pop();

		mvstack.push(model_view);
			model_view *= RotateZ(-25);
			model_view *= Translate(1.0f, 0.25f, 0.0f);
			drawFlower();
		model_view = mvstack.pop();
	model_view = mvstack.pop();
}

void drawLeg(void)
{
    mvstack.push(model_view);
		set_colour(0,0,0);
		model_view *= RotateY(90); //glRotatef(90,0,1,0);
		model_view *= RotateZ(45); //glRotatef(45,0,0,1);
		model_view *= Scale(0.25f, 0.75f, 0.25f); //glScalef(0.25,0.75,0.25);
		drawCube();
		model_view *= Scale(1.0f, 0.4f, 1.0f); //glScalef(1,0.4,1);
		model_view *= RotateZ(-67.5); //glRotatef(-67.5,0,0,1);
		model_view *= Translate(0.5f, -0.66f, 0.0f); //glTranslatef(0.5,-0.66,0);
		model_view *= RotateZ(pathLegs); //glRotatef(pathLegs,0,0,1);
		model_view *= Translate(1.0f, -1.5f, 0.0f); //glTranslatef(1,-1.5,0);
		model_view *= Scale(1.0f, 3.0f, 1.0f); //glScalef(1,3,1);
		drawCube();
	model_view = mvstack.pop();
}

void drawBee(void)
{
    //Draw head
    mvstack.push(model_view); //glPushMatrix();
		set_colour(0,0,1);
		model_view *= Scale(0.5f, 0.5f, 0.5f); //glScalef(0.5,0.5,0.5);
		model_view *= Translate(-3.0f, 0.0f, 0.0f); //glTranslatef(-3,0,0);
		drawSphere();
    model_view = mvstack.pop(); //glPopMatrix();

    //Draw body
    mvstack.push(model_view); //glPushMatrix();
		set_colour(0,0,0);
		model_view *= Scale(2.0f, 1.0f, 1.0f); //glScalef(2,1,1);
		drawCube();
    model_view = mvstack.pop(); //glPopMatrix();

    //Draw left wing
    mvstack.push(model_view); //glPushMatrix();
		set_colour(0.25,0.25,0.25);
		model_view *= Translate(0.0f, 0.5f, 0.5f); //glTranslatef(0,0.5,0.5);
		model_view *= RotateX(-pathWings); //glRotatef(-pathWings,1,0,0);
		model_view *= Translate(0.0f, 0.0f, 1.25f); //glTranslatef(0,0,1.25);
		model_view *= Scale(1.0f, 0.125f, 2.5f); //glScalef(1,0.125,2.5);
		drawCube();
    model_view = mvstack.pop(); //glPopMatrix();

    //Draw right wing
    mvstack.push(model_view); //glPushMatrix();
		set_colour(0.25,0.25,0.25);
		model_view *= Translate(0.0f, 0.5f, -0.5f); //glTranslatef(0,0.5,-0.5);
		model_view *= RotateX(pathWings); //glRotatef(pathWings,1,0,0);
		model_view *= Translate(0.0f, 0.0f, -1.25f); //glTranslatef(0,0,-1.25);
		model_view *= Scale(1.0f, 0.125f, 2.5f); //glScalef(1,0.125,2.5);
		drawCube();
    model_view = mvstack.pop(); //glPopMatrix();

    //Draw legs
	mvstack.push(model_view); //glPushMatrix();
		model_view *= Translate(-1.0f, -0.65f, -0.85f); //glTranslatef(-1,-0.65,-0.85);
		for(int legs = 0; legs < 3; legs++)
		{
			model_view *= Translate(0.5f, 0.0f, 0.0f); //glTranslatef(0.5,0,0);
			drawLeg();
		}
		model_view *= RotateY(180); //glRotatef(180,0,1,0);
		model_view *= Translate(-0.5f, 0.0f, -1.7f); //glTranslatef(-0.5,0,-1.7);
		for(int legs = 0; legs < 3; legs++)
		{
			model_view *= Translate(0.5f, 0.0f, 0.0f); //glTranslatef(0.5,0,0);
			drawLeg();
		}
    model_view = mvstack.pop(); //glPopMatrix();

    //Draw butt
    mvstack.push(model_view); //glPushMatrix();
		set_colour(1,1,0);
		model_view *= Translate(2.5f, 0.0f, 0.0f); //glTranslatef(2.5,0,0);
		model_view *= Scale(1.5f, 0.75f, 0.75f); //glScalef(1.5,0.75,0.75);
		drawSphere();
    model_view = mvstack.pop(); //glPopMatrix();
}

void drawMale(float r, float g, float b)
{
	mvstack.push(model_view);
		model_view *= Translate(0.0f, 1.0f, 0.0f);

		// Draw Head
		mvstack.push(model_view);
			model_view *= RotateY(90);
			if(timeline < 7.0f)
				model_view *= RotateX(-30+timeline*7);
			if(timeline > 8.0f && timeline < 9.0f) {
				model_view *= Translate(0.0f, 3.0f, 0.0f);
				drawExclam();
				model_view *= Translate(0.0f, -3.0f, 0.0f);
			}
			if(timeline > 13.5f && -temp > -90 && timeline < 18.0)
				model_view *= RotateY(-temp);
			else if(timeline > 13.5 && -temp <= -90 && timeline < 18.0)
				model_view *= RotateY(-90);

			if(timeline > 18) {
				if(timeline < 19.75 && -90+temp*1.25 < 0)
					model_view *= RotateY(-90+temp*1.25);
				if(timeline > 24.0)
					if(-temp*30 > -90 && timeline < 27.0)
						model_view *= RotateY(-temp*30);
					else
						model_view *= RotateY(-90);
				if(timeline > 27.25 && timeline < 29.5)
					model_view *= RotateX(temp*30);
				if(timeline > 29.5 && timeline < 30.5) {
					mvstack.push(model_view);
						model_view *= Translate(0.0f, 2.0f, 0.0f);
						model_view *= Scale(0.5f, 0.5f, 0.5f);
						drawBulb();
					model_view = mvstack.pop();
				}
				if(timeline > 34.0 && timeline < 35.0)
				{
					mvstack.push(model_view);
						model_view *= Translate(0.0f, 3.0f, 0.0f);
						drawExclam();
					model_view = mvstack.pop();
				}
				if(timeline > 47.0 && timeline < 54.25) {
						model_view *= RotateY(180);
						if(timeline < 50.0) {
							model_view *= RotateX(-temp2);
							prevX = -temp2;
						}
						else
							model_view *= RotateX(prevX);
				}
				if(timeline > 54.5)
						model_view *= RotateY(temp*2);
			}

			//Draw Hearts
			drawHearts();

			set_colour(r, g, b);
			drawSphere();
			set_colour(0.0f, 0.0f, 0.0f);
			model_view *= Translate(0.5f, 0.0f, 0.75f);
			model_view *= Scale(0.25f, 0.25f, 0.25f);
			drawSphere();
			model_view *= Translate(-4.0f, 0.0f, 0.0f);
			drawSphere();
		model_view = mvstack.pop();

		set_colour(r, g, b);
		model_view *= Translate(0.0f, -2.0f, 0.0f);
		model_view *= Scale(1.5f, 2.0f, 1.5f);
		drawCube();
	model_view = mvstack.pop();

	// Draw Arms
	mvstack.push(model_view);
		model_view *= Translate(1.25f, -0.5f, 0.0f);
		drawCube();
		model_view *= Translate(-2.5f, 0.0f, 0.0f);
		drawCube();
		if(timeline > 30.5) {
			model_view *= Translate(0.0f, 0.5f, 1.0f);
			model_view *= RotateX(45);
			model_view *= RotateY(90);
			if(timeline < 43.0)
				drawBouquet();
		}
	model_view = mvstack.pop();

	// Draw Legs Sitting
	mvstack.push(model_view);

		model_view *= Translate(0.5f, -2.0f, 1.0f);

		if((timeline > 20.0f && timeline < 23.0f) ||
			(timeline > 24.0f && timeline < 27.0f))
			model_view *= RotateX(moveLegs);

		if(timeline < 30.5 || timeline >= 49.0f)
			drawCube();

		if((timeline > 20.0f && timeline < 23.0f) ||
			(timeline > 24.0f && timeline < 27.0f))
			model_view *= RotateX(-moveLegs);

		model_view *= Translate(-1.0f, 0.0f, 0.0f);

		if((timeline > 20.0f && timeline < 23.0f) ||
			(timeline > 24.0f && timeline < 27.0f))

			model_view *= RotateX(-moveLegs);

		if(timeline < 30.5 || timeline >= 49.0f)
			drawCube();

	model_view = mvstack.pop();

	// Draw Legs Standing
	if(timeline >= 30.5f && timeline < 49.0f) {
		set_colour(r,g,b);
		model_view *= Translate(0.5f, -2.5f, 0.0f);
		mvstack.push(model_view);
			if(timeline < 34.0 || timeline > 35.0 && temp2 < 520)
				model_view *= RotateX(moveLegs);
			drawCube();
		model_view = mvstack.pop();
		model_view *= Translate(-1.0f, 0.0f, 0.0f);
		mvstack.push(model_view);
			if(timeline < 34.0 || timeline > 35.0 && temp2 < 520)
				model_view *= RotateX(-moveLegs);
			drawCube();
		model_view = mvstack.pop();
	}
}

void drawFemale(float r, float g, float b)
{
	mvstack.push(model_view);
		model_view *= Translate(0.0f, 1.0f, 0.0f);

		// Draw Head
		mvstack.push(model_view);
			set_colour(r, g, b);
			if(timeline > 21.0f && timeline < 24.0f)
				model_view *= RotateY(-temp2);
			if(timeline >= 24.0f && (-45+(temp2-45)*2) < 0)
				model_view *= RotateY(-45+(temp2-45)*2);
			if(timeline > 35.0 && timeline < 37.0f) {
				model_view *= RotateY(45-(temp2-230)*5);
				prevY = 45-(temp2-230)*5;
			}
			if(timeline > 37.0f && timeline < 38.0f)
				model_view *= RotateY(prevY);
			if(timeline > 38.0f && prevY+(temp2-255) < 0)
				model_view *= RotateY(prevY+(temp2-255));
			drawSphere();
			set_colour(0.0f, 0.0f, 0.0f);
			model_view *= Translate(0.5f, 0.0f, 0.75f);
			model_view *= Scale(0.25f, 0.25f, 0.25f);
			drawSphere();
			model_view *= Translate(-4.0f, 0.0f, 0.0f);
			drawSphere();
		model_view = mvstack.pop();

		set_colour(r, g, b);
		model_view *= Translate(0.0f, -2.0f, 0.0f);
		model_view *= Scale(1.5f, 2.0f, 1.5f);
		drawCube();
	model_view = mvstack.pop();

	//Draw Arms
	mvstack.push(model_view);
		model_view *= Translate(1.25f, -0.5f, 0.0f);
		drawCube();
		model_view *= Translate(-2.5f, 0.0f, 0.0f);
		drawCube();
	model_view = mvstack.pop();

	//Draw Legs
	if(timeline < 13.5f || (timeline > 40.0f && timeline < 53.75)) {
		model_view *= Translate(0.5f, -2.5f, 0.0f);
		mvstack.push(model_view);
			model_view *= RotateX(moveLegs);
			drawCube();
		model_view = mvstack.pop();
		model_view *= Translate(-1.0f, 0.0f, 0.0f);
		mvstack.push(model_view);
			model_view *= RotateX(-moveLegs);
			drawCube();
		model_view = mvstack.pop();
	}
	if(timeline > 13.5 && timeline < 40.0 || timeline > 53.75) {
		mvstack.push(model_view);
			model_view *= Translate(0.5f, -2.0f, 1.0f);
			drawCube();
			model_view *= Translate(-1.0f, 0.0f, 0.0f);
			drawCube();
		model_view = mvstack.pop();
	}
}

void resetArcball()
{
    Ball_Init(Arcball);
    Ball_Place(Arcball,qOne,0.75);
}


//////////////////////////////////////////////////////
//    PROC: myKey()
//    DOES: this function gets caled for any keypresses
// 
//////////////////////////////////////////////////////

void myKey(unsigned char key, int x, int y)
{
    float time ;
    switch (key) {
        case 'q':
        case 27:
            exit(0); 
        case 's':
            FrSaver.DumpPPM(Width,Height) ;
            break;
        case 'r':
            resetArcball() ;
            glutPostRedisplay() ;
            break ;
        case 'a': // togle animation
            Animate = 1 - Animate ;
            // reset the timer to point to the current time		
            time = TM.GetElapsedTime() ;
            TM.Reset() ;
            // printf("Elapsed time %f\n", time) ;
            break ;
        case '0':
            //reset your object
            break ;
        case 'm':
            if( Recording == 1 )
            {
                printf("Frame recording disabled.\n") ;
                Recording = 0 ;
            }
            else
            {
                printf("Frame recording enabled.\n") ;
                Recording = 1  ;
            }
            FrSaver.Toggle(Width);
            break ;
        case 'h':
        case '?':
            instructions();
            break;
    }
    glutPostRedisplay() ;

}

/*********************************************************
    PROC: myinit()
    DOES: performs most of the OpenGL intialization
     -- change these with care, if you must.

**********************************************************/

void myinit(void)
{
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
    glUseProgram(program);

    // Generate vertex arrays for geometric shapes
	generateGrass(program, &grassData);
	generateHeart(program, &heartData);
    generateCube(program, &cubeData);
    generateSphere(program, &sphereData);
    generateCone(program, &coneData);
    generateCylinder(program, &cylData);

    uModelView  = glGetUniformLocation( program, "ModelView"  );
    uProjection = glGetUniformLocation( program, "Projection" );
    uView       = glGetUniformLocation( program, "View"       );

    glClearColor( 0.5, 0.5, 1.0, 1.0 ); // dark blue background

    uAmbient   = glGetUniformLocation( program, "AmbientProduct"  );
    uDiffuse   = glGetUniformLocation( program, "DiffuseProduct"  );
    uSpecular  = glGetUniformLocation( program, "SpecularProduct" );
    uLightPos  = glGetUniformLocation( program, "LightPosition"   );
    uShininess = glGetUniformLocation( program, "Shininess"       );
    uTex       = glGetUniformLocation( program, "Tex"             );
    uEnableTex = glGetUniformLocation( program, "EnableTex"       );

    glUniform4f(uAmbient,    0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uDiffuse,    0.6f,  0.6f,  0.6f, 1.0f);
    glUniform4f(uSpecular,   0.2f,  0.2f,  0.2f, 1.0f);
    glUniform4f(uLightPos,  15.0f, 15.0f, 30.0f, 0.0f);
    glUniform1f(uShininess, 100.0f);

    glEnable(GL_DEPTH_TEST);

    TgaImage coolImage;
    if (!coolImage.loadTGA("challenge.tga"))
    {
        printf("Error loading image file\n");
        exit(1);
    }

    GLuint texture;
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
    
    glTexImage2D(GL_TEXTURE_2D, 0, 4, coolImage.width, coolImage.height, 0,
                 (coolImage.byteCount == 3) ? GL_BGR : GL_BGRA,
                 GL_UNSIGNED_BYTE, coolImage.data );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, texture );
    
    // Set texture sampler variable to texture unit 0
    // (set in glActiveTexture(GL_TEXTURE0))
    glUniform1i( uTex, 0 );
    
    Arcball = new BallData ;
    Ball_Init(Arcball);
    Ball_Place(Arcball,qOne,0.75);
}

/*********************************************************
    PROC: set_colour();
    DOES: sets all material properties to the given colour
    -- don't change
**********************************************************/

void set_colour(float r, float g, float b)
{
    float ambient  = 0.2f;
    float diffuse  = 0.6f;
    float specular = 0.2f;
    glUniform4f(uAmbient,  ambient*r,  ambient*g,  ambient*b,  1.0f);
    glUniform4f(uDiffuse,  diffuse*r,  diffuse*g,  diffuse*b,  1.0f);
    glUniform4f(uSpecular, specular*r, specular*g, specular*b, 1.0f);
}

/*********************************************************
**********************************************************
**********************************************************

    PROC: display()
    DOES: this gets called by the event handler to draw
          the scene, so this is where you need to build
          your ROBOT --  
      
        MAKE YOUR CHANGES AND ADDITIONS HERE

    Add other procedures if you like.

**********************************************************
**********************************************************
**********************************************************/

void update(int val)
{
    glutTimerFunc(1000/60, update, 0);
	prologue += 0.025;
	if(rotation < 471) {
		rotation += 0.75;
		rotateX = cos(rotation/60);
		rotateZ = sin(rotation/60);
	}
	if(prologue > 10)
		timeline += 0.025;
	grassSway = 15*sin(5*prologue);
	leafSway = 30*sin(1+prologue);
	moveLegs = 15*sin(15*timeline);
	if(timeline > 12.0f && timeline < 20.0f)
		temp++;
	if((timeline > 13.5 && timeline < 13.525) ||
		(timeline > 18.0 && timeline < 18.025) ||
		(timeline > 19.975 && timeline < 20.0) ||
		(timeline > 24.0 && timeline < 24.025) ||
		(timeline > 27.0 && timeline < 27.025) ||
		(timeline > 48.0 && timeline < 49.0) ||
		(timeline > 54.0 && timeline < 54.25))
		temp = 0;
	if((timeline > 20.0f && timeline < 23.0f) ||
		(timeline > 24.0f && timeline < 27.0f) ||
		(timeline > 27.25 && timeline < 28.25))
		temp += 0.025;
	if((timeline > 52.6 && temp < 45) ||
		(timeline > 54.25 && temp < 45))
		temp++;
	if((timeline > 21.0f && temp2 < 45) ||
		(timeline > 24.0 && temp2 < 90) ||
		(timeline > 30.5 && temp2 < 230) ||
		(timeline > 40.0 && temp2 < 420) ||
		(timeline > 43.0 && temp2 < 540) ||
		(timeline > 47.0 && temp2 < 560) ||
		(timeline > 50.0))
		temp2++;
	if(timeline > 35.0 && temp2 < 330)
		temp2 += 0.25;
	if(timeline > 38.0f && temp2 < 330)
		temp2 += 0.75;
	if(timeline > 57.0)
		temp3 += 0.25;
	pathBeeHor -= 1;
	pathBeeVert = 0.75*sin(1+pathBeeHor/20);
    pathWings = 15*sin(1+pathBeeHor/20);
    pathLegs = 22.5*sin(-pathBeeHor/20);
	//std::cout << "Timeline: " << timeline << " temp: " << temp << std::endl;
    glutPostRedisplay();
}

void getFPS()
{
	static float fps = 0.0f;
	static float timePrev = 0.0f;
	float timeCurr = GetTickCount() * 0.001f;    
	fps++;
	if(timeCurr - timePrev > 1.0f )
	{
		timePrev = timeCurr;
		std::cout << "\b\b\b\b\b\b\b\b" << "FPS: " << fps;
		fps = 0;
	}
}

void display(void)
{
    // Clear the screen with the background colour (set in myinit)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    model_view = mat4(1.0f);
    model_view *= Translate(0.0f, 0.0f, -15.0f);
    HMatrix r;
    Ball_Value(Arcball,r);

    mat4 mat_arcball_rot(
        r[0][0], r[0][1], r[0][2], r[0][3],
        r[1][0], r[1][1], r[1][2], r[1][3],
        r[2][0], r[2][1], r[2][2], r[2][3],
        r[3][0], r[3][1], r[3][2], r[3][3]);
    model_view *= mat_arcball_rot;

    mat4 view = model_view;
    glUniformMatrix4fv( uView, 1, GL_TRUE, view );

    // Previously glScalef(Zoom, Zoom, Zoom);
    model_view *= Scale(Zoom);
	model_view *= Translate(0.0f, 0.0f, -10.0f);
	model_view *= RotateX(10);


	if(prologue < 20) {
		model_view *= Translate(0.0f, -20.0f+prologue, 0.0f);
		cam_eye = vec4(rotateX, 0.0, rotateZ, 1.0);
		model_view *= LookAt(cam_eye, cam_ref, cam_up);
	}
	
	if(timeline > 57)
		model_view *= Translate(0.0f, -temp3, -0.0f);

	// Draw Scene
	drawScene();

	// Draw Leaf
	if(timeline < 8.0f) {
		mvstack.push(model_view);
			model_view *= Translate(0.0f, 25.0f-prologue, 0.0f);
			model_view *= RotateZ(leafSway);
			model_view *= Translate(4.0f, -1.0f-prologue, 3.0f);
			drawLeaf();
		model_view = mvstack.pop();
	}

	// Draw Bee
	if(timeline > 35.0f && timeline < 38.0f) {
		mvstack.push(model_view);
			model_view *= Translate(15.0f-(temp2-230), pathBeeVert, 4.5f);
			model_view *= Scale(0.5f, 0.5f, 0.5f);
			drawBee();
		model_view = mvstack.pop();
	}

	// Draw Male
	mvstack.push(model_view);
		if(timeline > 20.0f && timeline < 24.0f) {
			model_view *= Translate(-5.0f+temp, 0.0f, 2.25f);
			prevX = -5.0+temp;
		}
		else if(timeline >= 24.0f && timeline < 27.0f)
			model_view *= Translate(prevX-temp, 0.0f, 2.25f);
		else if(timeline > 30.5 && timeline < 35.0) {
			model_view *= Translate(-25.0+temp2/10, -1.25f, 4.5f);
			prevX = -25.0+temp2/10;
			model_view *= RotateY(90);
		}
		else if(timeline >= 35.0f && timeline < 43.0) {
			model_view *= Translate(prevX-(temp2-230), -1.25f, 4.5f);
			model_view *= RotateY(-90);
		}
		else if(timeline >= 43 && timeline < 49.0) {// && -15+(temp2-420)/10 < -5.0) {
			if(-15+(temp2-420)/10 < -5.0f) {
				model_view *= Translate(-15+(temp2-420)/10, -1.25f, 4.5f);
				prevX = -15+(temp2-420)/10;
			}
			else
				model_view *= Translate(-5.0, -1.25f, 4.5f);
			model_view *= RotateY(90);
		}
		else
			model_view *= Translate(-5.0f, 0.0f, 2.25f);
		drawMale(0.6f, 0.6f, 1.0f);
	model_view = mvstack.pop();

	// Draw Female
	if(timeline > 8.0f) {
		mvstack.push(model_view);
			if(timeline < 12.0f) {
				model_view *= Translate(29.0f-2*timeline, -1.25f, 4.5f);
				model_view *= RotateY(-90);
			}

			if(timeline > 12.0f && timeline < 13.5) {
				model_view *= Translate(5.0f, -1.25f, 4.5f);
				model_view *= RotateY(-90+temp*1.25);
			}

			if(timeline > 13.5f && timeline < 40.0f || timeline > 53.75)
				model_view *= Translate(5.0f, 0.0f, 2.25f);

			if(timeline >= 40.0f && timeline < 50.0f) {
				model_view *= Translate(5.0f+(temp2-330)/10, -1.25f, 4.5f);
				model_view *= RotateY(90);
			}

			if(timeline > 50.0f && timeline < 53.75) {
				if(15.0f-(temp2-560)/10 > 5.0) {
					model_view *= Translate(15.0-(temp2-560)/10, -1.25f, 4.5f);
					prevY = 15.0-(temp2-560)/10;
				}
				else
					model_view *= Translate(prevY, -1.25f, 4.5f);

				if(timeline < 52.6f)
					model_view *= RotateY(-90);
				else
					model_view *= RotateY(-90+temp*2);
			}

			if(timeline < 49.0)
				drawFemale(1.0f, 0.0, 1.0f);
			else
				drawFemale(1.0f, 1.0f, 1.0f);

		model_view = mvstack.pop();
	}

	if(timeline > 59.0)
		exit(0);

	getFPS();

    glutSwapBuffers();
    if(Recording == 1)
        FrSaver.DumpPPM(Width, Height) ;
}

/**********************************************
    PROC: myReshape()
    DOES: handles the window being resized 
    
      -- don't change
**********************************************************/

void myReshape(int w, int h)
{
    Width = w;
    Height = h;

    glViewport(0, 0, w, h);

    mat4 projection = Perspective(50.0f, (float)w/(float)h, 1.0f, 1000.0f);
    glUniformMatrix4fv( uProjection, 1, GL_TRUE, projection );
}

void instructions() 
{
    printf("Press:\n");
    printf("  s to save the image\n");
    printf("  r to restore the original view.\n") ;
    printf("  0 to set it to the zero state.\n") ;
    printf("  a to toggle the animation.\n") ;
    printf("  m to toggle frame dumping.\n") ;
    printf("  q to quit.\n");
	printf("\n");
}

// start or end interaction
void myMouseCB(int button, int state, int x, int y)
{
    Button = button ;
    if( Button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        HVect arcball_coords;
        arcball_coords.x = 2.0*(float)x/(float)Width-1.0;
        arcball_coords.y = -2.0*(float)y/(float)Height+1.0;
        Ball_Mouse(Arcball, arcball_coords) ;
        Ball_Update(Arcball);
        Ball_BeginDrag(Arcball);

    }
    if( Button == GLUT_LEFT_BUTTON && state == GLUT_UP )
    {
        Ball_EndDrag(Arcball);
        Button = -1 ;
    }
    if( Button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN )
    {
        PrevY = y ;
    }


    // Tell the system to redraw the window
    glutPostRedisplay() ;
}

// interaction (mouse motion)
void myMotionCB(int x, int y)
{
    if( Button == GLUT_LEFT_BUTTON )
    {
        HVect arcball_coords;
        arcball_coords.x = 2.0*(float)x/(float)Width - 1.0 ;
        arcball_coords.y = -2.0*(float)y/(float)Height + 1.0 ;
        Ball_Mouse(Arcball,arcball_coords);
        Ball_Update(Arcball);
        glutPostRedisplay() ;
    }
    else if( Button == GLUT_RIGHT_BUTTON )
    {
        if( y - PrevY > 0 )
            Zoom  = Zoom * 1.03 ;
        else 
            Zoom  = Zoom * 0.97 ;
        PrevY = y ;
        glutPostRedisplay() ;
    }
}

void idleCB(void)
{
    if( Animate == 1 )
    {
        // TM.Reset() ; // commenting out this will make the time run from 0
        // leaving 'Time' counts the time interval between successive calls to idleCB
        if( Recording == 0 )
            TIME = TM.GetElapsedTime() ;
        else
            TIME += 0.033 ; // save at 30 frames per second.
        printf("TIME %f\n", TIME) ;
        glutPostRedisplay() ; 
    }
}
/*********************************************************
     PROC: main()
     DOES: calls initialization, then hands over control
           to the event handler, which calls 
           display() whenever the screen needs to be redrawn
**********************************************************/

int main(int argc, char** argv) 
{
    glutInit(&argc, argv);
    // If your code fails to run, uncommenting these lines may help.
    //glutInitContextVersion(3, 2);
    //glutInitContextProfile(GLUT_CORE_PROFILE);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition (0, 0);
    glutInitWindowSize(Width,Height);
    glutCreateWindow(argv[0]);
    printf("GL version %s\n", glGetString(GL_VERSION));
    glewExperimental = GL_TRUE;
    glewInit();
    
    myinit();

    glutIdleFunc(idleCB) ;
    glutReshapeFunc (myReshape);
    glutKeyboardFunc( myKey );
    glutMouseFunc(myMouseCB) ;
    glutMotionFunc(myMotionCB) ;
    instructions();

    glutDisplayFunc(display);
	glutTimerFunc(1000/30, update, 0);
    glutMainLoop();

    TM.Reset() ;
    return 0;         // never reached
}




