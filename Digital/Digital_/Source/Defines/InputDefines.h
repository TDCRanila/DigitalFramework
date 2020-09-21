#pragma once

#include <Utility/TemplateUtility.h>

#pragma region Define copy of GLFW Keys

#define GLFW_RELEASE                0
#define GLFW_PRESS                  1
#define GLFW_REPEAT                 2

#define GLFW_KEY_UNKNOWN            -1

#define GLFW_KEY_SPACE              32
#define GLFW_KEY_APOSTROPHE         39  /* ' */
#define GLFW_KEY_COMMA              44  /* , */
#define GLFW_KEY_MINUS              45  /* - */
#define GLFW_KEY_PERIOD             46  /* . */
#define GLFW_KEY_SLASH              47  /* / */
#define GLFW_KEY_0                  48
#define GLFW_KEY_1                  49
#define GLFW_KEY_2                  50
#define GLFW_KEY_3                  51
#define GLFW_KEY_4                  52
#define GLFW_KEY_5                  53
#define GLFW_KEY_6                  54
#define GLFW_KEY_7                  55
#define GLFW_KEY_8                  56
#define GLFW_KEY_9                  57
#define GLFW_KEY_SEMICOLON          59  /* ; */
#define GLFW_KEY_EQUAL              61  /* = */
#define GLFW_KEY_A                  65
#define GLFW_KEY_B                  66
#define GLFW_KEY_C                  67
#define GLFW_KEY_D                  68
#define GLFW_KEY_E                  69
#define GLFW_KEY_F                  70
#define GLFW_KEY_G                  71
#define GLFW_KEY_H                  72
#define GLFW_KEY_I                  73
#define GLFW_KEY_J                  74
#define GLFW_KEY_K                  75
#define GLFW_KEY_L                  76
#define GLFW_KEY_M                  77
#define GLFW_KEY_N                  78
#define GLFW_KEY_O                  79
#define GLFW_KEY_P                  80
#define GLFW_KEY_Q                  81
#define GLFW_KEY_R                  82
#define GLFW_KEY_S                  83
#define GLFW_KEY_T                  84
#define GLFW_KEY_U                  85
#define GLFW_KEY_V                  86
#define GLFW_KEY_W                  87
#define GLFW_KEY_X                  88
#define GLFW_KEY_Y                  89
#define GLFW_KEY_Z                  90
#define GLFW_KEY_LEFT_BRACKET       91  /* [ */
#define GLFW_KEY_BACKSLASH          92  /* \ */
#define GLFW_KEY_RIGHT_BRACKET      93  /* ] */
#define GLFW_KEY_GRAVE_ACCENT       96  /* ` */
#define GLFW_KEY_WORLD_1            161 /* non-US #1 */
#define GLFW_KEY_WORLD_2            162 /* non-US #2 */

#define GLFW_KEY_ESCAPE             256
#define GLFW_KEY_ENTER              257
#define GLFW_KEY_TAB                258
#define GLFW_KEY_BACKSPACE          259
#define GLFW_KEY_INSERT             260
#define GLFW_KEY_DELETE             261
#define GLFW_KEY_RIGHT              262
#define GLFW_KEY_LEFT               263
#define GLFW_KEY_DOWN               264
#define GLFW_KEY_UP                 265
#define GLFW_KEY_PAGE_UP            266
#define GLFW_KEY_PAGE_DOWN          267
#define GLFW_KEY_HOME               268
#define GLFW_KEY_END                269
#define GLFW_KEY_CAPS_LOCK          280
#define GLFW_KEY_SCROLL_LOCK        281
#define GLFW_KEY_NUM_LOCK           282
#define GLFW_KEY_PRINT_SCREEN       283
#define GLFW_KEY_PAUSE              284
#define GLFW_KEY_F1                 290
#define GLFW_KEY_F2                 291
#define GLFW_KEY_F3                 292
#define GLFW_KEY_F4                 293
#define GLFW_KEY_F5                 294
#define GLFW_KEY_F6                 295
#define GLFW_KEY_F7                 296
#define GLFW_KEY_F8                 297
#define GLFW_KEY_F9                 298
#define GLFW_KEY_F10                299
#define GLFW_KEY_F11                300
#define GLFW_KEY_F12                301
#define GLFW_KEY_F13                302
#define GLFW_KEY_F14                303
#define GLFW_KEY_F15                304
#define GLFW_KEY_F16                305
#define GLFW_KEY_F17                306
#define GLFW_KEY_F18                307
#define GLFW_KEY_F19                308
#define GLFW_KEY_F20                309
#define GLFW_KEY_F21                310
#define GLFW_KEY_F22                311
#define GLFW_KEY_F23                312
#define GLFW_KEY_F24                313
#define GLFW_KEY_F25                314
#define GLFW_KEY_KP_0               320
#define GLFW_KEY_KP_1               321
#define GLFW_KEY_KP_2               322
#define GLFW_KEY_KP_3               323
#define GLFW_KEY_KP_4               324
#define GLFW_KEY_KP_5               325
#define GLFW_KEY_KP_6               326
#define GLFW_KEY_KP_7               327
#define GLFW_KEY_KP_8               328
#define GLFW_KEY_KP_9               329
#define GLFW_KEY_KP_DECIMAL         330
#define GLFW_KEY_KP_DIVIDE          331
#define GLFW_KEY_KP_MULTIPLY        332
#define GLFW_KEY_KP_SUBTRACT        333
#define GLFW_KEY_KP_ADD             334
#define GLFW_KEY_KP_ENTER           335
#define GLFW_KEY_KP_EQUAL           336
#define GLFW_KEY_LEFT_SHIFT         340
#define GLFW_KEY_LEFT_CONTROL       341
#define GLFW_KEY_LEFT_ALT           342
#define GLFW_KEY_LEFT_SUPER         343
#define GLFW_KEY_RIGHT_SHIFT        344
#define GLFW_KEY_RIGHT_CONTROL      345
#define GLFW_KEY_RIGHT_ALT          346
#define GLFW_KEY_RIGHT_SUPER        347
#define GLFW_KEY_MENU               348

#define GLFW_KEY_LAST               GLFW_KEY_MENU

#define GLFW_MOD_SHIFT           0x0001
#define GLFW_MOD_CONTROL         0x0002
#define GLFW_MOD_ALT             0x0004
#define GLFW_MOD_SUPER           0x0008

#define GLFW_MOUSE_BUTTON_1         0
#define GLFW_MOUSE_BUTTON_2         1
#define GLFW_MOUSE_BUTTON_3         2
#define GLFW_MOUSE_BUTTON_4         3
#define GLFW_MOUSE_BUTTON_5         4
#define GLFW_MOUSE_BUTTON_6         5
#define GLFW_MOUSE_BUTTON_7         6
#define GLFW_MOUSE_BUTTON_8         7
#define GLFW_MOUSE_BUTTON_LAST      GLFW_MOUSE_BUTTON_8
#define GLFW_MOUSE_BUTTON_LEFT      GLFW_MOUSE_BUTTON_1
#define GLFW_MOUSE_BUTTON_RIGHT     GLFW_MOUSE_BUTTON_2
#define GLFW_MOUSE_BUTTON_MIDDLE    GLFW_MOUSE_BUTTON_3

#define GLFW_JOYSTICK_1             0
#define GLFW_JOYSTICK_2             1
#define GLFW_JOYSTICK_3             2
#define GLFW_JOYSTICK_4             3
#define GLFW_JOYSTICK_5             4
#define GLFW_JOYSTICK_6             5
#define GLFW_JOYSTICK_7             6
#define GLFW_JOYSTICK_8             7
#define GLFW_JOYSTICK_9             8
#define GLFW_JOYSTICK_10            9
#define GLFW_JOYSTICK_11            10
#define GLFW_JOYSTICK_12            11
#define GLFW_JOYSTICK_13            12
#define GLFW_JOYSTICK_14            13
#define GLFW_JOYSTICK_15            14
#define GLFW_JOYSTICK_16            15
#define GLFW_JOYSTICK_LAST          GLFW_JOYSTICK_16

#pragma endregion

enum class DKeyMod : int32
{
	DMOD_SHIFT		= 1,
	DMOD_CONTROL	= 2,
	DMOD_ALT		= 4,
	DMOD_SUPER		= 8
};

enum class DKeyAction : int32
{
	UNDEFINED	= GLFW_KEY_UNKNOWN,

	RELEASED	= GLFW_RELEASE,
	PRESSED		= GLFW_PRESS,
	REPEATED	= GLFW_REPEAT,
};

enum class DKey : int32
{
	UNDEFINED		= GLFW_KEY_UNKNOWN,

	SPACE			= GLFW_KEY_SPACE,
	APOSTROPHE		= GLFW_KEY_APOSTROPHE,
	COMMA			= GLFW_KEY_COMMA,
	MINUS			= GLFW_KEY_MINUS,
	PERIOD			= GLFW_KEY_PERIOD,
	SLASH			= GLFW_KEY_SLASH,
	NUM_0			= GLFW_KEY_0,
	NUM_1			= GLFW_KEY_1,
	NUM_2			= GLFW_KEY_2,
	NUM_3			= GLFW_KEY_3,
	NUM_4			= GLFW_KEY_4,
	NUM_5			= GLFW_KEY_5,
	NUM_6			= GLFW_KEY_6,
	NUM_7			= GLFW_KEY_7,
	NUM_8			= GLFW_KEY_8,
	NUM_9			= GLFW_KEY_9,
	SEMICOLON		= GLFW_KEY_SEMICOLON,
	EQUAL			= GLFW_KEY_EQUAL,
	A				= GLFW_KEY_A,
	B				= GLFW_KEY_B,
	C				= GLFW_KEY_C,
	D				= GLFW_KEY_D,
	E				= GLFW_KEY_E, 
	F				= GLFW_KEY_F,
	G				= GLFW_KEY_G,
	H				= GLFW_KEY_H,
	I				= GLFW_KEY_I,
	J				= GLFW_KEY_J,
	K				= GLFW_KEY_K,
	L				= GLFW_KEY_L,
	M				= GLFW_KEY_M,
	N				= GLFW_KEY_N,
	O				= GLFW_KEY_O,
	P				= GLFW_KEY_P,
	Q				= GLFW_KEY_Q,
	R				= GLFW_KEY_R,
	S				= GLFW_KEY_S,
	T				= GLFW_KEY_T,
	U				= GLFW_KEY_U,
	V				= GLFW_KEY_V,
	W				= GLFW_KEY_W,
	X				= GLFW_KEY_X,
	Y				= GLFW_KEY_Y,
	Z				= GLFW_KEY_Z,
	LEFT_BRACKET	= GLFW_KEY_LEFT_BRACKET,
	BACKSLASH		= GLFW_KEY_BACKSLASH,
	RIGHT_BRACKET	= GLFW_KEY_RIGHT_BRACKET,
	GRAVE_ACCENT	= GLFW_KEY_GRAVE_ACCENT,
	WORLD_1			= GLFW_KEY_WORLD_1,
	WORLD_2			= GLFW_KEY_WORLD_2,

	ESCAPE			= GLFW_KEY_ESCAPE,
	ENTER			= GLFW_KEY_ENTER,
	TAB				= GLFW_KEY_TAB,
	BACKSPACE		= GLFW_KEY_BACKSPACE,
	INSERT			= GLFW_KEY_INSERT,
	KDELETE			= GLFW_KEY_DELETE,
	RIGHT			= GLFW_KEY_RIGHT,
	LEFT			= GLFW_KEY_LEFT,
	DOWN			= GLFW_KEY_DOWN,
	UP				= GLFW_KEY_UP,
	PAGE_UP			= GLFW_KEY_PAGE_UP,
	PAGE_DOWN		= GLFW_KEY_PAGE_DOWN,
	HOME			= GLFW_KEY_HOME,
	END				= GLFW_KEY_END,
	CAPS_LOCK		= GLFW_KEY_CAPS_LOCK,
	SCROLL_LOCK		= GLFW_KEY_SCROLL_LOCK,
	NUM_LOCK		= GLFW_KEY_NUM_LOCK,
	PRINT_SCREEN	= GLFW_KEY_PRINT_SCREEN,
	PAUSE			= GLFW_KEY_PAUSE,
	F1				= GLFW_KEY_F1,
	F2				= GLFW_KEY_F2,
	F3				= GLFW_KEY_F3,
	F4				= GLFW_KEY_F4,
	F5				= GLFW_KEY_F5,
	F6				= GLFW_KEY_F6,
	F7				= GLFW_KEY_F7,
	F8				= GLFW_KEY_F8,
	F9				= GLFW_KEY_F9,
	F10				= GLFW_KEY_F10,
	F11				= GLFW_KEY_F11,
	F12				= GLFW_KEY_F12,
	F13				= GLFW_KEY_F13,
	F14				= GLFW_KEY_F14,
	F15				= GLFW_KEY_F15,
	F16				= GLFW_KEY_F16,
	F17				= GLFW_KEY_F17,
	F18				= GLFW_KEY_F18,
	F19				= GLFW_KEY_F19,
	F20				= GLFW_KEY_F20,
	F21				= GLFW_KEY_F21,
	F22				= GLFW_KEY_F22,
	F23				= GLFW_KEY_F23,
	F24				= GLFW_KEY_F24,
	F25				= GLFW_KEY_F25,
	NUMPAD_0		= GLFW_KEY_KP_0,
	NUMPAD_1		= GLFW_KEY_KP_1,
	NUMPAD_2		= GLFW_KEY_KP_2,
	NUMPAD_3		= GLFW_KEY_KP_3,
	NUMPAD_4		= GLFW_KEY_KP_4,
	NUMPAD_5		= GLFW_KEY_KP_5,
	NUMPAD_6		= GLFW_KEY_KP_6,
	NUMPAD_7		= GLFW_KEY_KP_7,
	NUMPAD_8		= GLFW_KEY_KP_8,
	NUMPAD_9		= GLFW_KEY_KP_9,
	NUMPAD_DECIMAL	= GLFW_KEY_KP_DECIMAL,
	NUMPAD_DIVIDE	= GLFW_KEY_KP_DIVIDE,
	NUMPAD_MULTIPLY	= GLFW_KEY_KP_MULTIPLY,
	NUMPAD_SUBTRACT	= GLFW_KEY_KP_SUBTRACT,
	NUMPAD_ADD		= GLFW_KEY_KP_ADD,
	NUMPAD_ENTER	= GLFW_KEY_KP_ENTER,
	NUMPAD_EQUAL	= GLFW_KEY_KP_EQUAL,
	LEFT_SHIFT		= GLFW_KEY_LEFT_SHIFT,
	LEFT_CONTROL	= GLFW_KEY_LEFT_CONTROL,
	LEFT_ALT		= GLFW_KEY_LEFT_ALT,
	LEFT_SUPER		= GLFW_KEY_LEFT_SUPER,
	RIGHT_SHIFT		= GLFW_KEY_RIGHT_SHIFT,
	RIGHT_CONTROL	= GLFW_KEY_RIGHT_CONTROL,
	RIGHT_ALT		= GLFW_KEY_RIGHT_ALT,
	RIGHT_SUPER		= GLFW_KEY_RIGHT_SUPER,
	MENU			= GLFW_KEY_MENU,
	LAST			= GLFW_KEY_LAST
};

enum class DMouse : int32
{
	BUTTON_1        = GLFW_MOUSE_BUTTON_1,
	BUTTON_2        = GLFW_MOUSE_BUTTON_2,
	BUTTON_3        = GLFW_MOUSE_BUTTON_3,
	BUTTON_4        = GLFW_MOUSE_BUTTON_4,
	BUTTON_5        = GLFW_MOUSE_BUTTON_5,
	BUTTON_6        = GLFW_MOUSE_BUTTON_6,
	BUTTON_7        = GLFW_MOUSE_BUTTON_7,
	BUTTON_8        = GLFW_MOUSE_BUTTON_8,
	BUTTON_LAST     = BUTTON_8,
	BUTTON_LEFT     = BUTTON_1,
	BUTTON_RIGHT    = BUTTON_2,
	BUTTON_MIDDLE   = BUTTON_3
};

enum class DJoy : int32
{
	JOYSTICK_1		= GLFW_JOYSTICK_1,   
	JOYSTICK_2		= GLFW_JOYSTICK_2,   
	JOYSTICK_3		= GLFW_JOYSTICK_3,   
	JOYSTICK_4		= GLFW_JOYSTICK_4,   
	JOYSTICK_5		= GLFW_JOYSTICK_5,   
	JOYSTICK_6		= GLFW_JOYSTICK_6,   
	JOYSTICK_7		= GLFW_JOYSTICK_7,   
	JOYSTICK_8		= GLFW_JOYSTICK_8,   
	JOYSTICK_9		= GLFW_JOYSTICK_9,   
	JOYSTICK_10		= GLFW_JOYSTICK_10,
	JOYSTICK_11		= GLFW_JOYSTICK_11,
	JOYSTICK_12		= GLFW_JOYSTICK_12,
	JOYSTICK_13		= GLFW_JOYSTICK_13,
	JOYSTICK_14		= GLFW_JOYSTICK_14,
	JOYSTICK_15		= GLFW_JOYSTICK_15,
	JOYSTICK_16		= GLFW_JOYSTICK_16,
	JOYSTICK_LAST	= JOYSTICK_16
};
