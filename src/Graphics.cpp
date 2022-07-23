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

// �趨��ȾĿ�� Render Target��Vulkan��Attachment��
void Graphics::SetRenderTarget(FrameBuffer* i)
{
	renderTarget = i;
}

// ����ģ��
void Graphics::DrawModel(const Model& model, Uniform& u, int pass)
{
	if (model.meshes.empty()) {
		return;
	}
	// �� Mesh ����
	for (int i = 0; i < model.meshes.size(); i++) {
		Mesh* mesh = model.meshes[i];
		if (model.meshes[i]->VBO.empty() || model.meshes[i]->EBO.empty()) {
			return;
		}
		// �趨���ʲ���
		mesh->material->SetupUniform(u);
		// ���ݸ����� pass ������ȡ shader
		shader = mesh->material->GetShader(pass - 1);
		if (shader == nullptr)
			return;
		// �� shader ������Ⱦ�õ��Ĳ���
		shader->SetUniform(&u);

		// ��װ�����β�����
		for (int j = 0; j < mesh->EBO.size(); j += 3) {
			Vertex p1, p2, p3;
			p1 = mesh->VBO[mesh->EBO[j]];
			p2 = mesh->VBO[mesh->EBO[j + 1]];
			p3 = mesh->VBO[mesh->EBO[j + 2]];
			DrawTriangle(p1, p2, p3);
		}
	}
}

// ����������
void Graphics::DrawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
	// Vertex Shader �任����
	VertexOut vo1, vo2, vo3;
	vo1 = shader->VertexShader(v1);
	vo2 = shader->VertexShader(v2);
	vo3 = shader->VertexShader(v3);

	// �ü��ռ��޳�����ȫ���ɼ��򲻻�
	if (sys->viewCull && !Clip::ClipSpaceCull(vo1.windowPos, vo2.windowPos, vo3.windowPos)) {
		return;
	}

	// �ü��㷨�������¶������ͼԪ
	std::vector<VertexOut> clipingVertexs = Clip::SutherlandHodgeman(vo1, vo2, vo3);

	// ͸�ӳ��� + �ӿڱ任
	for (int i = 0; i < clipingVertexs.size(); i++) {
		PerspectiveDivision(clipingVertexs[i]);
		ViewPortMapping(clipingVertexs[i]);
	}

	// ��դ��
	int n = clipingVertexs.size() - 3 + 1;
	for (int i = 0; i < n; i++) {
		sys->triangle++;

		// ÿ���������γ�һ���������Σ�����Ӱ��Ч�ʣ������ü���Ҳ��������̫���¶���
		VertexOut f1 = clipingVertexs[0];
		VertexOut f2 = clipingVertexs[i + 1];
		VertexOut f3 = clipingVertexs[i + 2];

		// �����ߣ����ڲü��㷨����
		if (sys->renderMode == Line) {
			DrawLine(f1, f2);
			DrawLine(f2, f3);
			DrawLine(f3, f1);
		}
		// ������դ��
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

// �߽纯����դ���㷨
void Graphics::RasterizeTriangle(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3)
{
	int fxPtX[3], fxPtY[3];
	fxPtX[0] = (int)(v1.windowPos.x + 0.5); fxPtY[0] = (int)(v1.windowPos.y + 0.5);
	fxPtX[1] = (int)(v2.windowPos.x + 0.5); fxPtY[1] = (int)(v2.windowPos.y + 0.5);
	fxPtX[2] = (int)(v3.windowPos.x + 0.5); fxPtY[2] = (int)(v3.windowPos.y + 0.5);

	float Z[3] = { v1.windowPos.z,v2.windowPos.z,v3.windowPos.z };

	// �����ε� Bounding box
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

	// �����߽纯����ֵ
	// F = I * Px + J * Py + K
	int F01 = (I01 * startX) + (J01 * startY) + K01;
	int F02 = (I02 * startX) + (J02 * startY) + K02;
	int F03 = (I03 * startX) + (J03 * startY) + K03;

	// ��������������ڼ������Ĳ�ֵϵ��
	int Delta = (fxPtX[1] - fxPtX[0]) * (fxPtY[2] - fxPtY[0]) - (fxPtX[0] - fxPtX[2]) * (fxPtY[0] - fxPtY[1]);
	float OneDivideDelta = 1 / (float)Delta;

	// z����
	Z[1] = (Z[1] - Z[0]) * OneDivideDelta;
	Z[2] = (Z[2] - Z[0]) * OneDivideDelta;
	float zx = I03 * Z[1] + I01 * Z[2];

	// y����߽纯��
	int Cy1 = F01, Cy2 = F02, Cy3 = F03;

	for (int y = startY; y < endY; y++)
	{
		// x����߽纯������ֵ��y����һ��
		int Cx1 = Cy1;
		int Cx2 = Cy2;
		int Cx3 = Cy3;
		// ��ǰƬԪ���
		float depth = Z[0] + Cx3 * Z[1] + Cx1 * Z[2];
		for (int x = startX; x < endX; x++)
		{
			int mask = Cx1 | Cx2 | Cx3;
			if (mask >= 0) {
				// ��Ȳ���
				if (depth <= renderTarget->GetDepth(x, y)) {
					// ��ֵϵ��������ͨ���߽纯���õ�
					glm::vec3 weights = glm::vec3(Cx2 * OneDivideDelta, Cx3 * OneDivideDelta, Cx1 * OneDivideDelta);
					// ���Ĳ�ֵ
					VertexOut frag = lerp(v1, v2, v3, weights);
					// ��ֵ��ǵ�͸�ӻָ�����Ϊ֮ǰ͸�ӳ���ʱ�����Խ�����͸�Ӳ�ֵ����
					PerspectiveRestore(frag);
					// ��ɫд��
					if (sys->writeColor) renderTarget->WriteColor(x, y, shader->FragmentShader(frag));
					// ���д��
					if (sys->writeDepth) renderTarget->WriteDepth(x, y, depth);
				}
			}
			// ����x�߽纯��
			Cx1 += I01;
			Cx2 += I02;
			Cx3 += I03;
			// �������
			depth += zx;
		}
		// ����y�߽纯��
		Cy1 += J01;
		Cy2 += J02;
		Cy3 += J03;
	}
}

// bresenhamLine �����㷨
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

	// б��С��1
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
	// б�ʴ���1�����öԳ���
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

// ͸�ӻָ�
inline void Graphics::PerspectiveRestore(VertexOut& v)
{
	v.worldPos /= v.Z;
	v.lightPos /= v.Z;
	v.color /= v.Z;
	v.texcoord /= v.Z;
	v.normal /= v.Z;
	v.TBN /= v.Z;
}
// ͸�ӳ���
inline void Graphics::PerspectiveDivision(VertexOut& v)
{
	// ��¼ԭ���� z ֵ������͸�Ӳ�ֵ����
	v.Z = 1 / v.windowPos.w;
	// ͸�ӳ���
	v.windowPos /= v.windowPos.w;
	v.windowPos.w = 1.0f;
	// ӳ�䵽 0 �� 1
	v.windowPos.z = (v.windowPos.z + 1.0) * 0.5;
	// ͸�Ӳ�ֵ�������������Զ����� z����ֵ��֮��Ҫ�ָ�
	v.worldPos *= v.Z;
	v.lightPos *= v.Z;
	v.color *= v.Z;
	v.texcoord *= v.Z;
	v.normal *= v.Z;
	v.TBN *= v.Z;
}

// �ӿڱ任
inline void Graphics::ViewPortMapping(VertexOut& v)
{
	v.windowPos = sys->viewPortMatrix * v.windowPos;
}