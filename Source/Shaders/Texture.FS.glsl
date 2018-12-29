#version 330 core

layout(location = 0) out vec4 out_color;
  
in vec2 texCoord;

in vec3 world_position;
in vec3 world_normal;

// uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform sampler2D textureImg0;

void main()
{
    out_color = texture(textureImg0, texCoord);

    if (out_color.a < 0.3f) { // alpha-discard
	    discard;
    }
}