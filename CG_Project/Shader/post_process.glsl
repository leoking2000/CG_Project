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

float near_plane = 0.1;
float far_plane = 1000;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main(void)
{
    vec3 color = texture(uniform_texture, uv).rgb;

    out_color = vec4(color, 1.0);
    //out_color = vec4(vec3(LinearizeDepth(color.r) / far_plane), 1.0);
}
