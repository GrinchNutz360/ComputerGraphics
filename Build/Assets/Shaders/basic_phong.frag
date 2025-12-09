#version 460 core
#define MAX_LIGHTS		5
#define POINT			0
#define DIRECTIONAL		1
#define SPOT			2

#define	BASE_MAP		 (1<<0)
#define	SPECULAR_MAP	 (1<<1)
#define	EMISSIVE_MAP	 (1<<2)
#define	NORMAL_MAP		 (1<<3)

in VS_OUT{
	in vec2 texcoord;
	in vec3 position;
	in vec3 normal;
	mat3 tbn;
}fs_in;

out vec4 f_color;

struct Light
{
	int type;
	vec3 position;
	vec3 color;
	vec3 direction;
	float range;
	float intensity;
	float outerSpotAngle;
	float innerSpotAngle;
};

uniform struct Material
{
	vec3 baseColor;
	vec3 emissiveColor;

	float shininess;
	vec2 tiling;
	vec2 offset;
	
	uint parameters;
};

uniform vec3 u_ambient_light;
uniform sampler2D u_texture;

uniform int u_numLights = 1;
uniform Light u_lights[MAX_LIGHTS];
uniform Material u_material;

uniform sampler2D u_baseMap;
uniform sampler2D u_specularMap;
uniform sampler2D u_emissiveMap;
uniform sampler2D u_normalMap;


float calculateAttenuation(in float light_distance, in float range)
{	
	float attenuation = max( 0.0, (1.0 - (light_distance / range)));
	return attenuation * attenuation;
} 

vec3 calculateLight(in Light light, in vec3 position,in vec3 normal, in float specularMask)
{
	vec3 light_dir;
	float attenuation;

    switch (light.type)
	{
		case POINT:
		{
			light_dir = normalize(light.position - position);

	float light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);
			break;
		}
		

		case DIRECTIONAL:
		{
			light_dir = normalize(light.direction);
			attenuation = 1.0;
			break;
		}

	case SPOT:
		{
			light_dir = normalize(light.position - position);

	float light_distance = length(light.position - position);
			attenuation = calculateAttenuation(light_distance, light.range);
			float angle = acos(dot(light_dir, light.direction));

			if (angle > light.outerSpotAngle){ attenuation = 0.0;}
			else 
			{
				float spotAttenuation = smoothstep(light.outerSpotAngle + 0.001, light.innerSpotAngle, angle);
				attenuation *= spotAttenuation;
			}
			break;
		}
	}
	
	//diffuse
	float nDotL = max(dot(normal,light_dir ), 0);
	vec3 diffuse = light.color * u_material.baseColor * nDotL;

	//specular
	vec3 reflection = reflect(-light_dir, normal);
	
	// blinn phong
	vec3 view_dir = normalize(-light.position - position);
	vec3 halfway_dir = normalize(light_dir + view_dir);
	float nDotH = max(dot(normal, halfway_dir), 0);
	nDotH = pow(nDotH, u_material.shininess);
	vec3 specular = vec3(nDotH) * specularMask;

	return (diffuse + specular) * light.intensity * attenuation;
}

vec3 calculateNormal()
{
	// generate the normals from the normal map
	vec3 normal = texture(u_normalMap, fs_in.texcoord).rgb;
	// convert rgb normal (0 <-> 1) to xyx (-1 <-> 1)
	normal = normalize(normal * 2.0 - 1.0);
	// transform normals to model view space
	normal = normalize(fs_in.tbn * normal);

	return normal;
}

void main()
{
	//vec3 color = calculateLight(fs_in.position, fs_in.normal);
	//f_color = texture(u_material.baseMap, fs_in.texcoord) * vec4(color, 1);

	vec3 normal = ((u_material.parameters & NORMAL_MAP) != 0u)
	? calculateNormal()
	: fs_in.normal;

	float specularMask = ((u_material.parameters & SPECULAR_MAP) != 0u)
		? texture(u_specularMap, fs_in.texcoord).r
		: 1;

	vec3 color = u_ambient_light;
	for (int i = 0; i < u_numLights; i++)
	{
		color += calculateLight(u_lights[i], fs_in.position, normal, specularMask);
	}

	vec4 emissive = ((u_material.parameters & EMISSIVE_MAP) != 0u)
		? texture(u_emissiveMap, fs_in.texcoord) * vec4(u_material.emissiveColor, 1)
		: vec4(u_material.emissiveColor, 1);

	f_color = texture(u_baseMap, fs_in.texcoord) * vec4(color, 1) + emissive;
	//f_color = vec4(u_material.emissiveColor, 1);
}