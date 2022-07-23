#include "Graphics.h"
#include "FrameBuffer.h"
#include "Vertex.h"
#include "ShaderBase.h"
#include "StateMachine.h"
#include "Function.h"
#include "Clip.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "MaterialBase.h"
#include "Model.h"

Graphics::Graphics()
	: renderTarget(nullptr), shader(nullptr), sys(nullptr)
{
	sys = StateMachine::GetInstance();
}

Graphics::~Graphics()
{
	renderTarget = nullptr;
	shader = nullptr;
	sys = nullptr;
}

// 设定渲染目标 Render Target（Vulkan：Attachment）
void Graphics::SetRenderTarget(FrameBuffer* i)
{
	renderTarget = i;
}

// 绘制模型
void Graphics::DrawModel(const Model& model, Uniform& u, int pass)
{
	if (model.meshes.empty()) {
		return;
	}
	// 逐 Mesh 绘制
	for (int i = 0; i < model.meshes.size(); i++) {
		Mesh* mesh = model.meshes[i];
		if (model.meshes[i]->VBO.empty() || model.meshes[i]->EBO.empty()) {
			return;
		}
		// 设定材质参数
		mesh->material->SetupUniform(u);
		// 根据给定的 pass 索引获取 shader
		shader = mesh->material->GetShader(pass - 1);
		if (shader == nullptr)
			return;
		// 给 shader 传递渲染用到的参数
		shader->SetUniform(&u);

		// 组装三角形并绘制
		for (int j = 0; j < mesh->EBO.size(); j += 3) {
			Vertex p1, p2, p3;
			p1 = mesh->VBO[mesh->EBO[j]];
			p2 = mesh->VBO[mesh->EBO[j + 1]];
			p3 = mesh->VBO[mesh->EBO[j + 2]];
			DrawTriangle(p1, p2, p3);
		}
	}
}

// 绘制三角形
void Graphics::DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	// Vertex Shader 变换顶点
	VertexOut vo1, vo2, vo3;
	vo1 = shader->VertexShader(v1);
	vo2 = shader->VertexShader(v2);
	vo3 = shader->VertexShader(v3);

	// 裁剪空间剔除，完全不可见则不画
	if (sys->viewCull && !Clip::ClipSpaceCull(vo1.windowPos, vo2.windowPos, vo3.windowPos)) {
		return;
	}

	// 裁剪算法，生成新顶点和新图元
	std::vector<VertexOut> clipingVertexs = Clip::SutherlandHodgeman(vo1, vo2, vo3);

	// 透视除法 + 视口变换
	for (int i = 0; i < clipingVertexs.size(); i++) {
		PerspectiveDivision(clipingVertexs[i]);
		ViewPortMapping(clipingVertexs[i]);
	}

	// 光栅化
	int n = clipingVertexs.size() - 3 + 1;
	for (int i = 0; i < n; i++) {
		sys->triangle++;

		// 每三个顶点形成一个新三角形，可能影响效率，不过裁剪后也不会生成太多新顶点
		VertexOut f1 = clipingVertexs[0];
		VertexOut f2 = clipingVertexs[i + 1];
		VertexOut f3 = clipingVertexs[i + 2];

		// 仅画线，用于裁剪算法调试
		if (sys->renderMode == Line) {
			DrawLine(f1, f2);
			DrawLine(f2, f3);
			DrawLine(f3, f1);
		}
		// 正常光栅化
		else {
			if (sys->faceCull && !Clip::FaceCull(sys->faceCullMode, f1.windowPos, f2.windowPos, f3.windowPos)) {
				return;
			}
			if (sys->faceCullMode == Back)
				RasterizeTriangle(f1, f2, f3);
			else
				RasterizeTriangle(f1, f3, f2);
		}
	}
}

// 边界函数光栅化算法
void Graphics::RasterizeTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	int fxPtX[3], fxPtY[3];
	fxPtX[0] = (int)(v1.windowPos.x + 0.5); fxPtY[0] = (int)(v1.windowPos.y + 0.5);
	fxPtX[1] = (int)(v2.windowPos.x + 0.5); fxPtY[1] = (int)(v2.windowPos.y + 0.5);
	fxPtX[2] = (int)(v3.windowPos.x + 0.5); fxPtY[2] = (int)(v3.windowPos.y + 0.5);

	float Z[3] = { v1.windowPos.z,v2.windowPos.z,v3.windowPos.z };

	// 三角形的 Bounding box
	int startX = max(min(min(fxPtX[0], fxPtX[1]), fxPtX[2]), 0) & int(0xFFFFFFFE);
	int endX = min(max(max(fxPtX[0], fxPtX[1]), fxPtX[2]), renderTarget->width - 1);

	int startY = max(min(min(fxPtY[0], fxPtY[1]), fxPtY[2]), 0) & int(0xFFFFFFFE);
	int endY = min(max(max(fxPtY[0], fxPtY[1]), fxPtY[2]), renderTarget->height - 1);

	// I Ay - By
	int I01 = fxPtY[0] - fxPtY[1];
	int I02 = fxPtY[1] - fxPtY[2];
	int I03 = fxPtY[2] - fxPtY[0];

	// J Bx - Ax
	int J01 = fxPtX[1] - fxPtX[0];
	int J02 = fxPtX[2] - fxPtX[1];
	int J03 = fxPtX[0] - fxPtX[2];

	// K AxBy - AyBx
	int K01 = fxPtX[0] * fxPtY[1] - fxPtX[1] * fxPtY[0];
	int K02 = fxPtX[1] * fxPtY[2] - fxPtX[2] * fxPtY[1];
	int K03 = fxPtX[2] * fxPtY[0] - fxPtX[0] * fxPtY[2];

	// 三个边界函数初值
	// F = I * Px + J * Py + K
	int F01 = (I01 * startX) + (J01 * startY) + K01;
	int F02 = (I02 * startX) + (J02 * startY) + K02;
	int F03 = (I03 * startX) + (J03 * startY) + K03;

	// 三角形面积，用于计算重心插值系数
	int Delta = (fxPtX[1] - fxPtX[0]) * (fxPtY[2] - fxPtY[0]) - (fxPtX[0] - fxPtX[2]) * (fxPtY[0] - fxPtY[1]);
	float OneDivideDelta = 1 / (float)Delta;

	// z增量
	Z[1] = (Z[1] - Z[0]) * OneDivideDelta;
	Z[2] = (Z[2] - Z[0]) * OneDivideDelta;
	float zx = I03 * Z[1] + I01 * Z[2];

	// y方向边界函数
	int Cy1 = F01, Cy2 = F02, Cy3 = F03;

	for (int y = startY; y < endY; y++)
	{
		// x方向边界函数，初值和y方向一样
		int Cx1 = Cy1;
		int Cx2 = Cy2;
		int Cx3 = Cy3;
		// 当前片元深度
		float depth = Z[0] + Cx3 * Z[1] + Cx1 * Z[2];
		for (int x = startX; x < endX; x++)
		{
			int mask = Cx1 | Cx2 | Cx3;
			if (mask >= 0) {
				// 深度测试
				if (depth <= renderTarget->GetDepth(x, y)) {
					// 插值系数，可以通过边界函数得到
					glm::vec3 weights = glm::vec3(Cx2 * OneDivideDelta, Cx3 * OneDivideDelta, Cx1 * OneDivideDelta);
					// 重心插值
					VertexOut frag = lerp(v1, v2, v3, weights);
					// 插值后记得透视恢复，因为之前透视除法时对属性进行了透视插值矫正
					PerspectiveRestore(frag);
					// 颜色写入
					if (sys->writeColor) renderTarget->WriteColor(x, y, shader->FragmentShader(frag));
					// 深度写入
					if (sys->writeDepth) renderTarget->WriteDepth(x, y, depth);
				}
			}
			// 更新x边界函数
			Cx1 += I01;
			Cx2 += I02;
			Cx3 += I03;
			// 更新深度
			depth += zx;
		}
		// 更新y边界函数
		Cy1 += J01;
		Cy2 += J02;
		Cy3 += J03;
	}
}

// bresenhamLine 画线算法
void Graphics::DrawLine(const VertexOut& from, const VertexOut& to)
{
	int startX = from.windowPos.x;
	int startY = from.windowPos.y;
	int endX = to.windowPos.x;
	int endY = to.windowPos.y;
	startX = startX >= 0 ? startX : 0;
	startX = startX < renderTarget->width ? startX : renderTarget->width - 1;
	startY = startY >= 0 ? startY : 0;
	startY = startY < renderTarget->height ? startY : renderTarget->height - 1;
	endX = endX >= 0 ? endX : 0;
	endX = endX < renderTarget->width ? endX : renderTarget->width - 1;
	endY = endY >= 0 ? endY : 0;
	endY = endY < renderTarget->height ? endY : renderTarget->height - 1;

	int dx = endX - startX;
	int dy = endY - startY;
	if (dx == 0 && dy == 0)
		return;
	int Xstep = 1, Ystep = 1;
	if (dx == 0) {
		Xstep = 0;
	}
	else if (dx < 0)
	{
		Xstep = -1;
		dx = -dx;
	}
	if (dy == 0) {
		Ystep = 0;
	}
	else if (dy < 0)
	{
		Ystep = -1;
		dy = -dy;
	}
	int currentX = startX;
	int currentY = startY;

	// 斜率小于1
	if (dy <= dx)
	{
		int P = 2 * dy - dx;
		for (int i = 0; i <= dx; ++i)
		{
			renderTarget->WriteColor(currentX, currentY, 255, 0, 0);
			currentX += Xstep;
			if (P <= 0)
				P += 2 * dy;
			else
			{
				currentY += Ystep;
				P += 2 * (dy - dx);
			}
		}
	}
	// 斜率大于1，利用对称性
	else
	{
		int P = 2 * dx - dy;
		for (int i = 0; i <= dy; ++i)
		{
			renderTarget->WriteColor(currentX, currentY, 255, 0, 0);
			currentY += Ystep;
			if (P <= 0)
				P += 2 * dx;
			else
			{
				currentX += Xstep;
				P -= 2 * (dy - dx);
			}
		}
	}
}

// 透视恢复
inline void Graphics::PerspectiveRestore(VertexOut& v)
{
	v.worldPos /= v.Z;
	v.lightPos /= v.Z;
	v.color /= v.Z;
	v.texcoord /= v.Z;
	v.normal /= v.Z;
	v.TBN /= v.Z;
}
// 透视除法
inline void Graphics::PerspectiveDivision(VertexOut& v)
{
	// 记录原本的 z 值，用于透视插值矫正
	v.Z = 1 / v.windowPos.w;
	// 透视除法
	v.windowPos /= v.windowPos.w;
	v.windowPos.w = 1.0f;
	// 映射到 0 到 1
	v.windowPos.z = (v.windowPos.z + 1.0) * 0.5;
	// 透视插值矫正让所有属性都除以 z，插值完之后还要恢复
	v.worldPos *= v.Z;
	v.lightPos *= v.Z;
	v.color *= v.Z;
	v.texcoord *= v.Z;
	v.normal *= v.Z;
	v.TBN *= v.Z;
}

// 视口变换
inline void Graphics::ViewPortMapping(VertexOut& v)
{
	v.windowPos = sys->viewPortMatrix * v.windowPos;
}