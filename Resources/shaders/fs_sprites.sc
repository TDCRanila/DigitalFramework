$input v_texcoord0, v_color0

#include "bgfx_shaderlib.sh"

SAMPLER2D(s_texture, 0);

void main()
{
	vec4 texture_colour = texture2D(s_texture, v_texcoord0.xy);

	if (texture_colour.a < 0.5)
		discard;

	gl_FragColor = v_color0 * texture_colour;
}
