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

// 渲染管线主循环
void RenderLoop::MainLoop()
{
	// 初始模型
	Model* wBox = new Model(Mesh::CreateBox(glm::vec3(0, 0, 0), 0.5));
	Texture2D* mt = new Texture2D("assets/Textures/container.jpg");
	wBox->meshes[0]->material->SetTexture(mt);
	wBox->yaw = 60;
	wBox->name = "Box";
	wBox->meshes[0]->name = "Box";
	sys->AddModel(wBox);

	// 渲染循环
	while (!shouldClosed) {
		/***** Shadow Pass *****/
		if (sys->drawShadow) {
			// 参数设置，2048 ShadowMap 分辨率
			sys->SetViewPortMatrix(2048, 2048);
			sys->writeColor = false;
			sys->writeDepth = true;
			sys->faceCullMode = Front;
			// 逐光源生成 ShadowMap
			for (int i = 0; i < sys->dirLights.size(); i++) {
				sys->dirLights[i]->SetShadowMap(nullptr);
				// 设定 RT
				FrameBuffer* SBO = new FrameBuffer(2048, 2048);
				sys->graphics->SetRenderTarget(SBO);
				SBO->ClearDepth(1.0f);
				// 绘制 ShadowMap
				sys->DrawShadow(sys->dirLights[i]);
				// 存储 ShadowMap
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
		// 参数设置
		sys->SetViewPortMatrix(sys->width, sys->height);
		sys->writeColor = true;
		sys->writeDepth = true;
		sys->faceCullMode = Back;
		// 设定 RT
		sys->graphics->SetRenderTarget(sys->backBuffer);
		sys->ClearColor(0, 0, 0);
		sys->ClearDepth(1.0f);
		// 绘制场景
		sys->DrawScene();
		// 双重缓冲交换
		sys->SwapBuffer();
		// 发送信号在画布上显示当前帧
		emit Vsync(sys->GetFrame(), 0, 0);
		sys->fps++;
	}
}
