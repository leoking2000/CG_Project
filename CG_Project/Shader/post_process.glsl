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

//float near_plane = 0.1;
//float far_plane = 1000;
//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // Back to NDC 
//    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
//}

vec2 curveRemapUV(vec2 uv)
{
    uv = uv * 2.0 - 1.0;
    vec2 offset = abs(uv.yx) / vec2(5.0, 5.0);
    uv = uv + uv * offset * offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

void main(void)
{
    vec2 remappedUV = curveRemapUV(uv);
    vec3 color = texture(uniform_texture, remappedUV).rgb;

    float dist = 1.0 - mod(0.3 * uniform_time, 1.0) - remappedUV.y;
    dist = dist * dist;
    float time = exp(1 - 200 * dist) / 2.8;
    float power = time;
    power *= 0.1;
    float red = texture(uniform_texture, remappedUV + power * vec2(-0.1, 0)).r;
    float green = texture(uniform_texture, remappedUV + power * vec2(-0.05, 0)).g;
    float blue = texture(uniform_texture, remappedUV + power * vec2(-0.025, 0)).b;
    color = vec3(red, green, blue);

    if (remappedUV.x < 0.0 || remappedUV.y < 0.0 || remappedUV.x > 1.0 || remappedUV.y > 1.0)
    {
        out_color = vec4(0.0, 0.0, 0.0, 1.0);
    } 
    else 
    {
        out_color = vec4(color, 1.0);
    }

    //out_color = vec4(vec3(LinearizeDepth(color.r) / far_plane), 1.0);
}
