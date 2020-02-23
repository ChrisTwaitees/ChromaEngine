#include "Shader.h"
#include <scene/Scene.h>

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

		// replace MAX_VERT_INFLUENCES
		if (line.find("#MAX_VERT_INFLUENCES") != std::string::npos)
			Replace(line, "#MAX_VERT_INFLUENCES", std::to_string(MAX_VERT_INFLUENCES));

		// replace MAX JOINTS
		if (line.find("#MAX_JOINTS") != std::string::npos)
			Replace(line, "#MAX_JOINTS", std::to_string(MAX_JOINTS));

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

void Shader::Replace(std::string& sourceString, std::string const& from, std::string const& to)
{
	size_t start_pos = sourceString.find(from);
	sourceString.replace(start_pos, from.length(), to);
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

void Shader::Use() const
{
	glUseProgram(ShaderID);
}


void Shader::SetLightingUniforms(Camera const& renderCam)
{
	int pointlights{ 0 };
	int dirlights{ 0 };
	int spotlights{ 0 };

	for (UID const& lightUID : Chroma::Scene::GetLightUIDs())
	{
		Light* light = static_cast<Light*>(Chroma::Scene::GetComponent(lightUID));
		std::string lightIndex;
		// set uniforms
		switch (light->type) {
		case Light::POINT:
			pointlights++;
			lightIndex = "pointLights[" + std::to_string(pointlights - 1) + "]";
			//// lights point light falloff
			this->SetFloat(lightIndex + ".constant", light->constant);
			this->SetFloat(lightIndex + ".linear", light->linear);
			this->SetFloat(lightIndex + ".quadratic", light->quadratic);
			this->SetFloat(lightIndex + ".radius", light->getRadius());
			break;
		case Light::SUNLIGHT:
			dirlights++;
			lightIndex = "dirLights[" + std::to_string(dirlights - 1) + "]";
			//// lights directional
			this->SetVec3(lightIndex + ".direction", light->GetDirection());
			break;
		case Light::DIRECTIONAL:
			dirlights++;
			lightIndex = "dirLights[" + std::to_string(dirlights - 1) + "]";
			//// lights directional
			this->SetVec3(lightIndex + ".direction", light->GetDirection());
			break;
		case Light::SPOT:
			spotlights++;
			lightIndex = "spotLights[" + std::to_string(spotlights - 1) + "]";
			//// lights spotlight
			this->SetFloat(lightIndex + ".spotSize", light->getSpotSize());
			this->SetFloat(lightIndex + ".penumbraSize", light->getPenumbraSize());
			break;
		default:
			break;
		}
		// lights all
		this->SetFloat(lightIndex + ".intensity", light->getIntensity());
		this->SetVec3(lightIndex + ".diffuse", light->getDiffuse());
		this->SetVec3(lightIndex + ".position", light->GetPosition());
		// lights view pos
		this->SetVec3("viewPos", renderCam.GetPosition());

//		delete light;
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
			CHROMA_WARN("ERROR::SHADER : COMPILATION_ERROR of type: {0} \n {1}", type, infoLog);
			CHROMA_WARN("SHADER FRAG SOURCE : {0}", fragSourcePath);
			CHROMA_WARN("SHADER VTX SOURCE : {0}", vertexSourcePath);
			CHROMA_WARN("SHADER GEOM SOURCE  : {0}", geometrySourcePath);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			CHROMA_WARN("ERROR::SHADER : COMPILATION_ERROR of type: {0} \n {1}", type, infoLog);
			CHROMA_WARN("SHADER FRAG SOURCE : {0}", fragSourcePath);
			CHROMA_WARN("SHADER VTX SOURCE : {0}", vertexSourcePath);
			CHROMA_WARN("SHADER GEOM SOURCE  : {0}", geometrySourcePath);
		}
	}
}



void Shader::SetBool(const std::string& name, bool value) const
{
	Uniforms.SetBool(name, value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	Uniforms.SetInt(name, value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	Uniforms.SetFloat(name, value);
}

void Shader::SetVec2(const std::string& name, glm::vec2 value) const
{
	Uniforms.setVec2(name, value);
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) const
{
	Uniforms.setVec3(name, value);
}

void Shader::SetMat4(const std::string& name, glm::mat4 value) const
{
	Uniforms.SetMat4(name, value);
}

void Shader::setUniforms()
{
	Uniforms.setUniforms();
}

