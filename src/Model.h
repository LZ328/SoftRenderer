#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

class Material;

class Model {

public:
	// 位置
	glm::vec4 position;
	// 旋转
	float pitch;
	float yaw;
	float roll;
	// 缩放
	float scale;
	// 由 Mesh 组成
	std::vector<Mesh*> meshes;
	std::string name;

	Model(Mesh* m);
	Model(const std::string& filename);
	~Model();

	glm::mat4 GetModelMatrix() const;
	void LoadObj(const std::string& filename);
	void AddMesh(Mesh* m);
	void RemoveMesh(int num);
	
	// 方便创建立方体，定义了两个静态函数
	static Model* CreateFloor(const glm::vec3& center, float radius);
	static Model* CreateSkyBox();

private:
	void BuildMesh(
		const std::vector<glm::vec3>& vertex,
		const std::vector<glm::vec3>& normal,
		const std::vector<glm::vec2>& texcoord,
		const std::vector<glm::vec4>& tangent,
		const std::vector<glm::mat3>& face
	);
};



#endif