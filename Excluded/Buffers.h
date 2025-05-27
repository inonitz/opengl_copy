#pragma once
#pragma warning (disable : 4312)
#include "glad/glad.h"
#include <sstream>
#include <string>
#include "Array.h"

// create VertexBuffer, IndexBuffer array classes for a case in which someone wants
// to allocate multiple vertex buffers && index buffers.

// allow multi attribute functionality for vertexbuffer class, such as:
// add float type attribute with push
// add color with u_int RGBA attribute
// etc

typedef unsigned int u_int;
enum class buffer_usages {
	u_dynamic = GL_DYNAMIC_DRAW,
	u_static = GL_STATIC_DRAW,
	u_stream = GL_STREAM_DRAW
};

u_int GL_TypeSize(GLenum glType);

template<typename T> GLenum getType(T type) {
	std::string type_str = typeid(type).name();
	if (type_str.compare("float") == 0) return GL_FLOAT;
	else if (type_str.compare("int") == 0) return GL_INT;
	else if (type_str.compare("unsigned int") == 0) return GL_UNSIGNED_INT;
	else if (type_str.compare("char") == 0) return GL_BYTE;
	else {
		std::cout << "couldn't figure out type of object. Please use the following types:\n float\nint\nunsigned int\nchar\n";
		exit(1);
	}
}

struct attrib {
	u_int vec_count;
	u_int vec_size;
	GLenum type;
	bool normalize;
	attrib();
	attrib(u_int, u_int, GLenum, bool);
	attrib(const attrib&);

	std::string debug() const;
};

struct VA_Layout {
	u_int attrib_counter;
	u_int stride;

	Array<attrib> attributes;
	Array<u_int> offsets;
	bool tight_attribs;

	VA_Layout();
	VA_Layout(bool tight_attributes);
	VA_Layout(const VA_Layout&);
	void Add(const attrib& attribute);

	std::string debug() const;
};

class VertexBuffer {
private:
	u_int id;
public:

	VertexBuffer(void* data, u_int size_bytes, GLenum usage);
	VertexBuffer(void* data, u_int size_bytes);

	~VertexBuffer();

	void bind();
	void unbind();
};

class IndexBuffer {
private:
	u_int id;
public: u_int index_count;

public:

	IndexBuffer(Array<u_int>& data, GLenum usage);
	IndexBuffer(Array<u_int>& data);
	IndexBuffer(u_int data[], size_t data_size);
	~IndexBuffer();

	void bind();
	void unbind();
};

class VertexArray {
private:
	u_int id;
public:	VA_Layout layout;

public:
	VertexArray(bool tight_packed_attribs);
	~VertexArray();

	template<typename T> void push(u_int vec_count, u_int vec_size, bool norm);

	void gl_activateAttribs() const;
	void gl_disableAttrib(u_int attrib_locID) const;
	void gl_enableAttrib(u_int attrib_locID) const;

	void bind() const;
	void unbind() const;
	std::string debug() const;
};

template<typename T> inline void VertexArray::push(u_int vec_count, u_int vec_size, bool norm) { std::cout << "CANNOT USE TYPES OTHER THAN: float, unsigned int, int, char\n"; exit(-1); }
template<> inline void VertexArray::push<float>(u_int vec_count, u_int vec_size, bool norm)
{
	this->layout.Add({ vec_count, vec_size, GL_FLOAT, norm });
}
template<> inline void VertexArray::push<u_int>(u_int vec_count, u_int vec_size, bool norm)
{
	this->layout.Add({ vec_count, vec_size, GL_UNSIGNED_INT, norm });
}
template<> inline void VertexArray::push<int>(u_int vec_count, u_int vec_size, bool norm)
{
	this->layout.Add({ vec_count, vec_size, GL_INT, norm });
}
template<> inline void VertexArray::push<char>(u_int vec_count, u_int vec_size, bool norm)
{
	this->layout.Add({ vec_count, vec_size, GL_BYTE, norm });
}

/*/
if (!this->layout.tight_attribs) this->layout.stride += vec_size * 4;
	if (this->attrib_counter != 0)
		if (!this->tight_attribs) this->offsets[this->attrib_counter] = this->offsets.sum() + vec_size * 4;
		else this->offsets[this->attrib_counter] = this->offsets.sum() + (size_t)vec_count * vec_size * 4;
	this->bind();
	glEnableVertexArrayAttrib(this->id, this->attrib_counter);
	glVertexAttribPointer(this->attrib_counter, vec_size, GL_FLOAT, norm, this->stride, (void*)this->offsets[this->attrib_counter]);
	this->attrib_counter++;
*/