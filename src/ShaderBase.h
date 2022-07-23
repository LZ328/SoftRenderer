#ifndef SHADER_H
#define SHADER_H

#include "Global.h"

class Vertex;
class VertexOut;
class StateMachine;
class Texture2D;
class Texture3D;
class DirectionLight;

// Shader 要用到的参数和资源
class Uniform {
public:

	glm::mat4 modelMatrix;
	glm::mat4 viewMatrix;
	glm::mat4 projectorMatrix;
	glm::mat3 normalMatrix;
	glm::mat4 MVP;
	glm::vec4 cameraPos;
	
	glm::vec4 color;
	glm::vec4 specular;
	glm::vec4 ambient;
	float gloss;

	DirectionLight* dirLight;

	Texture2D* mainTex;

	Texture3D* cubemap;
	
	Uniform();
	Uniform(
		const glm::mat4& m,
		const glm::mat4& v,
		const glm::mat4& p
		);
	~Uniform();
};

// Shader 基类
class Shader {

private:
	static Shader* shader;
	

protected:
	Uniform* uniform;
	Shader() : uniform(nullptr){}
	Shader(const Shader& s) = delete;

public:
	virtual ~Shader() {}
	static Shader* GetInstance();
	void SetUniform(Uniform* u) { uniform = u; }

	virtual void Destroy() {
		uniform = nullptr;
		if (shader)
			delete shader;
		shader = nullptr;
	}
	virtual VertexOut VertexShader(const Vertex& v);
	virtual glm::vec4 FragmentShader(const VertexOut& f);

};

#endif