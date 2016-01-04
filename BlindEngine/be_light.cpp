#include "be_light.h"

//initialization of the counter
int BElight::total_lights = 0;

//global constructor
BElight::BElight(const LightType type, const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 position, glm::vec3 direction, float cutoff) : BEnode(name)
{
	if (total_lights <= 7)
		light_number_ = (total_lights++) + GL_LIGHT0;
	else
		throw new std::runtime_error("There are too many lights in the scene!");

	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;
	type_ = type;
	cutoff_ = cutoff;

	switch (type)
	{
	case DIRECTIONAL:
		//slide 37 of cg_opengl.pdf
		position_ = glm::vec4(direction, 0.f);  //the direction is meant to be hardcoded in the position with w=0
		break;

	case OMNIDIRECTIONAL:
		//slide 38 of cg_opengl.pdf
		position_ = glm::vec4(position, 1.f);
		break;

	case SPOTLIGHT:
		//slide 39 of cg_opengl.pdf
		position_ = glm::vec4(position, 1.f);;
		break;
	}
}

BElight* BElight::CreateOmnidirectionalLight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 position)
{
	return new BElight(OMNIDIRECTIONAL, name, ambient, diffuse, specular, position, glm::vec3(), 0);
}

BElight* BElight::CreateDirectionalLight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 direction)
{
	return new BElight(DIRECTIONAL, name, ambient, diffuse, specular, glm::vec3(), direction, 180.f);
}

BElight* BElight::CreateSpotLight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec3 position, glm::vec3 direction, float cutoff)
{
	return new BElight(SPOTLIGHT, name, ambient, diffuse, specular, position, direction, cutoff);
}

void BElight::Render()
{

	//Common color property
	glLightfv(light_number_, GL_AMBIENT, glm::value_ptr(ambient_));
	glLightfv(light_number_, GL_DIFFUSE, glm::value_ptr(diffuse_));
	glLightfv(light_number_, GL_SPECULAR, glm::value_ptr(specular_));

	//if the current light is a directional light, then direction is passed instead
	glLightfv(light_number_, GL_POSITION, glm::value_ptr(position_));

	//light number goes from GL_LIGHT0 to GL_LIGHT7
	if (type_ == SPOTLIGHT)
	{
		glLightfv(light_number_, GL_SPOT_CUTOFF, &cutoff_);
		glLightfv(light_number_, GL_SPOT_DIRECTION, glm::value_ptr(direction_));
	}
}

BElight::~BElight()
{
}
