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

// ��Ⱦ״̬��ʼ��
void StateMachine::Init(int w, int h, float fov)
{
	// �ӿ�
	width = w;
	height = h;
	SetViewPortMatrix(w, h);
	// ˫�ػ���
	frontBuffer = new FrameBuffer(w, h);
	backBuffer = new FrameBuffer(w, h);
	// ���
	camera = new Camera(w, h, fov);
	// ƽ�й�Դ
	DirectionLight* light = new DirectionLight();
	dirLights.push_back(light);
	// ģ��
	models.reserve(10);
	// ���ƶ����ʼ��
	graphics = new Graphics();
	// �趨RT
	graphics->SetRenderTarget(backBuffer);

	// Ĭ�ϵذ�ƽ��
	Model* floor = Model::CreateFloor(glm::vec3(0, -0.5, 0), 2.0f);
	floor->name = "Floor";
	floor->meshes[0]->name = "Floor_mesh";
	Texture2D* floorTex = new Texture2D("assets/Textures/DSC_3521.jpg");
	floor->meshes[0]->material->SetTexture(floorTex);
	delete floorTex;
	models.push_back(floor);

	// Ĭ������skybox
	skyBox = Model::CreateSkyBox();
	skyboxMap[0] = new Texture3D("assets/skybox/valley_skybox.hdr");
	skyboxMap[1] = new Texture3D("assets/skybox/city_skybox.hdr");
	skyboxMap[2] = new Texture3D("assets/skybox/peters_square_night.hdr");
}

// �ӿ�resize��Ŀǰû�õ�
void StateMachine::Resize(int w, int h)
{
	CleanUp();
	width = w;
	height = h;
	frontBuffer = new FrameBuffer(w, h);
	backBuffer = new FrameBuffer(w, h);
	SetViewPortMatrix(w, h);
}

// ���һ��ģ��
void StateMachine::AddModel(Model* m)
{
	models.push_back(m);
}

// �Ƴ�һ��ģ��
void StateMachine::RemoveModel(int num)
{
	if (num >= models.size())
		return;
	if (models[num])
		delete models[num];
	models.erase(models.begin() + num);
}

// ���һ��ƽ�й�
void StateMachine::AddDirectionLight(DirectionLight* l)
{
	dirLights.push_back(l);
}

// ���buffer
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

// ʹ��ָ����ɫ��� color buffer
void StateMachine::ClearColor(int r, int g, int b, int a)
{
	backBuffer->ClearColor(r, g, b, a);
}

// ʹ�ø���ֵ��� depth buffer
void StateMachine::ClearDepth(float d)
{
	backBuffer->ClearDepth(d);
}

// ����ǰ�󻺳�
void StateMachine::SwapBuffer()
{
	FrameBuffer* p = frontBuffer;
	frontBuffer = backBuffer;
	backBuffer = p;
	graphics->SetRenderTarget(backBuffer);
}

// �趨�ӿڱ任����
inline void StateMachine::SetViewPortMatrix(int w, int h)
{
	viewPortMatrix = Transform::GetViewPortMatrix(0, 0, w, h);
}

// ������Ӱ
void StateMachine::DrawShadow(Light* light)
{
	mutex.lock();
	// ����ϵ���������ֿɸı�
	glm::mat4 globelScale = glm::scale(glm::mat4(1.0f), glm::vec3(modelScale, modelScale, modelScale));
	// ����ģʽ��������Ӱ
	if (renderMode == Fill) {
		for (int j = 0; j < models.size(); j++) {
			// �趨��Ⱦ�õ��ı���
			Uniform u(globelScale * models[j]->GetModelMatrix(), light->viewMatrix, light->projectorMatrix);
			// ��Ⱦ��Ӱ
			graphics->DrawModel(*models[j], u, 1);
		}
	}
	mutex.unlock();
}

// ���Ƴ���
void StateMachine::DrawScene()
{
	mutex.lock();
	// ����ϵ��
	glm::mat4 globelScale = glm::scale(glm::mat4(1.0f), glm::vec3(modelScale, modelScale, modelScale));
	// View����
	glm::mat4 viewMatrix = camera->GetViewMatrix();
	// ͶӰ����
	glm::mat4 projectionMatrix = camera->GetProjectionMatrix();
	// ������������������
	triangle = 0;

	// ÿ��ģ�����Դ����
	for (int i = 0; i < models.size(); i++) {
		for (int j = 0; j < dirLights.size(); j++) {
			Uniform u(globelScale * models[i]->GetModelMatrix(), viewMatrix, projectionMatrix);
			u.cameraPos = glm::vec4(camera->position, 1.0f);
			u.ambient = ambient;
			u.dirLight = dirLights[j];
			graphics->DrawModel(*models[i], u, 2);
		}
	}
	// ���� sky box
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

// ��ȡ��ǰ֡����
unsigned char* StateMachine::GetFrame()
{
	return frontBuffer->colorBuffer->data;
}

// ����״̬������
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
