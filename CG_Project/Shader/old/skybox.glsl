#shader vertex
#version 330
layout (location = 0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    texCoords = aPos;
    vec4 pos = proj * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}

#shader fragment
#version 330

in vec3 texCoords;
uniform samplerCube skybox;

out vec4 out_color;

void main()
{    
    out_color = texture(skybox, texCoords);
}
