#pragma once
#include "glad/glad.h"
#include "stb_image.h"
#include <string>
#include <iostream>
#include <sstream>

static unsigned int ACTIVE_TEXTURES = 0;
static unsigned int TEXTURE_IDS[16];

enum class TextureType {
	tex1D = GL_TEXTURE_1D,
	tex2D = GL_TEXTURE_2D,
	tex3D = GL_TEXTURE_3D
};

enum class FilterTypes {
	lin = GL_LINEAR,
	near = GL_NEAREST,
	mipLin = GL_LINEAR_MIPMAP_LINEAR,
	mipNear = GL_NEAREST_MIPMAP_NEAREST,
	linMipNear = GL_LINEAR_MIPMAP_NEAREST,
	nearMipLin = GL_NEAREST_MIPMAP_LINEAR
}; FilterTypes filters[6] =
{
	FilterTypes::lin,
	FilterTypes::near,
	FilterTypes::mipLin,
	FilterTypes::mipNear,
	FilterTypes::linMipNear,
	FilterTypes::nearMipLin
};

enum class WrapTypes {
	rep = GL_REPEAT,
	repMirr = GL_MIRRORED_REPEAT,
	clampEdge = GL_CLAMP_TO_EDGE
	// Theres also GL_CLAMP_TO_BORDER
	// but its too much of a hassle to take care of
	// for now.
}; WrapTypes wrappers[3] =
{
	WrapTypes::rep,
	WrapTypes::repMirr,
	WrapTypes::clampEdge
};

// helper function that determines if a texture parameter is valid.
bool validParam(GLenum tp)
{
	int count = 0;
	for (const auto& c : filters) { if (tp == static_cast<std::underlying_type<FilterTypes>::type>(c)) count++; }
	for (const auto& c : wrappers) { if (tp == static_cast<std::underlying_type<FilterTypes>::type>(c)) count++; }

	return count == 1;
}

void deleteExistingTextures()
{
	if (ACTIVE_TEXTURES == 0) { std::cout << "CANT DELETE 0 TEXTURES"; exit(-1); }
	glDeleteTextures(ACTIVE_TEXTURES, TEXTURE_IDS);
}

struct texture2D {
	unsigned int id;
	unsigned int TextureNumber;
	std::string texPath;
	unsigned int TexChannels;
	unsigned int mipMapLevel;
	GLenum TexParams[4];

	texture2D() :
		mipMapLevel(0),
		TextureNumber(ACTIVE_TEXTURES)
	{
		this->texPath = "Assets/default.jpg";
		// loading the Texture Image data
		stbi_set_flip_vertically_on_load(true);
		int width, height, channelCount;
		unsigned char* data = stbi_load(texPath.c_str(), &width, &height, &channelCount, 0);

		// error checking.
		if (!data) { std::cout << "Failed to load Texture File!"; exit(-1); }
		// checking the Texture's channel count (ex. RGB)
		GLenum chFormat = GL_RGBA;
		if (channelCount == 3)
			chFormat = GL_RGB;

		this->TexChannels = channelCount;
		this->TexParams[2] = GL_REPEAT; this->TexParams[3] = GL_REPEAT;
		this->TexParams[0] = GL_LINEAR; this->TexParams[1] = GL_LINEAR;
		// Settings up the openGL texture parameters
		glGenTextures(1, &this->id);
		glBindTexture(GL_TEXTURE_2D, this->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexImage2D(GL_TEXTURE_2D, this->mipMapLevel, chFormat, width, height, 0, chFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		// free image data.
		stbi_image_free(data);
		TEXTURE_IDS[ACTIVE_TEXTURES] = this->id;
		ACTIVE_TEXTURES++;
	}

	texture2D(std::string texPath, unsigned int mipMapLevel, GLenum TexParam[4]) :
		texPath(texPath),
		mipMapLevel(mipMapLevel),
		TextureNumber(ACTIVE_TEXTURES),
		TexParams()
	{
		// loading the Texture Image data
		stbi_set_flip_vertically_on_load(true);
		int width, height, channelCount;
		unsigned char* data = stbi_load(texPath.c_str(), &width, &height, &channelCount, 0);

		// error checking.
		if (!data) { std::cout << "Failed to load Texture File!"; exit(-1); }
		for (int i = 0; i < 4; i++)
		{
			if (!validParam(TexParam[i])) { std::cout << "One of the Texture Parameters is Invalid! "; exit(-1); }
		}
		// checking the Texture's channel count (ex. RGB)
		GLenum gpuFormat = GL_RGB8, texFormat = GL_RGB;
		if (channelCount == 4) {
			gpuFormat = GL_RGBA8;
			texFormat = GL_RGBA;
		}
		this->TexChannels = channelCount;
		memcpy(this->TexParams, TexParam, sizeof(GLenum) * 4);

		// Settings up the openGL texture parameters
		glGenTextures(1, &this->id);
		glBindTexture(GL_TEXTURE_2D, this->id);
		glTextureStorage2D(this->id, mipMapLevel, gpuFormat, width, height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexParam[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexParam[1]);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TexParam[2]);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TexParam[3]);

		glTexImage2D(GL_TEXTURE_2D, mipMapLevel, texFormat, width, height, 0, texFormat, GL_UNSIGNED_BYTE, data);
		//glGenerateMipmap(GL_TEXTURE_2D);
		// free image data.
		stbi_image_free(data);
		TEXTURE_IDS[ACTIVE_TEXTURES] = this->id;
		ACTIVE_TEXTURES++;
	}

	void bind() { glActiveTexture(GL_TEXTURE0 + this->TextureNumber); glBindTexture(GL_TEXTURE_2D, this->id); }
	void unbind() { glActiveTexture(GL_TEXTURE0 + this->TextureNumber); glBindTexture(GL_TEXTURE_2D, 0); }
	std::string debug()
	{
		std::stringstream ss;
		ss << "\nTexture Debug: "
			<< "\nid: " << this->id
			<< "\nTextureNumber: " << this->TextureNumber
			<< "\ntexture path: " << this->texPath
			<< "\nChannels in Texture Image: " << this->TexChannels
			<< "\nmipMap level: " << this->mipMapLevel
			<< "\nTexture Parameters: "
			<< this->TexParams[0] << " "
			<< this->TexParams[1] << " "
			<< this->TexParams[2] << " "
			<< this->TexParams[3] << " "
			<< "\nEnd of debug.\n";

		return ss.str();
	}

	~texture2D() { glDeleteTextures(1, &this->id); }
};

// UNFINISHED CODE, WILL BE USED FOR GENRAL DIMENSION-PURPOSE TEXTURE CLASS
//template<TextureType t>
//struct textureParameters {
//	GLenum wrap[texWrapSize(t)];
//	GLenum minMag[2];
//
//	textureParameters()
//	{
//
//	}
//
//	textureParameters(GLenum wrap[texWrapSize(t)], GLenum min, GLenum mag)
//	{
//		for (unsigned int i = 0; i < texWrapSize(t); i++)
//		{
//			if()
//		}
//
//	}
//
//};
//
//unsigned int texWrapSize(TextureType texType)
//{
//	switch (texType)
//	{
//	case TextureType::tex3D: return 3;
//	case TextureType::tex2D: return 2;
//	case TextureType::tex1D: return 1;
//	default:
//		std::cout << "Not a valid texture type!";
//		return -1;
//	}
//}

// only supporting 2D textures for the time being
//struct texture {
//	unsigned int id;
//	std::string texPath;
//	TextureType texType;
//	textureParameters<texType> tp;
//
//	texture()
//	{
//		glGenTextures(1, &this->id);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	}
//	texture(std::string texPath, TextureType texType, unsigned int mipMapLevel) : texPath(texPath), texType(texType)
//	{
//		stbi_set_flip_vertically_on_load(true);
//		int width, height, channelCount;
//		unsigned char* data = stbi_load(texPath.c_str(), &width, &height, &channelCount, 0);
//
//		GLenum chFormat = GL_RGBA;
//		if (channelCount == 3)
//			chFormat = GL_RGB;
//
//		glGenTextures(1, &this->id);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//		glTexImage2D(GL_TEXTURE_2D, mipMapLevel, chFormat, width, height, 0, chFormat, GL_UNSIGNED_BYTE, data);
//
//
//	}
//
//private:
//	void setupTexture(TextureType texType) {
//		switch (texType) {
//		case TextureType::tex3D:
//			glBindTexture(GL_TEXTURE_3D, this->id);
//
//		}
//	}
//};
