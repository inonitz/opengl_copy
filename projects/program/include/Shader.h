#pragma once
#include <glad/glad.h>
#include <string>
#include "m_simd.h"


struct vertexShader
{
	unsigned int id;
	int compiled;
	std::string vert_source;


	vertexShader();
	vertexShader(const std::string& source);

	int compile();
	unsigned int get_id();
};

struct fragmentShader
{
	unsigned int id;
	std::string frag_source;
	int compiled;
	fragmentShader();
	fragmentShader(const std::string& source);

	int compile();
	unsigned int get_id();
};

/*
======================================================================
							SHADER CLASS
======================================================================
*/

class Shader {
private:
	const char* m_Filepath;
	vertexShader vSource;
	fragmentShader fSource;
	bool bound[2];
public:
	unsigned int m_ProgramID;

public:
	Shader(const std::string& filepath);
	Shader();

	int linkAndUse();
	int link();
	void use();
	void print_sources();

	int getUniformLocation(const std::string& name);
	void setUniform1f(const std::string& name, float f);
	void setUniform1i(const std::string& name, int f);
	void setUniform1ui(const std::string& name, unsigned int ui);
	void setUniform3ui(const std::string& name, vec3& v);
	void setUniform3f(const std::string& name, vec3& v);
	void setUniform4f(const std::string& name, vec4& v);
	void setUniformMat4(const std::string& name, mat4&);
	void setUniformMat4(const std::string& name, float*);

	void setUniform3ui(const std::string& name, vec3&& v);
	void setUniform3f(const std::string& name, vec3&& v);
	void setUniform4f(const std::string& name, vec4&& v);
	void setUniformMat4(const std::string& name, mat4&&);
};