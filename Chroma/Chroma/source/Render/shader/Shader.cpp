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
	CheckCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");
	// if geometry shader is given, compile geometry shader
	unsigned int geometry;
	if (geometrySourcePath != "")
	{
		const char* gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		CheckCompileErrors(geometry, "GEOMETRY");
	}
	// shader Program
	glAttachShader(ShaderID, vertex);
	glAttachShader(ShaderID, fragment);
	if (geometrySourcePath != "")
		glAttachShader(ShaderID, geometry);
	glLinkProgram(ShaderID);
	CheckCompileErrors(ShaderID, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometrySourcePath != "")
		glDeleteShader(geometry);

}

std::string Shader::ExpandShaderSource(std::string shaderSourcePath)
{
	//  retrieve the vertex/fragment source code from filePath
	std::ifstream shaderFileStream;
	// create empty string stream to collect expanded shader strings
	std::stringstream shaderstringstream;
	// open file_
	shaderFileStream.open(shaderSourcePath);

	while (shaderFileStream)
	{
		std::string line;
		std::getline(shaderFileStream, line);

		// replace versions
		if (line.find("#version") != std::string::npos)
			if (line.find("_OPENGL_VERSION_") != std::string::npos) // checking for opengl version override
				line = "#version " + OPENGL_VERSION;

		// expand includes with included files
		if (line.find("#include") != std::string::npos)
			{
				// fetching path of include
				int start = line.find('"') + 1;
				int end = line.rfind('"');
				std::string includeSourcePath = shaderDir + line.substr(start, end - start);
				// creating include source file
				std::fstream includeFile;
				includeFile.open(includeSourcePath);
				std::stringstream includeSource;
				includeSource << includeFile.rdbuf();
				includeFile.close();
				// replacing line with exanded include
				line = includeSource.str();
			}
		shaderstringstream << line << std::endl;
	}
	shaderFileStream.close();

	return shaderstringstream.str();
}


void Shader::LoadShaderSource()
{
	// Load and Expand Shader Source
	vertexCode = ExpandShaderSource(vertexSourcePath);
	fragCode = ExpandShaderSource(fragSourcePath);

	// if geometry shader path is present, also load a geometry shader
	if (geometrySourcePath != "")
	{
		geometryCode = ExpandShaderSource(geometrySourcePath);
	}
}

Shader::Shader(std::string fragmentPath, std::string vertexPath, std::string geometryPath)
{
	// init
	ShaderID = glCreateProgram();
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
	ShaderID = glCreateProgram();
}

void Shader::use() const
{
	glUseProgram(ShaderID);
}


void Shader::setLightingUniforms(std::vector<Light*> Lights, Camera& renderCam)
{
	int pointlights{ 0 };
	int dirlights{ 0 };
	int spotlights{ 0 };
	for (int i = 0; i < Lights.size(); i++)
	{
		std::string lightIndex;
		// set uniforms
		switch (Lights[i]->type) {
		case Light::POINT:
			pointlights++;
			lightIndex = "pointLights[" + std::to_string(pointlights - 1) + "]";
			//// lights point light falloff
			this->setFloat(lightIndex + ".constant", Lights[i]->constant);
			this->setFloat(lightIndex + ".linear", Lights[i]->linear);
			this->setFloat(lightIndex + ".quadratic", Lights[i]->quadratic);
			this->setFloat(lightIndex + ".radius", Lights[i]->getRadius());
			break;
		case Light::SUNLIGHT:
		case Light::DIRECTIONAL:
			dirlights++;
			lightIndex = "dirLights[" + std::to_string(dirlights - 1) + "]";
			//// lights directional
			this->setVec3(lightIndex + ".direction", Lights[i]->getDirection());
			break;
		case Light::SPOT:
			spotlights++;
			lightIndex = "spotLights[" + std::to_string(spotlights - 1) + "]";
			//// lights spotlight
			this->setFloat(lightIndex + ".spotSize", Lights[i]->getSpotSize());
			this->setFloat(lightIndex + ".penumbraSize", Lights[i]->getPenumbraSize());
			break;
		default:
			break;
		}
		// lights all
		this->setFloat(lightIndex + ".intensity", Lights[i]->getIntensity());
		this->setVec3(lightIndex + ".diffuse", Lights[i]->getDiffuse());
		this->setVec3(lightIndex + ".position", Lights[i]->getPosition());
		// lights view pos
		this->setVec3("viewPos", renderCam.getPosition());
	}
}

void Shader::CheckCompileErrors(GLuint shader, std::string type)
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



void Shader::setBool(const std::string& name, bool value) const
{
	Uniforms.setBool(name, value);
}

void Shader::setInt(const std::string& name, int value) const
{
	Uniforms.setInt(name, value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	Uniforms.setFloat(name, value);
}

void Shader::setVec2(const std::string& name, glm::vec2 value) const
{
	Uniforms.setVec2(name, value);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	Uniforms.setVec3(name, value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
	Uniforms.setMat4(name, value);
}

void Shader::setUniforms()
{
	Uniforms.setUniforms();
}

