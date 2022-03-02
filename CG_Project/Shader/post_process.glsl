#shader vertex
#version 330 core
layout(location = 0) in vec2 coord2d;

out vec2 uv;

void main(void)
{
    gl_Position = vec4(coord2d, 0.0, 1.0);

    uv = (coord2d + 1.0) / 2.0;
}

#shader fragment
#version 330 core

in vec2 uv;

uniform sampler2D uniform_texture;
uniform float uniform_time;

out vec4 out_color;

void main(void)
{
    out_color = texture(uniform_texture, uv);
}
