#shader vertex
#version 330
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 atex_cord;

out vec2 tex_cord;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    tex_cord = atex_cord;
    gl_Position = proj * view * vec4(aPos, 1.0);
}

#shader fragment
#version 330

#define PI 3.14159

in vec2 tex_cord;
uniform sampler2D skybox;

out vec4 out_color;

void main()
{
    out_color = texture(skybox, tex_cord);
}
