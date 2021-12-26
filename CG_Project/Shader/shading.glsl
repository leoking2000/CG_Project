#shader vertex
#version 330

layout(location = 0) in vec3 apos;
layout(location = 1) in vec2 atex_cord;
layout(location = 2) in vec3 anormal;
layout(location = 3) in vec3 atangent;
layout(location = 4) in vec3 abitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 light_dir; // in world space
uniform mat4 lightSpaceMatrix;

out vec2 tex_cord;
out vec3 world_pos; // camera space
out vec4 world_pos_l; // light space
out mat3 TBN;

void main()
{
    mat4 view_model = view * model;
    mat3 normalMatrix = transpose(inverse(mat3(view_model)));

    tex_cord    = atex_cord;

    world_pos   = (view_model * vec4(apos, 1.0) ).xyz;

    world_pos_l = lightSpaceMatrix * model * vec4(apos, 1.0);

    gl_Position = proj * view_model * vec4(apos, 1.0);

    vec3 T = normalize(normalMatrix * atangent);
    vec3 B = normalize(normalMatrix * abitangent);
    vec3 N = normalize(normalMatrix * anormal);
    TBN = mat3(T, B, N);
}

#shader fragment
#version 330

#define PI 3.14159

in vec2 tex_cord;
in vec3 world_pos; // camera space
in vec4 world_pos_l; // light space
in mat3 TBN;

uniform vec4 lightDir; // camera space

uniform sampler2D shadowMap;
uniform float bias;

uniform sampler2D BaseMap;
uniform sampler2D MaskMap;
uniform sampler2D NormalMap;

out vec4 out_color;

vec3 GetNormal()
{
    vec3 nmap = texture(NormalMap, tex_cord).rgb;
    nmap = nmap * 2.0 - 1.0;
    vec3 normal = normalize(TBN * nmap);

    return normal;
}

float ShadowCalculation()
{
    // perform perspective divide
    vec3 projCoords = world_pos_l.xyz / world_pos_l.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    //if(projCoords.z > 1.0) return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = (currentDepth - bias) < closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 blinn_phong()
{
    vec4 mask = texture(MaskMap, tex_cord);
    vec3 normal = GetNormal();

    vec3 frag_to_light = -vec3(lightDir);

    float shininess = 200.0;

    vec3 halfVector = normalize( (-world_pos) + frag_to_light );
    float NdotL = max(dot(normal, frag_to_light), 0.0);
    float NdotH = max(dot(normal, halfVector), 0.0);

    vec3 albedo = texture(BaseMap, tex_cord).rgb;
    vec3 ambient = vec3(0.1) * albedo;
    vec3 diffuse = (albedo) * NdotL;

    vec3 specular = vec3(mask.r) * pow(NdotH, shininess);

    float Shadow = ShadowCalculation();

    return (diffuse + specular) * Shadow + ambient;
}

void main()
{
    out_color = vec4(blinn_phong(),1.0);
}