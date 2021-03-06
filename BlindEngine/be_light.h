#pragma once

// DLL
#include "be_dll_interface.h"

//BlindEngine
#include "be_node.h"
#include "be_includes.h"

class LIB_API BElight : public BEnode
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
	BElight(const LightType type, const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 position, glm::vec3 direction, float cutoff);

	//the type of the instantiated light
	LightType type_;

	//light index (from GL_LIGHT0 to GL_LIGHT7)
	int light_number_;

	// The light properties
	glm::vec4 ambient_, diffuse_, specular_, position_;
	glm::vec3 direction_;
	float cutoff_, angle_inner_cone_, angle_outer_cone_, attenuation_constant_, attenuation_linear_, attenuation_quadratic_;
	bool is_active_ = true;
public:
	//factory methods used to create lights
	static BElight* CreateOmnidirectionalLight(const std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position);
	static BElight* CreateDirectionalLight(const std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);
	static BElight* CreateSpotLight(const std::string name, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 position, glm::vec3 direction, float cutoff);

	//Setters
	void SetAngleInnerCone(float);
	void SetAngleOuterCone(float);
	void SetAttenuationConstant(float);
	void SetAttenuationLinear(float);
	void SetAttenuationQuadratic(float);

	void SetAmbient(glm::vec3);
	void SetDiffuse(glm::vec3);
	void SetSpecular(glm::vec3);
	void SetDirection(glm::vec3);
	void SetPosition(glm::vec4);

	void ToggleLight(){ is_active_ = !is_active_; }

	//destructor
	~BElight();

	//inherited render method
	//virtual void Render(glm::mat4) override;
	virtual void Render(glm::mat4 world_matrix) override;
	virtual void CalcTransformation(glm::mat4 world_matrix) override;

	//Utility
	virtual BEnode* Find(std::string) override;
	virtual BEnode* Find(long) override;
};