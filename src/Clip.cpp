#include "Clip.h"
#include "Vertex.h"
#include "Function.h"
#include "StateMachine.h"

const std::vector<glm::vec4> ViewPlanes = {
	//Near
	glm::vec4(0,0,1,1),
	//far
	glm::vec4(0,0,-1,1),
	//left
	glm::vec4(1,0,0,1),
	//top
	glm::vec4(0,1,0,1),
	//right
	glm::vec4(-1,0,0,1),
	//bottom 
	glm::vec4(0,-1,0,1)
};
float Clip::near = 0.3f;
float Clip::far = 100.0f;

// 判断点是否在平面法向一侧
bool Inside(const glm::vec4& line, const glm::vec4& p) {

	return line.x * p.x + line.y * p.y + line.z * p.z + line.w * p.w > -EPSILON;
}

// 判断是否所有定点都在裁剪空间内部
bool AllVertexsInside(const std::vector<VertexOut> v) {
	for (int i = 0; i < v.size(); i++) {
		if (fabs(v[i].windowPos.x) > fabs(v[i].windowPos.w))
			return false;
		if (fabs(v[i].windowPos.y) > fabs(v[i].windowPos.w))
			return false;
		if (fabs(v[i].windowPos.z) > fabs(v[i].windowPos.w))
			return false;
	}
	return true;
}

// 交点，通过点到平面距离插值得到
VertexOut Intersect(const VertexOut& v1, const VertexOut& v2, const glm::vec4& line) {
	float da = v1.windowPos.x * line.x + v1.windowPos.y * line.y + v1.windowPos.z * line.z + v1.windowPos.w * line.w;
	float db = v2.windowPos.x * line.x + v2.windowPos.y * line.y + v2.windowPos.z * line.z + v2.windowPos.w * line.w;

	float weight = da / (da - db);
	return lerp(v1, v2, weight);
}

// 裁剪算法
std::vector<VertexOut> Clip::SutherlandHodgeman(const VertexOut& v1, const VertexOut& v2, const VertexOut& v3) {
	std::vector<VertexOut> output = { v1,v2,v3 };
	// 完全在裁剪空间中无需裁剪
	if (AllVertexsInside(output)) {
		return output;
	}
	// 否则遍历每一个平面对三角形每条边进行裁剪
	for (int i = 0; i < ViewPlanes.size(); i++) {
		// 上一次的输出是下一次的输入
		std::vector<VertexOut> input(output);
		output.clear();
		for (int j = 0; j < input.size(); j++) {
			// 每条边的起始点
			VertexOut current = input[j];
			// 每条边的结束点
			VertexOut last = input[(j + input.size() - 1) % input.size()];
			// 两点在裁剪面两侧则进行裁剪，求两点连线和裁剪平面的交点作为新顶点，并将在内部的点加入结果
			if (Inside(ViewPlanes[i], current.windowPos)) {
				if (!Inside(ViewPlanes[i], last.windowPos)) {
					VertexOut intersecting = Intersect(last, current, ViewPlanes[i]);
					output.push_back(intersecting);
				}
				output.push_back(current);
			}
			else if (Inside(ViewPlanes[i], last.windowPos)) {
				VertexOut intersecting = Intersect(last, current, ViewPlanes[i]);
				output.push_back(intersecting);
			}
		}
	}
	return output;
}

// 裁剪空间剔除掉完全不可见的
bool Clip::ClipSpaceCull(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) {
	if (v1.w < near && v2.w < near && v3.w < near)
		return false;
	if (v1.w > far && v1.w > far && v3.w > far)
		return false;
	if (fabs(v1.x) <= fabs(v1.w) || fabs(v1.y) <= fabs(v1.w) || fabs(v1.z) <= fabs(v1.w))
		return true;
	if (fabs(v2.x) <= fabs(v2.w) || fabs(v2.y) <= fabs(v2.w) || fabs(v2.z) <= fabs(v2.w))
		return true;
	if (fabs(v3.x) <= fabs(v3.w) || fabs(v3.y) <= fabs(v3.w) || fabs(v3.z) <= fabs(v3.w))
		return true;
	return false;
}

// 正面/背面剔除
bool Clip::FaceCull(FaceCullMode face, const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3) {

	glm::vec3 tmp1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	glm::vec3 tmp2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

	// 叉乘得到法向量
	glm::vec3 normal = glm::normalize(glm::cross(tmp1, tmp2));
	// NDC中观察方向指向+z
	glm::vec3 view = glm::vec3(0, 0, 1);
	if (face == Back)
		return glm::dot(normal, view) > 0;
	else
		return glm::dot(normal, view) < 0;
}
