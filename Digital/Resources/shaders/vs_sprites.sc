$input a_position, a_texcoord0, a_color0, i_data0, i_data1, i_data2, i_data3, i_data4
$output v_texcoord0, v_color0

#include "bgfx_shaderlib.sh"

void main()
{
	v_texcoord0 = a_texcoord0;
	v_color0 = a_color0 * i_data4;

	mat4 instanceMatrix = mtxFromCols(i_data0, i_data1, i_data2, i_data3);
	vec4 instancePosition = mul(instanceMatrix, vec4(a_position, 1.0));
	gl_Position = mul(u_viewProj, instancePosition);
}
