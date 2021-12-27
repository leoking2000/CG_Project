#shader vertex
#version 330
layout (location = 0) in vec3 aPos;

out vec3 pos;

uniform mat4 proj;
uniform mat4 view;

void main()
{
    pos = aPos;
    gl_Position = proj * view * vec4(aPos, 1.0);
}

#shader fragment
#version 330

#define PI 3.14159

in vec3 pos;
uniform sampler2D skybox;

out vec4 out_color;

void main()
{
    float angle_1 = atan(pos.x / pos.z);
    float angle_2 = atan(pos.y / (sqrt(pos.x * pos.x + pos.z * pos.z)));

    float u = (angle_1 + PI) / (2 * PI);
    float v = (angle_2 + PI / 2.0) / PI;

    vec2 tex_coords = vec2(u, v);
    //tex_coords = (tex_coords + 1.0) * 1.01;

    out_color = texture(skybox, tex_coords);
}
