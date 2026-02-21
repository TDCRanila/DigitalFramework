$input v_texcoord0, v_color0

#include "bgfx_shaderlib.sh"

SAMPLER2D(s_texture, 0);

void main()
{
	vec4 colour = texture2D(s_texture, v_texcoord0.xy);

	if (colour.a < 0.5)
		discard;

	gl_FragColor = colour;
}
