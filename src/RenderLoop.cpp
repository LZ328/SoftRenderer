#include "RenderLoop.h"
#include "Graphics.h"
#include "Vertex.h"
#include "StateMachine.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "Texture.h"
#include "MaterialBase.h"
#include "Model.h"
#include "Light.h"

RenderLoop::RenderLoop(QObject* parent)
	: QObject(parent), scale(1.0), sys(nullptr)
{
	sys = StateMachine::GetInstance();
}

RenderLoop::~RenderLoop() {}

void RenderLoop::Stop() {
	shouldClosed = true;
}

void RenderLoop::Zoom(float delta)
{
	scale += delta;
	if (scale < 0.1)
		scale = 0.1;
	if (scale > 10)
		scale = 10;
	sys->modelScale = scale;
}

// ��Ⱦ������ѭ��
void RenderLoop::MainLoop()
{
	// ��ʼģ��
	Model* wBox = new Model(Mesh::CreateBox(glm::vec3(0, 0, 0), 0.5));
	Texture2D* mt = new Texture2D("assets/Textures/container.jpg");
	wBox->meshes[0]->material->SetTexture(mt);
	wBox->yaw = 60;
	wBox->name = "Box";
	wBox->meshes[0]->name = "Box";
	sys->AddModel(wBox);

	// ��Ⱦѭ��
	while (!shouldClosed) {
		/***** Shadow Pass *****/
		if (sys->drawShadow) {
			// �������ã�2048 ShadowMap �ֱ���
			sys->SetViewPortMatrix(2048, 2048);
			sys->writeColor = false;
			sys->writeDepth = true;
			sys->faceCullMode = Front;
			// ���Դ���� ShadowMap
			for (int i = 0; i < sys->dirLights.size(); i++) {
				sys->dirLights[i]->SetShadowMap(nullptr);
				// �趨 RT
				FrameBuffer* SBO = new FrameBuffer(2048, 2048);
				sys->graphics->SetRenderTarget(SBO);
				SBO->ClearDepth(1.0f);
				// ���� ShadowMap
				sys->DrawShadow(sys->dirLights[i]);
				// �洢 ShadowMap
				sys->dirLights[i]->SetShadowMap(new Texture2D(SBO->depthBuffer));
				delete SBO;
			}
		}
		else {
			for (int i = 0; i < sys->dirLights.size(); i++) {
				sys->dirLights[i]->SetShadowMap(nullptr);
			}
		}
		/***** Forward Render Pass *****/
		// ��������
		sys->SetViewPortMatrix(sys->width, sys->height);
		sys->writeColor = true;
		sys->writeDepth = true;
		sys->faceCullMode = Back;
		// �趨 RT
		sys->graphics->SetRenderTarget(sys->backBuffer);
		sys->ClearColor(0, 0, 0);
		sys->ClearDepth(1.0f);
		// ���Ƴ���
		sys->DrawScene();
		// ˫�ػ��彻��
		sys->SwapBuffer();
		// �����ź��ڻ�������ʾ��ǰ֡
		emit Vsync(sys->GetFrame(), 0, 0);
		sys->fps++;
	}
}
