#include "MaterialBase.h"
#include "ShadowShader.h"
#include "BlinnShader.h"
#include "Texture.h"

Material::Material() :
	shaders(2, nullptr),
	color(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)),
	specular(glm::vec4(1.0f)),
	gloss(8.0f),
	mainTex(nullptr)
{
	shaders[0] = ShadowShader::GetInstance();
	shaders[1] = BlinnShader::GetInstance();
}

Material::~Material() {}

Shader* Material::GetShader(int pass)
{
	return shaders[pass];
}


void Material::SetShader(Shader* shader, int pass)
{
	shaders[pass] = shader;
}

void Material::SetTexture(Texture2D* t)
{
	mainTex = std::make_shared<Texture2D>(*t);
}

void Material::SetupUniform(Uniform& u)
{
	u.color = color;
	u.specular = specular;
	u.gloss = gloss;
	u.mainTex = mainTex.get();
}