#pragma once


//GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_inverse.hpp>

// Glew (include it before GL.h):
#include <GL/glew.h>

//Freeglut
#include <GL/freeglut.h>
#include <GL/glut.h>
#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

//freeimage
#include "FreeImage.h"

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
#include <cstdio>
#include <sys/stat.h>
#include <stdexcept>

#ifdef WIN32
#define snprintf sprintf_s
#endif

// Debug info
//#define PRINT_DEBUG_MESH_INFO //Uncomment to print vertex, faces, and textcoords info for Mesh objects during the scene_loader