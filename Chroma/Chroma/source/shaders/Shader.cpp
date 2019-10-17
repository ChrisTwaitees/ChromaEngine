#include "Shader.h"


void Shader::CompileAndLink()
{
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (geometrySourcePath != "")
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	id = glCreateProgram();
	glAttachShader(id, vertex);
	glAttachShader(id, fragment);
	if (geometrySourcePath != "")
		glAttachShader(id, geometry);
	glLinkProgram(id);
	checkCompileErrors(id, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometrySourcePath != "")
		glDeleteShader(geometry);

}

void Shader::LoadShaderSource()
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexSourcePath);
		fShaderFile.open(fragSourcePath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragCode = fShaderStream.str();
		// if geometry shader path is present, also load a geometry shader
		if (geometrySourcePath != "")
		{
			gShaderFile.open(geometrySourcePath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

Shader::Shader(std::string fragmentPath, std::string vertexPath, std::string geometryPath)
{
	// init
	fragSourcePath = fragmentPath;
	vertexSourcePath = vertexPath;
	geometrySourcePath = geometryPath;

	// load shader source files
	LoadShaderSource();

	// compile and link shaders
	CompileAndLink();
}


Shader::~Shader()
{
}

void Shader::use()
{
	glUseProgram(id);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const
{
	glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
}

void Shader::setVec3(const std::string& name, glm::vec3 vec3) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), vec3.x, vec3.y, vec3.z);
}

void Shader::setMat4(const std::string& name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,  glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(GLuint shader, std::string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}