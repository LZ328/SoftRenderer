#ifndef TEXTURE_H
#define TEXTURE_H

#include "Global.h"

class Image;

// 二维纹理
class Texture2D {

protected:
	Image* image;
	glm::vec2 texSize;
	glm::vec2 unitTexel;

public:

	glm::vec4 Sample2D(const glm::vec2& texcoord);
	Texture2D();
	Texture2D(const std::string& filename);
	Texture2D(Image* img);
	Texture2D(const Texture2D& t);
	Image* GetImage() const;
	~Texture2D();
};

enum FaceDir {
	front,
	back,
	left,
	right,
	top,
	bottom
};

// 并不是真正意义的三维纹理，这里特指 cube map
class Texture3D {
public:
	std::vector<Texture2D*> faces;
	Texture3D(const std::string& filename);
	Texture3D(const Texture3D& t);
	virtual ~Texture3D();

	glm::vec4 SampleCUBE(const glm::vec3& v);
};

#endif