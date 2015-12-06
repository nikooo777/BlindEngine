/**
 * @file		main.cpp
 * @brief	Series 5, ex. 2
 *
 * @author	Achille Peternier (C) SUPSI [achille.peternier@supsi.ch]
 */



//////////////
// #INCLUDE //
//////////////

   // GLM:
   #define  GLM_FORCE_RADIANS
   #include <glm/glm.hpp>
   #include <glm/gtc/matrix_transform.hpp>
   #include <glm/gtc/type_ptr.hpp>

   // FreeGLUT:
   #include <GL/freeglut.h>

   // C/C++:
   #include <iostream>
   #include <stdio.h>



/////////////
// GLOBALS //
/////////////

   // Flags:
   bool wireframe = false;
   bool flat = false;

   // Context params:
   int windowId;

   // Projection matrices:
   glm::mat4 perspective;
   glm::mat4 ortho;

   // Material properties:
   glm::vec4 materialAmbient(0.5f, 0.5f, 0.5f, 1.0f);
   glm::vec4 materialDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
   glm::vec4 materialSpecular(1.0f, 1.0f, 1.0f, 1.0f);
   float materialShininess = 128.0f;

   // Light properties:
   glm::vec3 lightPosition(0.0f, -3.0f, -30.0f);
   glm::vec4 lightAmbient(0.5f, 0.5f, 0.5f, 1.0f);
   glm::vec4 lightDiffuse(0.5f, 0.5f, 0.5f, 1.0f);
   glm::vec4 lightSpecular(1.0f, 1.0f, 1.0f, 1.0f);
   glm::vec3 lightDirection(0.0f, 0.0f, 0.0f);
   float lightCutoff = 180.0f;

   // FPS:
   int fps = 0;
   int frames = 0;

   // Level of detail:
   int detail = 1;



/////////////
// METHODS //
/////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Renders a square grid on plane XZ made of triangles.
 * @param size size of the grid
 * @param tesselation number of triangles to use on each direction
 */
void drawGrid(float size, int tesselation)
{
   // Compute starting coordinates and step size:
   float startX = -size/2.0f;
   float startZ = size/2.0f;
   float triangleSize = size / (float) tesselation;

   // Normal is just one, set it now:
   glNormal3f(0.0f, 1.0f, 0.0f);

   // Go over XZ and draw triangles:
   for (int curZ = 0; curZ<tesselation; curZ++)
   {
      for (int curX = 0; curX<tesselation; curX++)
      {
         glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(startX, 0.0f, startZ);
            glVertex3f(startX+triangleSize, 0.0f, startZ);
            glVertex3f(startX, 0.0f, startZ-triangleSize);
            glVertex3f(startX+triangleSize, 0.0f, startZ-triangleSize);
         glEnd();

         startX += triangleSize;
      }
      startX = -size/2.0f;
      startZ -= triangleSize;
   }
}



///////////////
// CALLBACKS //
///////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This is the main rendering routine automatically invoked by FreeGLUT.
 */
void displayCallback()
{
   // Clear the screen:
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   ////////////////
   // 3D rendering:

   // Set perpsective matrix:
   glMatrixMode(GL_PROJECTION);
      glLoadMatrixf(glm::value_ptr(perspective));
   glMatrixMode(GL_MODELVIEW);

   // Set light position:
   glm::mat4 transLight = glm::translate(glm::mat4(), lightPosition);
   glLoadMatrixf(glm::value_ptr(transLight));

   // Draw a (unlit) cube to show light position:
   glDisable(GL_LIGHTING);
      glColor3fv(glm::value_ptr(lightDiffuse));
      glutSolidSphere(0.5f, 8, 8);
   glEnable(GL_LIGHTING);

   // Light position is set to object coordinates and is modified by the current OpenGL matrix (as with any other object):
   glm::vec4 objectCoordPosition(0.0f, 0.0f, 0.0f, 1.0f);
   glLightfv(GL_LIGHT0, GL_POSITION, glm::value_ptr(objectCoordPosition));
   glLightfv(GL_LIGHT0, GL_AMBIENT, glm::value_ptr(lightAmbient));
   glLightfv(GL_LIGHT0, GL_DIFFUSE, glm::value_ptr(lightDiffuse));
   glLightfv(GL_LIGHT0, GL_SPECULAR, glm::value_ptr(lightSpecular));

   // Set material properties:
   glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialShininess);
   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(materialAmbient));
   glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(materialDiffuse));
   glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(materialSpecular));

   // Position and render the grid:
   glm::mat4 transGrid = glm::translate(glm::mat4(), glm::vec3(0.0f, -10.0f, -50.0f));
   glLoadMatrixf(glm::value_ptr(transGrid));
   drawGrid(75.0f, (int) pow(8, detail));

   // Position and render the sphere:
   glm::mat4 transSphere = glm::translate(glm::mat4(), glm::vec3(-20.0f, 0.0f, -60.0f));
   glLoadMatrixf(glm::value_ptr(transSphere));
   glutSolidSphere(6.0f, 150, (int) pow(8, detail));

   // Position and render the teapot:
   glm::mat4 transTeapot = glm::translate(glm::mat4(), glm::vec3(0.0f, -5.0f, -50.0f));
   glLoadMatrixf(glm::value_ptr(transTeapot));
   glutSolidTeapot(6.0f);

   // Position and render the donut:
   glm::mat4 transTorus = glm::translate(glm::mat4(), glm::vec3(12.0f, -3.0f, -30.0f));
   glLoadMatrixf(glm::value_ptr(transTorus));
   glutSolidTorus(1.0f, 3.0f, (int) pow(8, detail), (int) pow(8, detail));


   //////////////////////////
   // Switch to 2D rendering:

   // Set orthographic projection:
   glMatrixMode(GL_PROJECTION);
      glLoadMatrixf(glm::value_ptr(ortho));
   glMatrixMode(GL_MODELVIEW);
   glLoadMatrixf(glm::value_ptr(glm::mat4(1.0)));

   // Disable lighting before rendering 2D text:
   glDisable(GL_LIGHTING);

   // Write some text:
   char text[64];
   sprintf(text, "FPS: %d", fps);
   glColor3f(1.0f, 1.0f, 1.0f);
   glRasterPos2f(1.0f, 2.0f);
   glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *) text);

   sprintf(text, "LOD: %d", detail);
   glRasterPos2f(1.0f, 14.0f);
   glutBitmapString(GLUT_BITMAP_8_BY_13, (unsigned char *) text);

   // Reactivate lighting:
   glEnable(GL_LIGHTING);

   // Swap this context's buffer:
   frames++;
   glutSwapBuffers();

   // Force rendering refresh:
   glutPostWindowRedisplay(windowId);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This callback is invoked each time the window gets resized (and once also when created).
 * @param width new window width
 * @param height new window height
 */
void reshapeCallback(int width, int height)
{
   // Update viewport size:
   glViewport(0, 0, width, height);

   // Refresh projection matrices:
   perspective = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 1.0f, 100.0f);
   ortho = glm::ortho(0.0f, (float) width, 0.0f, (float) height, -1.0f, 1.0f);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This callback is invoked each time a standard keyboard key is pressed.
 * @param key key pressed id
 * @param mouseX mouse X coordinate
 * @param mouseY mouse Y coordinate
 */
void keyboardCallback(unsigned char key, int mouseX, int mouseY)
{
   switch (key)
   {
      case 'r':
         materialDiffuse.x = (rand()%100) / 100.0f;
         materialDiffuse.y = (rand()%100) / 100.0f;
         materialDiffuse.z = (rand()%100) / 100.0f;
         materialAmbient = materialDiffuse;
         materialSpecular = materialDiffuse;
         materialShininess = (float) (rand()%128);
         break;

      case 'w':
         wireframe = !wireframe;
         if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
         break;

      case 'm':
         if (detail < 3)
            detail++;
         break;

      case 'l':
         if (detail > 1)
            detail--;
         break;

      case 'f':
         flat = !flat;
         if (flat)
            glShadeModel(GL_FLAT);
         else
            glShadeModel(GL_SMOOTH);
   }

   // Force rendering refresh:
   glutPostWindowRedisplay(windowId);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This callback is invoked each time a special keyboard key is pressed.
 * @param key key pressed id
 * @param mouseX mouse X coordinate
 * @param mouseY mouse Y coordinate
 */
void specialCallback(int key, int mouseX, int mouseY)
{
   // Change box rotation:
   const float speed = 1.0f;
   switch (key)
   {
      case GLUT_KEY_F1:
      case GLUT_KEY_F2:
      case GLUT_KEY_F3:
      case GLUT_KEY_F4:
      case GLUT_KEY_F5:
      case GLUT_KEY_F6:
      case GLUT_KEY_F7:
      case GLUT_KEY_F8:
      case GLUT_KEY_F9:
      case GLUT_KEY_F10:
         lightDiffuse.x = ((float) (key - GLUT_KEY_F1)) / 10.0f;
         lightDiffuse.y = lightDiffuse.x;
         lightDiffuse.z = lightDiffuse.x;
         lightAmbient = lightDiffuse;
         lightSpecular = lightDiffuse;
         break;

       case GLUT_KEY_UP:
         lightPosition.z -= speed;
         break;

      case GLUT_KEY_DOWN:
         lightPosition.z += speed;
         break;

      case GLUT_KEY_LEFT:
         lightPosition.x -= speed;
         break;

      case GLUT_KEY_RIGHT:
         lightPosition.x += speed;
         break;

      case GLUT_KEY_PAGE_UP:
         lightPosition.y += speed;
         break;

      case GLUT_KEY_PAGE_DOWN:
         lightPosition.y -= speed;
         break;
   }

   // Force rendering refresh:
   glutPostWindowRedisplay(windowId);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * This callback is invoked each 3 seconds.
 * @param value passepartout value
 */
void timerCallback(int value)
{
   // Update values:
   fps = frames;
   frames = 0;

   // Register the next update:
   glutTimerFunc(1000, timerCallback, 0);
}



//////////
// MAIN //
//////////

/**
 * Application entry point.
 * @param argc number of command-line arguments passed
 * @param argv array containing up to argc passed arguments
 * @return error code (0 on success, error code otherwise)
 */
int main(int argc, char *argv[])
{
   // Credits:
   std::cout << "3D scene lighting - A. Peternier (C) SUPSI" << std::endl;
   std::cout << std::endl;

   // Init context:
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowPosition(100, 100);

   // FreeGLUT can parse command-line params, in case:
   glutInit(&argc, argv);

   // Set some optional flags:
   glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

   // Create the window with a specific title:
   windowId = glutCreateWindow("3D scene lighting");

   // Set callback functions:
   glutDisplayFunc(displayCallback);
   glutReshapeFunc(reshapeCallback);
   glutKeyboardFunc(keyboardCallback);
   glutSpecialFunc(specialCallback);
   glutTimerFunc(1000, timerCallback, 0);

   // Global OpenGL settings:
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   // Enter the main FreeGLUT processing loop:
   glutMainLoop();

   // Done:
   return 0;
}
