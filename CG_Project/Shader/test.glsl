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
in vec4 light_ndc;
in mat3 TBN;

uniform vec4 lightDir; // camera space

uniform sampler2D shadowMap;
uniform float bias;

uniform sampler2D BaseMap;
uniform sampler2D MaskMap;
uniform sampler2D NormalMap;

uniform sampler2D Lightmap;
uniform int Has_Lightmap;

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


vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}


vec3 cook_torrance(vec3 frag_to_light, vec3 normal, vec3 frag_to_view)
{
    vec4 mask = texture(MaskMap, tex_cord);

    vec3 albedo = texture(BaseMap, tex_cord).rgb;
    float metallic = mask.r;
    float roughness = 1 - mask.a;
    float ao = mask.g;

    vec3 halfVector = normalize( frag_to_light + frag_to_view );

    vec3 radiance = vec3(1.0);

    vec3 F0 = vec3(0.04); 
    F0      = mix(F0, albedo, metallic);
    vec3 F  = fresnelSchlick(max(dot(halfVector, frag_to_view), 0.0), F0);

    float NDF = DistributionGGX(normal, halfVector, roughness);
    float G   = GeometrySmith(normal, frag_to_view, frag_to_light, roughness);

    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, frag_to_view), 0.0) * max(dot(normal, frag_to_light), 0.0)  + 0.0001;
    vec3 specular     = numerator / denominator; 

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
  
    kD *= 1.0 - metallic;

    float NdotL = max(dot(normal, frag_to_light), 0.0);
    vec3 color;

    if(Has_Lightmap)
    {
        color = (kD * albedo / PI + texture(Lightmap, tex_cord).rgb + specular) * radiance * NdotL;
    }
    else
    {
        color = (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.3) * albedo * ao;

    return ambient + color * shadow();
}


void main()
{
    vec3 color = cook_torrance(-vec3(lightDir), GetNormal(), -world_pos);

    out_color = vec4(color, 1.0);
}