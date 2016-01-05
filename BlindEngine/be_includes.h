#pragma once

#define GLM_FORCE_RADIANS

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Freeglut
#include <GL/freeglut.h>
#include <GL/glut.h>

// Assimp:
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <version.h>

//system libs
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

// Debug info
//#define PRINT_DEBUG_MESH_INFO //Uncomment to print vertex, faces, and textcoords info for Mesh objects during the scene_loader
