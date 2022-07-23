#include "StateMachine.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "Image.h"
#include "Transform.h"
#include "Camera.h"
#include "Graphics.h"
#include "Light.h"
#include "Model.h"
#include "Mesh.h"
#include "MaterialBase.h"
#include "BlinnShader.h"
#include "ShadowShader.h"
#include "SkyBoxShader.h"

StateMachine* StateMachine::localInstance = nullptr;

StateMachine::StateMachine() :
	frontBuffer(nullptr),
	backBuffer(nullptr),
	width(0),
	height(0),
	graphics(nullptr),
	fps(0),
	triangle(0),
	camera(nullptr),
	viewPortMatrix(glm::mat4(1.0f)),
	ambient(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)),
	modelScale(1.0f),
	skyBox(nullptr),
	currentSkybox(0),
	skyboxMap(3, nullptr),
	faceCull(true),
	viewCull(true),
	faceCullMode(Back),
	writeDepth(true),
	writeColor(true),
	renderMode(Fill),
	drawSkyBox(false),
	drawShadow(true)
{}

StateMachine::~StateMachine() {}

StateMachine* StateMachine::GetInstance()
{
	if (localInstance == nullptr)
	{
		localInstance = new StateMachine();
	}
	return localInstance;
}

// 渲染状态初始化
void StateMachine::Init(int w, int h, float fov)
{
	// 视口
	width = w;
	height = h;
	SetViewPortMatrix(w, h);
	// 双重缓冲
	frontBuffer = new FrameBuffer(w, h);
	backBuffer = new FrameBuffer(w, h);
	// 相机
	camera = new Camera(w, h, fov);
	// 平行光源
	DirectionLight* light = new DirectionLight();
	dirLights.push_back(light);
	// 模型
	models.reserve(10);
	// 绘制对象初始化
	graphics = new Graphics();
	// 设定RT
	graphics->SetRenderTarget(backBuffer);

	// 默认地板平面
	Model* floor = Model::CreateFloor(glm::vec3(0, -0.5, 0), 2.0f);
	floor->name = "Floor";
	floor->meshes[0]->name = "Floor_mesh";
	Texture2D* floorTex = new Texture2D("assets/Textures/DSC_3521.jpg");
	floor->meshes[0]->material->SetTexture(floorTex);
	delete floorTex;
	models.push_back(floor);

	// 默认两个skybox
	skyBox = Model::CreateSkyBox();
	skyboxMap[0] = new Texture3D("assets/skybox/valley_skybox.hdr");
	skyboxMap[1] = new Texture3D("assets/skybox/city_skybox.hdr");
	skyboxMap[2] = new Texture3D("assets/skybox/peters_square_night.hdr");
}

// 视口resize，目前没用到
void StateMachine::Resize(int w, int h)
{
	CleanUp();
	width = w;
	height = h;
	frontBuffer = new FrameBuffer(w, h);
	backBuffer = new FrameBuffer(w, h);
	SetViewPortMatrix(w, h);
}

// 添加一个模型
void StateMachine::AddModel(Model* m)
{
	models.push_back(m);
}

// 移除一个模型
void StateMachine::RemoveModel(int num)
{
	if (num >= models.size())
		return;
	if (models[num])
		delete models[num];
	models.erase(models.begin() + num);
}

// 添加一个平行光
void StateMachine::AddDirectionLight(DirectionLight* l)
{
	dirLights.push_back(l);
}

// 清空buffer
void StateMachine::CleanUp()
{
	if (frontBuffer)
		delete frontBuffer;
	if (backBuffer)
		delete backBuffer;
	frontBuffer = nullptr;
	backBuffer = nullptr;
	width = 0;
	height = 0;
}

// 使用指定颜色清空 color buffer
void StateMachine::ClearColor(int r, int g, int b, int a)
{
	backBuffer->ClearColor(r, g, b, a);
}

// 使用给定值清空 depth buffer
void StateMachine::ClearDepth(float d)
{
	backBuffer->ClearDepth(d);
}

// 交换前后缓冲
void StateMachine::SwapBuffer()
{
	FrameBuffer* p = frontBuffer;
	frontBuffer = backBuffer;
	backBuffer = p;
	graphics->SetRenderTarget(backBuffer);
}

// 设定视口变换矩阵
inline void StateMachine::SetViewPortMatrix(int w, int h)
{
	viewPortMatrix = Transform::GetViewPortMatrix(0, 0, w, h);
}

// 绘制阴影
void StateMachine::DrawShadow(Light* light)
{
	mutex.lock();
	// 缩放系数，鼠标滚轮可改变
	glm::mat4 globelScale = glm::scale(glm::mat4(1.0f), glm::vec3(modelScale, modelScale, modelScale));
	// 画线模式不产生阴影
	if (renderMode == Fill) {
		for (int j = 0; j < models.size(); j++) {
			// 设定渲染用到的变量
			Uniform u(globelScale * models[j]->GetModelMatrix(), light->viewMatrix, light->projectorMatrix);
			// 渲染阴影
			graphics->DrawModel(*models[j], u, 1);
		}
	}
	mutex.unlock();
}

// 绘制场景
void StateMachine::DrawScene()
{
	mutex.lock();
	// 缩放系数
	glm::mat4 globelScale = glm::scale(glm::mat4(1.0f), glm::vec3(modelScale, modelScale, modelScale));
	// View矩阵
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	// 投影矩阵
	glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
	// 三角形数量，调试用
	triangle = 0;

	// 每个模型逐光源绘制
	for (int i = 0; i < models.size(); i++) {
		for (int j = 0; j < dirLights.size(); j++) {
			Uniform u(globelScale * models[i]->GetModelMatrix(), viewMatrix, projectionMatrix);
			u.cameraPos = glm::vec4(camera->position, 1.0f);
			u.ambient = ambient;
			u.dirLight = dirLights[j];
			graphics->DrawModel(*models[i], u, 2);
		}
	}
	// 绘制 sky box
	if (drawSkyBox) {
		faceCullMode = Front;
		writeDepth = false;
		Uniform u(glm::mat4(1.0f), viewMatrix, projectionMatrix);
		u.cubemap = skyboxMap[currentSkybox];
		graphics->DrawModel(*skyBox, u, 2);
		faceCullMode = Back;
	}
	mutex.unlock();
}

// 获取当前帧数据
unsigned char* StateMachine::GetFrame()
{
	return frontBuffer->colorBuffer->data;
}

// 销毁状态机对象
void StateMachine::Destroy()
{
	CleanUp();
	if (camera)
		delete camera;
	for (int i = 0; i < models.size(); i++) {
		delete models[i];
	}
	for (int i = 0; i < dirLights.size(); i++) {
		delete dirLights[i];
	}
	if (skyBox)
		delete skyBox;
	for (int i = 0; i < 3; i++) {
		if (skyboxMap[i])
			delete skyboxMap[i];
	}
	Shader* shader = Shader::GetInstance();
	shader->Destroy();
	shader = BlinnShader::GetInstance();
	shader->Destroy();
	shader = ShadowShader::GetInstance();
	shader->Destroy();
	shader = SkyBoxShader::GetInstance();
	shader->Destroy();
	camera = nullptr;
	skyBox = nullptr;
	if (localInstance)
		delete localInstance;
	localInstance = nullptr;
}
