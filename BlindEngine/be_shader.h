#pragma once

#include "be_object.h"

/////////////
// #DEFINE //
/////////////

// Commodity macro:
#define SHADER_STRINGIFY(x) #x

class BEshader : public BEobject
{
	//////////
public: //
	//////////

	// Constants:
	static const unsigned int MAX_LOGSIZE = 1024;  ///< Max output size in char for a shader log

	// Enums:
	enum ///< Kind of shader
	{
		TYPE_UNDEFINED = 0,
		TYPE_VERTEX,
		TYPE_FRAGMENT,
		TYPE_PROGRAM,
		TYPE_LAST
	};

	// Const/dest:	 
	BEshader();
	~BEshader();

	virtual void Render(glm::mat4 world_matrix) override;

	// Get/set:
	int getParamLocation(char *name);
	inline void setMatrix(int param, glm::mat4 &mat)
	{
		glUniformMatrix4fv(param, 1, GL_FALSE, glm::value_ptr(mat));
	}

	inline void setMatrix(int param, glm::mat3 &mat){
		glUniformMatrix3fv(param, 1, GL_FALSE, glm::value_ptr(mat));
	}

	inline void setFloat(int param, float value)
	{
		glUniform1f(param, value);
	}
	inline void setInt(int param, int value)
	{
		glUniform1i(param, value);
	}
	inline void setVec3(int param, glm::vec3 &vect)
	{
		glUniform3fv(param, 1, glm::value_ptr(vect));
	}
	inline void setVec4(int param, glm::vec4 &vect)
	{
		glUniform4fv(param, 1, glm::value_ptr(vect));
	}

	// Accessing data:
	bool loadFromMemory(int kind, char *data);
	bool loadFromFile(int kind, char *filename);
	bool build(BEshader *vertexShader, BEshader *fragmentShader);

	inline void bind(int location, char *attribName)
	{
		glBindAttribLocation(glId, location, attribName);
	}
	void initParam();
	// Shader variables
	int projLoc;
	int mvLoc;
	int normalMatLoc;

	int matAmbientLoc;
	int matDiffuseLoc;
	int matSpecularLoc;
	int matShininessLoc;

	int lightPositionLoc;
	int lightAmbientLoc;
	int lightDiffuseLoc;
	int lightSpecularLoc;


	///////////	 
private: //
	///////////					

	// Object kind:
	int type;

	// OGL id:
	GLuint glId;
};