#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Global.h"

class Camera;
class Model;
class Light;
class DirectionLight;
class FrameBuffer;
class Graphics;
class Texture3D;

// 正面/背面剔除
enum FaceCullMode {
	Back,
	Front
};

// 渲染模式
enum RenderMode {
	Line,
	Fill
};

// 状态机，记录系统的渲染状态
class StateMachine
{
private:
	static StateMachine* localInstance;
	StateMachine();

public:
	~StateMachine();

	FrameBuffer* frontBuffer;
	FrameBuffer* backBuffer;
	int width, height;
	Graphics* graphics;
	int fps, triangle;

#pragma region UNIFORM

	Camera* camera;
	std::vector<Model*> models;
	std::vector<DirectionLight*> dirLights;
	glm::mat4 viewPortMatrix;
	glm::vec4 ambient;
	float modelScale;
	Model* skyBox;
	int currentSkybox;
	std::vector<Texture3D*> skyboxMap;

#pragma endregion UNIFORM

#pragma region STATE

	bool faceCull;
	bool viewCull;
	FaceCullMode faceCullMode;
	bool writeDepth;
	bool writeColor;
	RenderMode renderMode;

	bool drawSkyBox;
	bool drawShadow;

	QMutex mutex;

#pragma endregion STATE

	static StateMachine* GetInstance();

	void Init(int w, int h, float fov);
	void Resize(int w, int h);
	void AddModel(Model* m);
	void RemoveModel(int num);
	void AddDirectionLight(DirectionLight* l);

	void CleanUp();

	void ClearColor(int r, int g, int b, int a = 255);
	void ClearDepth(float d = 1.0f);

	void SwapBuffer();
	void SetViewPortMatrix(int w, int h);

	void DrawShadow(Light* light);
	void DrawScene();
	unsigned char* GetFrame();

	void Destroy();
};


#endif