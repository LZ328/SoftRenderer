#ifndef MATERIAL_H
#define MATERIAL_H

#include "Global.h"

class Shader;
class Texture2D;
class Texture3D;
class Uniform;

class Material {

protected:
	std::vector<Shader*> shaders;

public:
	// ��ɫ
	glm::vec4 color;
	// �߹���ɫ
	glm::vec4 specular;
	// gloss ϵ��
	float gloss;
	// ������
	std::shared_ptr<Texture2D> mainTex;

	Material();
	virtual ~Material();

	Shader* GetShader(int pass);
	void SetShader(Shader* shader, int pass);
	void SetTexture(Texture2D* t);
	void SetupUniform(Uniform& u);
};

#endif