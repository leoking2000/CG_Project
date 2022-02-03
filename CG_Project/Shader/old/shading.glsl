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
out vec4 light_ndc; // light space
out mat3 TBN;

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

#shader fragment
#version 330

#define PI 3.14159

in vec2 tex_cord;
in vec3 world_pos; // camera space
in mat3 TBN;

in vec4 light_ndc;

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

float shadow_pcf2x2_weighted(vec3 light_space_xyz)
{
	ivec2 shadow_map_size = textureSize(shadowMap, 0);
	float xOffset = 1.0 / shadow_map_size.x;
    float yOffset = 1.0 / shadow_map_size.y;

	// compute the weights of the neighboring pixels
	vec2 uv = light_space_xyz.xy - vec2(xOffset, yOffset);
	float u_ratio = mod(uv.x, xOffset) / xOffset;
	float v_ratio = mod(uv.y, yOffset) / yOffset;
	float u_opposite = 1 - u_ratio;
	float v_opposite = 1 - v_ratio;

	// compute the distance with a small bias
	float z = light_space_xyz.z - bias;

	// compute the shadow percentage
	float bottomLeft  = (texture(shadowMap, uv).r > z)                         ? u_opposite : 0.0;
	float bottomRight = (texture(shadowMap, uv + vec2(xOffset, 0)).r > z)      ? u_ratio : 0.0; 
	float topLeft     = (texture(shadowMap, uv + vec2(0, yOffset), 0).r > z)   ? u_opposite : 0.0;
	float topRight    =  texture(shadowMap, uv + vec2(xOffset, yOffset)).r > z ? u_ratio : 0.0;

	float factor      = (bottomLeft + bottomRight) * v_opposite + (topLeft + topRight) * v_ratio;
    return factor;
}

// 1 sample per pixel
float shadow()
{
	// perspective division
	vec3 ndc = light_ndc.xyz / light_ndc.w; // normalize device cordince
	// transform to [0,1] range
    ndc = ndc * 0.5 + 0.5;

	// check that we are inside light clipping frustum
	if (ndc.x < 0.0) return 0.0;
	if (ndc.y < 0.0) return 0.0;
	if (ndc.x > 1.0) return 0.0;
	if (ndc.y > 1.0) return 0.0;
    if (ndc.z < 0.0) return 0.0;
    if (ndc.z > 1.0) return 0.0;

	// sample shadow map
	return shadow_pcf2x2_weighted(ndc);
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

    vec3 specular = vec3(mask.a) * pow(NdotH, shininess);

    return (diffuse + specular) * shadow() + ambient;
}

void main()
{
    out_color = vec4(blinn_phong(),1.0);
}