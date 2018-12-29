#version 330

layout(location = 0) out vec4 out_color;
  
in vec2 texCoord;

in vec3 world_position;
in vec3 world_normal;

// uniforms for light properties
uniform vec3 sun_pos;
uniform vec3 light1_pos, light2_pos, light3_pos;
uniform vec3 light1_dir, light2_dir, light3_dir;
uniform float spot_angle;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform sampler2D textureImg0;

float GetSunLight() 
{
    // compute world space vectors
	vec3 L = normalize(sun_pos - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	// define ambient light component
	float ambient_light = 0.6;
	float lightColor = 0.5;

	// compute diffuse light component
	float getLight = (dot(world_normal, L) > 0) ? 1 : 0;
	float diffuse_light = 700 * material_kd * lightColor * max(dot(world_normal, L), 0);	

	// compute specular light component
	float specular_light = material_ks * lightColor * getLight * pow(max(dot(world_normal, H), 0), material_shininess);

	float d = distance(sun_pos, world_position);
	float attenuation =  1 / (d * d);

	// compute light
	return ambient_light + attenuation * (diffuse_light + specular_light);
}

vec3 ColorWithSpotlight(vec3 color, vec3 light_pos, vec3 light_dir)
{
    // compute world space vectors
	vec3 L = normalize(light_pos - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);

	// define ambient light component
	float ambient_light = 0.25;
	float lightColor = 1;

	// compute diffuse light component
	float getLight = (dot(world_normal, L) > 0) ? 1 : 0;
	float diffuse_light = material_kd * lightColor * max(dot(world_normal, L), 0);	

	// compute specular light component
	float specular_light = material_ks * lightColor * getLight * pow(max(dot(world_normal, H), 0), material_shininess);

	float d = distance(light_pos, world_position);
	float attenuation =  1 / (d * d);

	// compute light from spot
	float light = ambient_light + attenuation * (diffuse_light + specular_light);

	float spot_light = dot(-L, light_dir);
	float spot_light_limit = cos(radians(spot_angle));
 
	// quadratic attenuation
	float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	float light_att_factor = pow(linear_att, 2);

	if (spot_light > spot_light_limit) // check the cutoff angle
		return color * (light * light_att_factor);
	else
		return color * 0;
}

void main()
{
    vec4 color_alpha = texture(textureImg0, texCoord);
    vec3 color = color_alpha.rgb; // real object's color
    vec3 color_lit = vec3(0);

    // add spotlights
    color_lit += ColorWithSpotlight(color, light1_pos, light1_dir);
    color_lit += ColorWithSpotlight(color, light2_pos, light2_dir);
    color_lit += ColorWithSpotlight(color, light3_pos, light3_dir);

    // add sunlight
    color_lit += color * GetSunLight();

    out_color = vec4(color_lit, color_alpha.a);
}