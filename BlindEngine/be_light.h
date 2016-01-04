#pragma once

#include <stdexcept>
//BlindEngine
#include "be_node.h"

#include "be_includes.h"
class BELight : public BEnode
{
private:
	//type of lights
	typedef enum lightType
	{
		DIRECTIONAL = 0,
		OMNIDIRECTIONAL,
		SPOTLIGHT
	}LightType;

	//static counter for the number of instantiated lights
	static int total_lights;

	//global constructor
	BELight(const LightType type, const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 position, glm::vec3 direction, float cutoff);

	//the type of the instantiated light
	LightType type_;

	//light index (from GL_LIGHT0 to GL_LIGHT7)
	int light_number_;

	// The light properties
	glm::vec4 ambient_, diffuse_, specular_, position_;
	glm::vec3 direction_;
	float cutoff_;
public:
	//factory methods used to create lights
	static BELight* CreateOmnidirectionalLight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 position);
	static BELight* CreateDirectionalLight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 direction);
	static BELight* CreateSpotLight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 position, glm::vec3 direction, float cutoff);

	//destructor
	~BELight();

	//inherited render method
	virtual void Render() override;
};