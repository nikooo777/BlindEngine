#include "be_light.h"


//constructor for directional light
BELight::BELight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 direction_position, bool omnidirectional) : BEnode(name)
{
	
	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;
	if (omnidirectional)
		position_ = direction_position;
	else
		direction_ = direction_position;

}

//constructor for spotlight light
BELight::BELight(const std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, glm::vec4 position, float cutoff) : BEnode(name)
{
	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;
	position_ = position;

}


void BELight::Render()
{

}

BELight::~BELight()
{
}
