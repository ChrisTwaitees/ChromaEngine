#include "Shader.h"

const char* Shader::ReadShaderSource(const GLchar*& shaderSourcePath) const
{
	std::ifstream ShaderFileStream;
	std::string ShaderSource;
	// ensure ifstream objects can throw exceptions
	ShaderFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//open files using file handlers
		ShaderFileStream.open(shaderSourcePath);
		std::stringstream ShaderStream;
		// read the files buffer contents into streams
		ShaderStream << ShaderFileStream.rdbuf();
		//close file handlers
		ShaderFileStream.close();
		ShaderFileStream.clear();
		//convert stream into string
		ShaderSource = ShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	// shader source in c-style strings
	return ShaderSource.c_str();
}

Shader::Shader(const GLchar* fragmentSourcePath, const GLchar* vertexSourcePath)
{
	// retrieve the vertex/fragment source code from filepath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		//open files using file handlers
		vShaderFile.open(vertexSourcePath);
		fShaderFile.open(fragmentSourcePath);
		std::stringstream vShaderStream, fShaderStream;
		// read the files buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		//close file handlers
		vShaderFile.close();
		fShaderFile.close();
		//convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
	}
	// shader source in c-style strings
	const char* vShaderSourceCode = vertexCode.c_str();
	// TODO: Abstract reading file streams as c strings
	//const char* vShaderSourceCode = ReadShaderSource(vertexSourcePath);

	const char* fShaderSourceCode = fragmentCode.c_str();

	// 2. Compile and link the shaders
	int success;
	char infoLog[512];
	// vx shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderSourceCode, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::VERTEX SHADER::COMPILE_FAILURE" << infoLog << std::endl;
	}

	// px shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderSourceCode, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::FRAGMENT SHADER::COMPILE_FAILURE" << infoLog << std::endl;
	}

	//program
	id = glCreateProgram();
	glAttachShader(id, vertexShader);
	glAttachShader(id, fragmentShader);
	glLinkProgram(id);
	//debug link
	glGetProgramiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
		std::cout << "ERROR::LINKING OF SHADERS FAILED" << infoLog << std::endl;
	}

	//clear shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader()
{
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

void Shader::setVec3(const std::string& name, glm::vec3 vec3) const
{
	glUniform3f(glGetUniformLocation(id, name.c_str()), vec3.x, vec3.y, vec3.z);
}

void Shader::setMat4(const std::string& name, glm::mat4 matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,  glm::value_ptr(matrix));
}

