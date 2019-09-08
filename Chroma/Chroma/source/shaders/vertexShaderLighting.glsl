#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoords;

// matrix uniforms
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos; // this is the world position of the fragment
out vec2 TexCoords;
void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   FragPos = vec3(model * vec4(aPos, 1.0));
   // we transform the normal by the normal matrix which ideally should be done on the CPU and passed to the GPU
   Normal = mat3(transpose(inverse(model))) * aNorm;
   TexCoords = aTexCoords;
}
