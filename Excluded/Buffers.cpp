#include "Buffers.h"

u_int GL_TypeSize(GLenum glType) {
	switch (glType)
	{
	case GL_FLOAT: return 4;
	case GL_INT: return 4;
	case GL_UNSIGNED_INT: return 4;
	case GL_BYTE: return 1;
	case GL_UNSIGNED_BYTE: return 1;
	case GL_SHORT: return 2;
	case GL_UNSIGNED_SHORT: return 2;
	case GL_FIXED: return 2;
	case GL_HALF_FLOAT: return 2;
	case GL_DOUBLE: return 8;
	}
	std::cout << "ERROR! " << glType << " is an invalid GLenum Data Type\n";
	exit(-1);
}

/*
======================================================================
							BUFFER LAYOUT
======================================================================
*/

attrib::attrib()
{
	this->vec_count = 0;
	this->vec_size = 0;
	this->type = GL_FLOAT;
	this->normalize = false;
}

attrib::attrib(u_int v_count, u_int v_size, GLenum dtype, bool to_normalize)
{
	this->vec_count = v_count;
	this->vec_size = v_size;
	this->type = dtype;
	this->normalize = to_normalize;
}

attrib::attrib(const attrib& cp)
{
	this->vec_count = cp.vec_count;
	this->vec_size = cp.vec_size;
	this->type = cp.type;
	this->normalize = cp.normalize;
}

std::string attrib::debug() const
{
	std::stringstream ss;
	ss << "\nvec count: " << this->vec_count
		<< "\nvec size: " << this->vec_size
		<< "\ntype: " << this->type
		<< "\nto Normalize [0->1]: " << this->normalize;
	return ss.str();
}

VA_Layout::VA_Layout()
{
	this->tight_attribs = false;
	this->attrib_counter = 0;
	this->stride = 0;
	this->offsets = Array<u_int>((u_int)0, 16);
	this->attributes = Array<attrib>(16, true);
}

VA_Layout::VA_Layout(bool tight_attribs)
{
	this->tight_attribs = tight_attribs;
	this->attrib_counter = 0;
	this->stride = 0;
	this->offsets = Array<u_int>((u_int)0, 16);
	this->attributes = Array<attrib>(16, true);
}

VA_Layout::VA_Layout(const VA_Layout& copy)
{
	this->tight_attribs = copy.tight_attribs;
	this->attrib_counter = copy.attrib_counter;
	this->stride = copy.stride;
	this->offsets = copy.offsets;
	this->attributes = copy.attributes;
}

void VA_Layout::Add(const attrib& a)
{
	this->attributes.push_back(attrib(a));
	u_int s_bytes = GL_TypeSize(a.type);

	if (!this->tight_attribs) this->stride += a.vec_size * s_bytes;
	if (this->attrib_counter != 0)
		if (!this->tight_attribs) this->offsets[this->attrib_counter] = this->offsets.sum() + a.vec_size * s_bytes;
		else this->offsets[this->attrib_counter] = this->offsets.sum() + (size_t)a.vec_count * a.vec_size * s_bytes;

	this->attrib_counter++;
}

std::string VA_Layout::debug() const
{
	std::stringstream ss;
	for (unsigned int i = 0; i < this->attrib_counter; i++)
	{
		ss << "\nAttribute " << i
			<< this->attributes[i].debug()
			<< "\noffset: " << this->offsets[i]
			<< "\n";
	}
	ss << "\nstride: " << this->stride
		<< "\n";

	return ss.str();
}

/*
======================================================================
						   VERTEX BUFFER
======================================================================
*/

VertexBuffer::VertexBuffer(void* data, u_int size_bytes, GLenum usage)
{
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, size_bytes, data, usage);
}

VertexBuffer::VertexBuffer(void* data, u_int size_bytes)
{
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
	glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &this->id);
}

void VertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->id);
}

void VertexBuffer::unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/*
======================================================================
							INDEX BUFFER
======================================================================
*/

IndexBuffer::IndexBuffer(Array<u_int>& data, GLenum usage)
{
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size * sizeof(u_int), data.get_c(), usage);
	this->index_count = (u_int)data.size;
}

IndexBuffer::IndexBuffer(Array<u_int>& data)
{
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size * sizeof(u_int), data.get_c(), GL_STATIC_DRAW);
	this->index_count = (u_int)data.size;
}

IndexBuffer::IndexBuffer(u_int data[], size_t array_size)
{
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, array_size * sizeof(u_int), (void*)data, GL_STATIC_DRAW);
	this->index_count = (u_int)array_size;
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &this->id);
}

void IndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
}

void IndexBuffer::unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*
======================================================================
							VERTEX ARRAY
======================================================================
*/

VertexArray::VertexArray(bool tight_attribs)
{
	glGenVertexArrays(1, &this->id);
	glBindVertexArray(this->id);
	this->layout = VA_Layout(tight_attribs);
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(1, &this->id);
}

void VertexArray::bind() const {
	glBindVertexArray(this->id);
}

void VertexArray::unbind() const {
	glBindVertexArray(0);
}

void VertexArray::gl_activateAttribs() const
{
	this->bind();
	std::cout << this->layout.debug();
	for (unsigned int i = 0; i < this->layout.attrib_counter; i++)
	{
		glEnableVertexArrayAttrib(this->id, i);
		glVertexAttribPointer(i, this->layout.attributes[i].vec_size, this->layout.attributes[i].type, this->layout.attributes[i].normalize, this->layout.stride, (const void*)this->layout.offsets[i]);
	}
}

void VertexArray::gl_disableAttrib(u_int attrib_locID) const {
	glDisableVertexArrayAttrib(this->id, attrib_locID);
}

void VertexArray::gl_enableAttrib(u_int attrib_locID) const {
	glEnableVertexArrayAttrib(this->id, attrib_locID);
}

std::string VertexArray::debug() const {
	std::string debug = "\nVertexArray ID: " + std::to_string(this->id) + "\n";
	debug += this->layout.debug() + "\n\n\n";
	return debug;
}

//std::cout << "\nPushing Attribute: "
//	<< this->attrib_counter << "\n"
//	<< "vec size: " << vec_size << "\n"
//	<< "Type: " << GL_FLOAT << "\n"
//	<< "To Normalize: " << norm << "\n"
//	<< "Stride: " << this->stride << "\n"
//	<< "offset: " << (void*)this->offsets[this->attrib_counter] << "\n";