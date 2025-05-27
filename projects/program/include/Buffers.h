#pragma once
#include "stb_image.h"
#include "bhdr.h"
#pragma warning(disable: 4312)

#define stringify(name) #name
enum class vtype { vec2, vec3, vec4 };
enum class dtype { Float, Double, Int, uInt, Byte, uByte, Mat4, Mat3, Mat2 };
enum class USAGE { STATIC, DYNAMIC, STREAM, COMPUTE };


static uint32_t CURRENT_TEXTURE_INDEX = GL_TEXTURE0;


// texparam unused, maybe there will be use for it in the future.
enum class texparam {
	MIPMAP,
	WRAP_S,
	WRAP_T,
	WRAP_R,
	MIN_FILTER,
	MAG_FILTER
};
enum class texarg {
	// texture wrap parameters.
	REPEAT,
	REPEAT_MIRRORED,
	CLAMP_EDGE,
	CLAMP_BORDER,
	// min/mag && mipmap filter arguments
	LINEAR,
	NEAREST,
	MIPMAP_LINEAR_GL_LINEAR,
	MIPMAP_LINEAR_GL_NEAREST,
	MIPMAP_NEAREST_GL_LINEAR,
	MIPMAP_NEAREST_GL_NEAREST
};

inline uint32_t size(vtype t)
{
	switch (t)
	{
	case vtype::vec2: return 2;
	case vtype::vec3: return 3;
	case vtype::vec4: return 4;
	}
	return 0;
}

inline uint32_t size(dtype d)
{
	switch (d)
	{
	case dtype::Float: return 4;
	case dtype::Double: return 8;
	case dtype::Int: return 4;
	case dtype::uInt: return 4;
	case dtype::Byte: return 1;
	case dtype::uByte: return 1;
	}
	return NULL;
}

inline GLenum getGL_TYPE(dtype d)
{
	switch (d)
	{
	case dtype::Float: return GL_FLOAT;
	case dtype::Double: return GL_DOUBLE;
	case dtype::Int: return GL_INT;
	case dtype::uInt: return GL_UNSIGNED_INT;
	case dtype::Byte: return GL_BYTE;
	case dtype::uByte: return GL_UNSIGNED_BYTE;
	}
	return NULL;
}

inline GLenum get_BufferUsage(USAGE u)
{
	switch (u)
	{
	case USAGE::STATIC: return GL_STATIC_DRAW;
	case USAGE::DYNAMIC: return GL_DYNAMIC_DRAW;
	case USAGE::STREAM: return GL_STREAM_DRAW;
	}
	return NULL;
}

inline GLenum get_TexArg(texarg tp)
{
	switch (tp)
	{
	case texarg::REPEAT: return GL_REPEAT;
	case texarg::REPEAT_MIRRORED: return GL_MIRRORED_REPEAT;
	case texarg::CLAMP_EDGE: return GL_CLAMP_TO_EDGE;
	case texarg::CLAMP_BORDER: return GL_CLAMP_TO_BORDER;
	case texarg::LINEAR: return GL_LINEAR;
	case texarg::NEAREST: return GL_NEAREST;
	case texarg::MIPMAP_LINEAR_GL_LINEAR: return GL_LINEAR_MIPMAP_LINEAR;
	case texarg::MIPMAP_LINEAR_GL_NEAREST: return GL_NEAREST_MIPMAP_LINEAR;
	case texarg::MIPMAP_NEAREST_GL_LINEAR: return GL_LINEAR_MIPMAP_NEAREST;
	case texarg::MIPMAP_NEAREST_GL_NEAREST: return GL_NEAREST_MIPMAP_NEAREST;
	default: std::cout << "invalid texture argument!"; exit(-1);
	}
}

struct Vertex {
	vtype vec;
	dtype data_type;
	uint32_t offset;
	bool normalized;

	Vertex() :
		vec(vtype::vec2),
		data_type(dtype::Float),
		offset(0),
		normalized(true)
	{}

	Vertex(
		const vtype& v,
		const dtype& d,
		const uint32_t& off,
		const bool& norm
	) :
		vec(v),
		data_type(d),
		offset(off),
		normalized(norm)
	{}

	Vertex(
		const vtype& v,
		const dtype& d,
		const bool& norm
	) :
		vec(v),
		data_type(d),
		normalized(norm),
		offset(0xFFFF)
	{}

	uint32_t Size() { return size(vec) * size(data_type); }
};

struct VBO
{
	uint32_t m_va;
	uint32_t m_vb;
	uint32_t m_stride;
	USAGE usage;
	Array<Vertex> attributes;

	VBO(Array<float>& data, Array<Vertex>& attributes, USAGE&& drawUsage) :
		m_stride(0), attributes(attributes), usage(std::move(drawUsage))
	{
		glGenBuffers(1, &m_vb);
		glGenVertexArrays(1, &m_va);
		glBindVertexArray(m_va);

		glBindBuffer(GL_ARRAY_BUFFER, m_vb);
		glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.get_c(), get_BufferUsage(drawUsage));

		attributes[0].offset = 0;
		m_stride += attributes[0].Size();
		for (int i = 1; i < attributes.size; ++i) 
		{
			attributes[i].offset = m_stride;
			m_stride += attributes[i].Size();
		}
		for (int i = 0; i < attributes.size; ++i)
		{
			glEnableVertexArrayAttrib(m_va, i);
			glVertexAttribPointer(
				i,
				size(attributes[i].vec),
				getGL_TYPE(attributes[i].data_type),
				attributes[i].normalized,
				m_stride,
				reinterpret_cast<const void*>(attributes[i].offset)
			);
		}
	}

	const void bindVAO() const { glBindVertexArray(m_va);                                }
	const void bindVBO() const { glBindBuffer(GL_ARRAY_BUFFER, m_vb);                    }
	const void unbind()  const { glBindBuffer(GL_ARRAY_BUFFER, 0); glBindVertexArray(0); }
};

struct IBO
{
	uint32_t m_id;
	size_t m_count;
	USAGE usage;
	Array<unsigned int> indices;

	IBO(Array<unsigned int>& data, USAGE&& us) : indices(data), usage(std::move(us))
	{
		glGenBuffers(1, &m_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size_bytes(), data.get_c(), get_BufferUsage(us));
		m_count = data.size;
	}
	~IBO() { glDeleteBuffers(1, &m_id); }
	const void bind()   const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id); }
	const void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
};

template<unsigned int textureDimensions>
struct texFormat
{
	texarg wrapSTR[textureDimensions];
	texarg filters[2];
	int mipmap;
	int width, height;
	int channelCount;

	texFormat()
	{
		for (int i = 0; i < textureDimensions; ++i)
		{
			wrapSTR[i] = texarg::REPEAT;
		}
		filters[0] = texarg::LINEAR;
		filters[1] = texarg::LINEAR;
		mipmap = 0, width = 0, height = 0, channelCount = 0;
	}

	texFormat(
		texarg&& filter_MIN,
		texarg&& filter_MAG,
		texarg wrapArgs[textureDimensions])
	{
		filters[0] = filter_MIN;
		filters[1] = filter_MAG;
		for (int i = 0; i < textureDimensions; ++i)
		{
			wrapSTR[i] = wrapArgs[i];
		}
		mipmap = 0, width = 0, height = 0, channelCount = 0;
	}

	// takes in initializer_list in the form of {FILTER_MIN, FILTER_MAG, WRAP_S, WRAP_T, ...}
	// for an N dimensional texture there are N Wrapping Arguments.
	texFormat(const std::initializer_list<texarg>& wrapArguments)
	{
		size_t arg_size = wrapArguments.size();
		ON_CONDITION_DO(arg_size - 2 != textureDimensions, ERR_EXIT("Texture Wrap-Argument-List Size != Texture Dimensions (3D/2D)"));
		filters[0] = wrapArguments.begin()[0];
		filters[1] = wrapArguments.begin()[1];
		for (int i = 2; i < arg_size; ++i)
		{
			wrapSTR[i - 2] = wrapArguments.begin()[i];
		}
		mipmap = 0, width = 0, height = 0, channelCount = 0;
	}

	// takes in initializer_list in the form of {FILTER_MIN, FILTER_MAG, WRAP_S, WRAP_T}
	texFormat<textureDimensions>& operator =(const std::initializer_list<texarg>& wrapArguments)
	{
		size_t arg_size = wrapArguments.size();
		ON_CONDITION_DO(arg_size - 2 != textureDimensions, ERR_EXIT("Texture Wrap-Argument-List Size != Texture Dimensions (3D/2D)"));
		filters[0] = wrapArguments.begin()[0];
		filters[1] = wrapArguments.begin()[1];
		for (int i = 2; i < arg_size; ++i)
		{
			wrapSTR[i - 2] = wrapArguments.begin()[i];
		}
		mipmap = 0, width = 0, height = 0, channelCount = 0;
	}

	texFormat<textureDimensions>& operator =(const texFormat<textureDimensions>& tf)
	{
		for (int i = 0; i < textureDimensions; ++i)
		{
			wrapSTR[i] = tf.wrapSTR[i];
		}
		filters[0] = tf.filters[0];
		filters[1] = tf.filters[1];
		mipmap = tf.mipmap;
		width = tf.width;
		height = tf.height;
		channelCount = tf.channelCount;
		return *this;
	}
};

class Texture2D
{
private:
	uint32_t m_id;
	uint32_t m_TextureIndex;
	std::string filepath;
	texFormat<2> m_format;
public:
	Texture2D() : filepath("Assets/default.jpg")
	{
		m_format = texFormat<2>();
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, get_TexArg(m_format.wrapSTR[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, get_TexArg(m_format.wrapSTR[1]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, get_TexArg(m_format.filters[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, get_TexArg(m_format.filters[1]));

		stbi_set_flip_vertically_on_load(true);
		unsigned int gl_format = GL_RGB;
		unsigned char* img_buffer = stbi_load(
			filepath.c_str(), 
			&m_format.width,
			&m_format.height,
			&m_format.channelCount, 
			0
		);

		ON_CONDITION_DO(
			!img_buffer,
			stbi_image_free(img_buffer);
			ERR_EXIT("couldn't Load " << filepath << ". Please check your filepath")
		);

		if (m_format.channelCount == 4)
			gl_format = GL_RGBA;

		m_format.mipmap = 1 + (int)floor((float)log2(MATH_MSIMD_MAX(m_format.width, m_format.height)));
		for (int i = 0; i < m_format.mipmap; ++i) 
		{
			glTexImage2D(GL_TEXTURE_2D, i, gl_format, m_format.width, m_format.height, 0, gl_format, GL_UNSIGNED_BYTE, img_buffer);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(img_buffer);

		m_TextureIndex = CURRENT_TEXTURE_INDEX;
		++CURRENT_TEXTURE_INDEX;
	}

	Texture2D(std::string&& filepath, const bool& generateMipMap = true) : filepath(filepath)
	{
		m_format = texFormat<2>();
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, get_TexArg(m_format.wrapSTR[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, get_TexArg(m_format.wrapSTR[1]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, get_TexArg(m_format.filters[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, get_TexArg(m_format.filters[1]));

		stbi_set_flip_vertically_on_load(true);
		unsigned int gl_rgbFormat = GL_RGB;
		unsigned char* img_buffer = stbi_load(
			filepath.c_str(),
			&m_format.width,
			&m_format.height,
			&m_format.channelCount,
			0
		);

		ON_CONDITION_DO(
			!img_buffer, 
			stbi_image_free(img_buffer);
			ERR_EXIT("couldn't Load " << filepath << ". Please check your filepath")
		);

		if (m_format.channelCount == 4) gl_rgbFormat = GL_RGBA;


		if(!generateMipMap)
			glTexImage2D(GL_TEXTURE_2D, 0,
				gl_rgbFormat,
				m_format.width,
				m_format.height,
				0,
				gl_rgbFormat,
				GL_UNSIGNED_BYTE,
				img_buffer
			);
		else
		{
			m_format.mipmap = 1 + (int)floor((float)log2(MATH_MSIMD_MAX(m_format.width, m_format.height)));
			for (int i = 0; i < m_format.mipmap; ++i)
			{
				glTexImage2D(GL_TEXTURE_2D, i,
					gl_rgbFormat,
					m_format.width, m_format.height,
					0,
					gl_rgbFormat,
					GL_UNSIGNED_BYTE,
					img_buffer
				);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(img_buffer);

		m_TextureIndex = CURRENT_TEXTURE_INDEX;
		++CURRENT_TEXTURE_INDEX;
	}



	Texture2D(std::string&& filepath, const texFormat<2>& parameters, bool generateMipMap=true) :
		filepath(filepath),
		m_format(parameters)
	{
		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     get_TexArg(m_format.wrapSTR[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     get_TexArg(m_format.wrapSTR[1]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, get_TexArg(m_format.filters[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, get_TexArg(m_format.filters[1]));
		if (m_format.wrapSTR[0] == texarg::CLAMP_BORDER || m_format.wrapSTR[1] == texarg::CLAMP_BORDER) 
		{
			float borderColor[4] = { 1.0f, 0.0784f, 0.5464f, 1.0f };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &borderColor[0]);
		}

		stbi_set_flip_vertically_on_load(true);
		unsigned int gl_rgbFormat = GL_RGB;
		unsigned char* img_buffer = stbi_load(
			filepath.c_str(),
			&m_format.width,
			&m_format.height,
			&m_format.channelCount,
			0
		);
		ON_CONDITION_DO(
			!img_buffer,
			stbi_image_free(img_buffer);
			ERR_EXIT("couldn't Load " << filepath << ". Please check your filepath")
		);

		if (m_format.channelCount == 4) gl_rgbFormat = GL_RGBA;
		if (!generateMipMap)
		{
			glTexImage2D(
				GL_TEXTURE_2D, 0,
				gl_rgbFormat,
				m_format.width, 
				m_format.height,
				0,
				gl_rgbFormat,
				GL_UNSIGNED_BYTE,
				img_buffer);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			m_format.mipmap = 1 + (int)floor((float)log2(MATH_MSIMD_MAX(m_format.width, m_format.height)));
			for (int i = 0; i < m_format.mipmap; ++i)
			{
				glTexImage2D(GL_TEXTURE_2D, i,
					gl_rgbFormat,
					m_format.width, m_format.height,
					0,
					gl_rgbFormat,
					GL_UNSIGNED_BYTE,
					img_buffer
				);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		stbi_image_free(img_buffer);

		m_TextureIndex = CURRENT_TEXTURE_INDEX;
		++CURRENT_TEXTURE_INDEX;
	}



	~Texture2D()
	{
		glDeleteBuffers(1, &m_id);
	}
	void    bindToActive()  { glBindTexture(GL_TEXTURE_2D, m_id); }
	void unbindFromActive() { glBindTexture(GL_TEXTURE_2D, 0);    }
	void setActiveTexture() { glActiveTexture(m_TextureIndex);    }

};

class Texture3D
{
};


inline void enableTexture2D()
{
	glEnable(GL_TEXTURE_2D);
}
inline void disableTexture2D()
{
	glDisable(GL_TEXTURE_2D);
}
inline void enableTexture3D()
{
	glEnable(GL_TEXTURE_3D);
}
inline void disableTexture3D()
{
	glDisable(GL_TEXTURE_3D);
}

inline void setActiveTexture(const uint32_t& tex_index)
{
	glActiveTexture(GL_TEXTURE0 + tex_index);
}
