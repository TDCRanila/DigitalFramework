vec3 v_texcoord0    : TEXCOORD0 = vec3(0.0, 0.0, 0.0);
vec4 v_color0       : COLOR0    = vec4(1.0, 1.0, 1.0, 1.0);

vec3 a_position  : POSITION;
vec3 a_texcoord0 : TEXCOORD0;
vec4 a_color0    : COLOR0;

vec4 i_data0     : TEXCOORD7; // SpriteInstance - TransformMatrix.x
vec4 i_data1     : TEXCOORD6; // SpriteInstance - TransformMatrix.y
vec4 i_data2     : TEXCOORD5; // SpriteInstance - TransformMatrix.z
vec4 i_data3     : TEXCOORD4; // SpriteInstance - TransformMatrix.z
vec4 i_data4     : TEXCOORD3; // SpriteInstance - Colour
