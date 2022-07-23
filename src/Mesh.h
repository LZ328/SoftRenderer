#ifndef MESH_H
#define MESH_H

#include "Global.h"
#include "Vertex.h"

class Material;

class Mesh
{

public:
	// 顶点缓冲对象
	std::vector<Vertex> VBO;
	// 顶点索引对象
	std::vector<unsigned int> EBO;
	// Mesh材质
	Material* material;
	std::string name;


	Mesh();
	Mesh(int vNum, int iNum);
	~Mesh();

	Mesh(const Mesh& mesh);
	Mesh& operator=(const Mesh& mesh);
	void AddMesh(const Mesh& mesh);
	void AddTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

	// 方便创建基础模型，定义了两个静态函数
	static Mesh* CreatePlane(const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal);
	static Mesh* CreateBox(const glm::vec3& center, float radius);
};

#endif
