#shader vertex
#version 330
layout (location = 0) in vec2 aPos;
layout(location = 1) in vec2 atex_cord;

uniform mat4 proj;

out vec2 tex_cord;

void main()
{
    tex_cord = atex_cord;
    gl_Position = proj * vec4(aPos, 0.0, 1.0);
}

#shader fragment
#version 330

in vec2 tex_cord;
uniform sampler2D font;
uniform vec3 color;

out vec4 out_color;

void main()
{
    if(texture(font, tex_cord).r == 1.0)
    {
        discard;
    }

    out_color = vec4(color, 1.0);
}