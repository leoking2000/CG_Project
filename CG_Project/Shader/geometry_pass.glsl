#shader vertex
layout(location = 0) in vec3 apos;
layout(location = 1) in vec2 atex_cord;
layout(location = 2) in vec3 anormal;
layout(location = 3) in vec3 atangent;
layout(location = 4) in vec3 abitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 lightSpaceMatrix;

out vec2 tex_cord;
out vec3 world_pos; // camera space
out vec4 light_ndc; // light space
out mat3 TBN;

#version 330
void main()
{
    mat4 view_model = view * model;
    mat3 normalMatrix = transpose(inverse(mat3(view_model)));

    tex_cord    = atex_cord;

    world_pos   = (view_model * vec4(apos, 1.0) ).xyz;

    light_ndc = lightSpaceMatrix * model * vec4(apos, 1.0);

    gl_Position = proj * view_model * vec4(apos, 1.0);

    vec3 T = normalize(normalMatrix * atangent);
    vec3 B = normalize(normalMatrix * abitangent);
    vec3 N = normalize(normalMatrix * anormal);
    TBN = mat3(T, B, N);
}

#shader geometry
#version 330

void main()
{

}

#shader fragment
#version 330

void main()
{

}