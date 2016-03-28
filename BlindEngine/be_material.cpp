#include "be_material.h"
#include "be_engine.h"

BEmaterial* BEmaterial::mat_shadow = new BEmaterial("shadow_mat", glm::vec4(.0f), glm::vec4(.0f), glm::vec4(.0f), .0f, .0f, .0f, nullptr);

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
		//OLD_gl //glEnable(GL_TEXTURE_2D);
		texture_->Render(glm::mat4());
	}
	else;
		//OLD_gl //glDisable(GL_TEXTURE_2D);

	std::cout << "Rendering material: " << get_name() << std::endl;
	//OLD_gl //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, glm::value_ptr(ambient_));
	//OLD_gl //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, glm::value_ptr(diffuse_));
	//OLD_gl //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, glm::value_ptr(specular_));
	//OLD_gl //glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess_);
	BEshader* shader = BEengine::GetInstance()->get_shader();
	shader->setVec3(shader->matAmbientLoc, glm::vec3(ambient_));
	shader->setVec3(shader->matDiffuseLoc, glm::vec3(diffuse_));
	shader->setVec3(shader->matSpecularLoc, glm::vec3(specular_));
	shader->setFloat(shader->matShininessLoc, shininess_);
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