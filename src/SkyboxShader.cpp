#include "SkyboxShader.h"
#include "Vertex.h"
#include "Texture.h"

SkyBoxShader* SkyBoxShader::shader = nullptr;

SkyBoxShader* SkyBoxShader::GetInstance()
{
	if (shader == nullptr)
		shader = new SkyBoxShader();
	return shader;
}

VertexOut SkyBoxShader::VertexShader(const Vertex& v)
{
	VertexOut o;
	glm::mat4 rotView = glm::mat3(uniform->viewMatrix);
	o.worldPos = v.position;
	glm::vec4 clipPos = uniform->projectorMatrix * rotView * v.position;
	o.windowPos = glm::vec4(clipPos.x, clipPos.y, clipPos.w, clipPos.w);
	o.Z = 1;
	return o;
}

glm::vec4 SkyBoxShader::FragmentShader(const VertexOut& f)
{
	glm::vec3 uv = glm::normalize(f.worldPos);
	glm::vec4 color = uniform->cubemap->SampleCUBE(uv);
	return color;
}
