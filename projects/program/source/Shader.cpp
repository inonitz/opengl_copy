#include "Shader.h"
#include <fstream>


vertexShader::vertexShader() : id(0), compiled(0), vert_source("") {}


vertexShader::vertexShader(const std::string& source) :
	id(glCreateShader(GL_VERTEX_SHADER)),
	compiled(0),
	vert_source(source)
{
	const char* a = source.c_str();
	glShaderSource(id, 1, &a, NULL);
}

int vertexShader::compile()
{
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		int log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(id, 1024, &log_length, message);
		std::cout << "COULDN'T COMPILE VERTEX SHADER!\n" << message << std::endl;
	}
	return compiled;
}

unsigned int vertexShader::get_id() {
	return id;
}

/*
======================================================================
							FRAGMENT SHADER
======================================================================
*/

fragmentShader::fragmentShader() : id(0), frag_source(""), compiled(0) {}

fragmentShader::fragmentShader(const std::string& source) :
	id(glCreateShader(GL_FRAGMENT_SHADER)),
	frag_source(source),
	compiled(0)
{
	const char* a = source.c_str();
	glShaderSource(id, 1, &a, NULL);
}

int fragmentShader::compile()
{
	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
	if (compiled != GL_TRUE)
	{
		int log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(id, 1024, &log_length, message);
		std::cout << "COULDN'T COMPILE FRAGMENT SHADER!\n" << message << std::endl;
	}

	return compiled;
}

unsigned int fragmentShader::get_id()
{
	return id;
}

/*
======================================================================
							SHADER CLASS
======================================================================
*/

Shader::Shader(const std::string& filepath)
{
	m_Filepath = filepath.c_str();

	std::ifstream infile(m_Filepath);
	if(!infile.is_open()) {
		std::cout << "COULDN'T FIND FILE " << filepath << std::endl;
		ERR_EXIT("");
	}
	std::string line;
	std::getline(infile, line);

	std::string vs_ = "", fs_ = "";
	while (std::getline(infile, line) && line.compare("#fragment shader") != 0) { vs_ += line + "\n"; }
	while (std::getline(infile, line)) { fs_ += line + "\n"; }
	m_ProgramID = glCreateProgram();
	vSource = vertexShader(vs_);
	fSource = fragmentShader(fs_);

	vSource.compile();
	fSource.compile();

	glAttachShader(m_ProgramID, vSource.get_id());
	glAttachShader(m_ProgramID, fSource.get_id());

	bound[0] = 1;
	bound[1] = 1;
}

Shader::Shader() {
	m_Filepath = "src/Abstract/Shaders/default.shader";

	std::ifstream infile(m_Filepath);
	std::string line;
	std::getline(infile, line);

	std::string vs_ = "", fs_ = "";
	while (std::getline(infile, line) && line.compare("#fragment shader") != 0) { vs_ += line + "\n"; }
	while (std::getline(infile, line)) { fs_ += line + "\n"; }

	m_ProgramID = glCreateProgram();
	vSource = vertexShader(vs_);
	fSource = fragmentShader(fs_);

	vSource.compile();
	fSource.compile();

	glAttachShader(m_ProgramID, vSource.get_id());
	glAttachShader(m_ProgramID, fSource.get_id());

	bound[0] = 1;
	bound[1] = 1;
}

int Shader::linkAndUse()
{
	int linked;
	glLinkProgram(m_ProgramID);
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		int log_length = 0;
		char message[1024];
		glGetProgramInfoLog(m_ProgramID, 1024, &log_length, message);
		std::cout << "COUDLN'T LINK SHADER PROGRAM!\n" << message << std::endl;
	}
	else {
		// compilation successful [shaders already attached]. can delete them now.
		glDeleteShader(vSource.get_id());
		glDeleteShader(fSource.get_id());
		glUseProgram(m_ProgramID);
	}
	return linked;
}

int Shader::link()
{
	int linked;
	glLinkProgram(m_ProgramID);
	glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		int log_length = 0;
		char message[1024];
		glGetProgramInfoLog(m_ProgramID, 1024, &log_length, message);
		std::cout << "COUDLN'T LINK SHADER PROGRAM!\n" << message << std::endl;
	}
	else
	{
		// compilation successful [shaders already attached]. can delete them now.
		glDeleteShader(vSource.get_id());
		glDeleteShader(fSource.get_id());
	}
	return linked;
}

void Shader::use()
{
	glUseProgram(m_ProgramID);
}

void Shader::print_sources() {
	std::cout
		<< "\nShader compiling status:"
		<< "\nVertex Shader: " << vSource.compiled
		<< "\nFragment Shader: " << fSource.compiled
		<< "\n";

	std::cout << "printing BOUND SHADER sources:";
	std::cout << vSource.vert_source;
	if (bound[0])
		std::cout << vSource.vert_source << "\n";
	if (bound[1])
		std::cout << fSource.frag_source << "\n";
}

int  Shader::getUniformLocation(const std::string& name) { return glGetUniformLocation(m_ProgramID, name.c_str()); }
void Shader::setUniform1f(const std::string& name, float f) { glUniform1f(getUniformLocation(name), f); }
void Shader::setUniform1i(const std::string& name, int i) { glUniform1i(getUniformLocation(name), i); }
void Shader::setUniform1ui(const std::string& name, unsigned int ui) { glUniform1ui(getUniformLocation(name), ui); }
void Shader::setUniform3ui(const std::string& name, vec3& v) { glUniform3ui(getUniformLocation(name), v.x, v.y, v.z); }
void Shader::setUniform3f(const std::string& name, vec3& v) { glUniform3f(getUniformLocation(name), v.x, v.y, v.z); }
void Shader::setUniform4f(const std::string& name, vec4& v) { glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w); }
void Shader::setUniformMat4(const std::string& name, mat4& matrix) { glUniformMatrix4fv(getUniformLocation(name), 1, false, matrix.get_c()); }
void Shader::setUniformMat4(const std::string& name, float* matrix) { glUniformMatrix4fv(getUniformLocation(name), 1, false, matrix); }

void Shader::setUniform3ui(const std::string& name, vec3&& v) { glUniform3ui(getUniformLocation(name), v.x, v.y, v.z); }
void Shader::setUniform3f(const std::string& name, vec3&& v) { glUniform3f(getUniformLocation(name), v.x, v.y, v.z); }
void Shader::setUniform4f(const std::string& name, vec4&& v) { glUniform4f(getUniformLocation(name), v.x, v.y, v.z, v.w); }
void Shader::setUniformMat4(const std::string& name, mat4&& matrix) { glUniformMatrix4fv(getUniformLocation(name), 1, false, matrix.get_c()); }
