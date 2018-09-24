/*
 * Brent Crane
 * Started September 21st, 2018
 * A field of points, thought it might look cool
 * Might try to expand it to react to music, somehow. That could be fun.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <GL/glut.h>


// Type definitions

typedef struct Points {
  float x;
  float y;
  float z;
  float theta;
} Point;

// Function prototypes

void updatePoints();
void drawPoints();
void allocatePoints();
void positionPoints();

// Global variables

int i;
// How wide and long the point field should be
int fieldSize = 100;
// How many points need to be allocated
int pointCount;

// Pointer to beginning of the points array
Point *points;

// Initial height value of a given row of points
float theta = 0.0f;
float thetaIncrement = 0.5f;
float thetaMovementSpeed = 0.01f;

// Variable to allow for custom playback speeds, so it doesnt go as fast as your PC can go
int currentTick = 0;
int tickDelay = 1;
int readyToTick = 1;

// Camera position variables, global because they are modified and used independantly
float camEyeX = 0;
float camEyeY = 3;
float camEyeZ = 10;
float camAtX = 0;
float camAtY = 0;
float camAtZ = 0;
float camUpX = 0;
float camUpY = 1;
float camUpZ = 0;


// Make all animation changes that need to happen each frame
static void animationTick()
{
  // Clear the redering window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Clear the current matrix
  glLoadIdentity();

  // Position the camera
  gluLookAt(camEyeX, camEyeY, camEyeZ, camAtX, camAtY, camAtZ, camUpX, camUpY, camUpZ);

  // Update the point field
  updatePoints();
  // Draw the point field
  drawPoints();

  // Flush and swap the buffers
  glFlush();
  glutSwapBuffers();

  // Request a re-draw for animation purposes
	glutPostRedisplay();
}


// Initialize OpenGL's rendering modes, and the display lists
void initialize()
{
  pointCount = fieldSize * fieldSize;

  // Read all of the control points from the input file into memory
  allocatePoints();

  // Set up other GL features
  glShadeModel( GL_SMOOTH );
  glClearColor( 0.0, 0.0, 0.0, 0.0 );
  glClearDepth( 1.0 );
  glEnable( GL_DEPTH_TEST );
}

void updatePoints()
{
  for (i = 0; i < pointCount; i++)
  {
    points[i].theta += thetaMovementSpeed;
    points[i].y = sin(points[i].theta);
  }
}

// Drawing the control points of the spline
void drawPoints ()
{
  glColor3f( 1,0,0 );
  glPointSize( 5 );
  glBegin( GL_POINTS );
    glColor3f(0,0.3f,0.7f);
    for ( i = 0 ; i < pointCount ; i++ )
      glVertex3f( points[i].x, points[i].y, points[i].z );
  glEnd();
}


void allocatePoints()
{
  // Allocate memory for the points
  points = malloc( pointCount*sizeof( Point ) );
  positionPoints();
  // Seed the random number generator
  srand( time( NULL ) );
}

void positionPoints()
{
  int rowIncrementer = -(fieldSize/2);
  int colIncrementer = -(fieldSize/2);
  float pointSeparation = 0.5f;


  for (i = 0; i < pointCount; i++)
  {
    if (rowIncrementer > fieldSize/2)
    {
      rowIncrementer = -(fieldSize/2);
      colIncrementer++;
      //theta += thetaIncrement;
    }
    else
    {
      rowIncrementer++;
      theta += thetaIncrement;
    }

    points[i].theta = theta;
    points[i].x = (float) rowIncrementer * pointSeparation;
    points[i].y = sin(points[i].theta);
    points[i].z = (float) colIncrementer * pointSeparation;

    //printf("X: %f\tY: %f\tZ: %f\n", points[i].x, points[i].y, points[i].z);
    //printf("fieldSize: %d\n", fieldSize);
    //printf("Row: %d\tCol: %d\n", rowIncrementer, colIncrementer);
    //printf("Theta: %f\n", theta);
  }
}


// Keyboard callback to handle normal key input
static void keyPressFunc( unsigned char key, int x, int y )
{
	switch ( key ) {
    case 'e':

      break;
    case 'q':

      break;
    case 's':

      break;
    case 'w':

      break;
    // If Escape is pressed, quit the program
  	case 27:
  		exit(1);
    case 'a':

      break;
    case 'd':

      break;
    case ' ':

      break;
	}
}


// Keyboard callback to handle special keys
static void specialKeyFunc( int key, int x, int y )
{
	switch ( key ) {
    // Up arrow
  	case GLUT_KEY_UP:
      camEyeY += 0.5f;
  		break;
    // Down arrow
  	case GLUT_KEY_DOWN:
      camEyeY -= 0.5f;
    	break;
    case GLUT_KEY_LEFT:
      thetaMovementSpeed -= 0.01f;
      break;
    case GLUT_KEY_RIGHT:
      thetaMovementSpeed += 0.01f;
      break;
	}
}


// Timer callback that is called every 10 milliseconds
static void timerFunc( int value )
{
  // Determine whether the scene should tick, to control run speed
  if (currentTick < tickDelay)
    currentTick++;
  else
  {
    readyToTick = 1;
    currentTick = 0;
  }

  // Set up the next tick of the timer callback
  glutTimerFunc(10, timerFunc, 0);
}


// resizeFunc is called whenever the window is resized
static void resizeFunc( int w, int h )
{
  // Set values of the aspect ratio
  float aspectRatio;
	glViewport( 0, 0, w, h );
	aspectRatio = (float)w / (float)h;

	// Set up the projection view matrix
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  gluPerspective( 60.0, aspectRatio, 0.5, 300.0 );

	// Select the Modelview matrix
  glMatrixMode( GL_MODELVIEW );
}



// Main routine
// Set up OpenGL, set up callbacks, and get the main loop going
int main( int argc, char** argv )
{

	// Need to double buffer for animation and movement stuff
	glutInit( &argc,argv );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

	// Create and position the graphics window
  glutInitWindowPosition( 0, 0 );
  glutInitWindowSize( 600, 360 );
  glutCreateWindow( "Point Field" );

	// Initialize all things that need to be initialized
  initialize();


	// Set up callback functions for key presses
	glutKeyboardFunc( keyPressFunc );
	glutSpecialFunc( specialKeyFunc );

  // Set up callback for logical ticks (the timer function)
  glutTimerFunc(0, timerFunc, 0);

	// Set up the reshape callback function
  glutReshapeFunc( resizeFunc );

	// Set up the display callback function
  glutDisplayFunc( animationTick );

	// Start the main loop
	glutMainLoop();

  // Required but unused return statement, will never be reached
  return(0);
}
