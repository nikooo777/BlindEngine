#include "be_material.h"


BEmaterial::BEmaterial(std::string name, glm::vec4 ambient, glm::vec4 diffuse, glm::vec4 specular, float transparency, float shininess, float shininess_strength, BEtexture* texture) : BEobject(name)
{
	ambient_ = ambient;
	diffuse_ = diffuse;
	specular_ = specular;
	shininess_ = shininess;
	shininess_strength_ = shininess_strength;
	texture_ = texture;
	transparency_ = transparency;
	SetTransparency(transparency);
}


BEmaterial::~BEmaterial()
{

}

void BEmaterial::Render(glm::mat4 f)
{
	if (texture_ != nullptr)
	{
		//std::cout << "A texture was found" << std::endl;
		glEnable(GL_TEXTURE_2D);
		texture_->Render(glm::mat4());
	}
	else
		glDisable(GL_TEXTURE_2D);

	//std::cout << "Rendering material: " << get_name() << std::endl;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient_));
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse_));
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular_));
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, powf(2.0f, shininess_));
}

bool BEmaterial::IsTransparent()
{
	float precision = 0.00001f;
	return !FloatCompare(transparency_, 1.0f);
}

void BEmaterial::SetTransparency(float alpha)
{
	//bug in assimp setting alpha to 0
	//since 0 wouldn't make sense (completely transparent) we will revert it to visible
	if (FloatCompare(alpha, 0.0f))
		alpha = 1.0f;

	ambient_.a = alpha;
	diffuse_.a = alpha;
	specular_.a = alpha;
	transparency_ = alpha;
}

bool FloatCompare(float f1, float f2, float precision)
{
	return (((f1 - precision) < f2) && ((f1 + precision) > f2));
}