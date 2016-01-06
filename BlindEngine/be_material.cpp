#include "be_material.h"


BEmaterial::BEmaterial(std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float shininess, float shininess_strength, BEtexture* texture) : BEobject(name)
{
	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;
	shininess_ = shininess;
	shininess_strength_ = shininess_strength;
	texture_ = texture;
}


BEmaterial::~BEmaterial()
{

}

void BEmaterial::Render(glm::mat4 f)
{
	//if the material has a texture, then render it
	//std::cout << "Rendering a material"<< get_name() << std::endl;

	if (texture_ != nullptr)
	{
		//std::cout << "A texture was found" << std::endl;
		texture_->Render(glm::mat4());
	}

	//std::cout << "Rendering material: " << get_name() << std::endl;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient_));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse_));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular_));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, powf(2.0f, shininess_));

}