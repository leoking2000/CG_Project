#shader vertex
#version 330

layout(location = 0) in vec3 apos;
layout(location = 1) in vec2 atex_cord;
layout(location = 2) in vec3 anormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform vec3 u_light_pos; // in world space
uniform mat4 lightSpaceMatrix;

out vec2 tex_cord;
out vec3 normal;    // camera space
out vec3 world_pos; // camera space
out vec4 world_pos_l; // light space
out vec3 light_pos; // camera space

void main()
{
    mat4 view_model = view * model;

    tex_cord    = atex_cord;

    normal      = (view_model * vec4(anormal, 0.0)).xyz;

    world_pos   = (view_model * vec4(apos, 1.0) ).xyz;

    world_pos_l = lightSpaceMatrix * model * vec4(apos, 1.0);

    light_pos   = (view * vec4(u_light_pos, 1.0) ).xyz;

    gl_Position = proj * view_model * vec4(apos, 1.0);
}

#shader fragment
#version 330

in vec2 tex_cord;
in vec3 normal;
in vec3 world_pos;
in vec4 world_pos_l; // light space
in vec3 light_pos;

uniform sampler2D shadowMap;

struct Material 
{
    float ambient;
    sampler2D texture_diffuse;
    sampler2D texture_specular;
    float shininess;
};

struct PointLight
{
    vec3 light_color;
    float quadratic_term;
    float linear_term;
};

uniform Material mat;
uniform PointLight light;

out vec4 out_color;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    //if(projCoords.z > 1.0) return 0.0;

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = (currentDepth - 0.00005) > closestDepth  ? 1.0 : 0.0;

    return shadow;
} 

vec4 point_light()
{

    float shadow = ShadowCalculation(world_pos_l);

    vec3 diffuse_color = texture(mat.texture_diffuse, tex_cord).xyz;

    if(shadow == 1.0)
    {
        return vec4(mat.ambient * diffuse_color, 1.0);
    }

    vec3 specular_color = texture(mat.texture_specular, tex_cord).xyz;

    vec3 lightVec = light_pos - world_pos;
    vec3 lightDir = normalize(lightVec);
    float dist = length(lightVec);
    float inten = 1.0 / ( light.quadratic_term * dist * dist + light.linear_term * dist + 1.0);

    vec3 surf_normal = normalize(normal);
    float diff_power = max(0.0, dot(surf_normal, lightDir));
    vec3 diffuse = diffuse_color * (diff_power + mat.ambient);

    vec3 viewDir = normalize(-world_pos);
    vec3 reflectDir = reflect(-lightDir, surf_normal);
    float spec_power = pow( max(0.0, dot(viewDir, reflectDir)), mat.shininess);
    vec3 specular = specular_color * spec_power * 0.5;

    return vec4((diffuse + specular) * inten * light.light_color, 1.0);
}

void main()
{
    out_color = point_light();
}
