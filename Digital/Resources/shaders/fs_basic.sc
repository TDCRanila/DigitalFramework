$input v_texcoord0
$input v_color0

#include "bgfx_shaderlib.sh"

SAMPLER2D(s_texture, 0);

void main()
{
	 gl_FragColor = texture2D(s_texture, v_texcoord0.xy);
}
