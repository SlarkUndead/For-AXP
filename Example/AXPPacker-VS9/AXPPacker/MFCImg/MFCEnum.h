#ifndef _MFC_ENUM_060701_H_
#define _MFC_ENUM_060701_H_
#include "MFCImageType.h"

//////////////////////////////////////////////////////////////////////////
//
// enum
//
//////////////////////////////////////////////////////////////////////////

//=============================================================================
//	����ϵͳ
enum MFC_AXIS_SYS
{
	MFC_AXIS_X,
	MFC_AXIS_Y,
	MFC_AXIS_Z,
	MFC_AXIS_TIME, // ��ά��:-)
} ;
//=============================================================================
//	ƽ��8������
enum MFC_DIRECT_SYS
{
	MFC_DIRECT_TOP_LEFT,
	MFC_DIRECT_TOP,
	MFC_DIRECT_TOP_RIGHT,
	MFC_DIRECT_LEFT,
	MFC_DIRECT_RIGHT,
	MFC_DIRECT_BOTTOM_LEFT,
	MFC_DIRECT_BOTTOM,
	MFC_DIRECT_BOTTOM_RIGHT,
} ;
//=============================================================================
//	��״
enum MFC_SHAPE_TYPE
{
	MFC_SHAPE_RECT = 0,
	MFC_SHAPE_SQUARE,
	MFC_SHAPE_ELLIPSE,
	MFC_SHAPE_CIRCLE,
	MFC_SHAPE_ROUNDRECT,
	MFC_SHAPE_LINE,
	MFC_SHAPE_POLYGON,
} ;
//=============================================================================
//	�����������
enum MFC_REPEAT_MODE
{
	MFC_REPEAT_NONE = 0,
	MFC_REPEAT_SAWTOOTH = 1,	// ��ݲ��ظ�
	MFC_REPEAT_TRIANGULAR = 2,	// ���ǲ��ظ�
} ;
//=============================================================================
//	��ֵģʽ
enum MFC_INTERPOLATION_TYPE
{
	MFC_INTERPOLATION_NONE = 0,
	MFC_INTERPOLATION_BILINEAR,
} ;
//=============================================================================
//	�߼�����
enum LOGICAL_OP
{
	LOGI_AND,   // c = a & b
	LOGI_OR,    // c = a | b
	LOGI_XOR,   // c = a ^ b
	LOGI_ADD,   // c = a + b
	LOGI_SUB,   // c = a - b
	LOGI_MUL,   // c = a * b
	LOGI_DIV,   // c = a / b
	LOGI_LOG,   // c = log(a)
	LOGI_EXP,   // c = exp(a)
	LOGI_SQRT,  // c = sqrt(a)
	LOGI_TRIG,  // c = sin/cos/tan(a)
	LOGI_INVERT,// c = (2B - 1) - a

	LOGI_SEL_ADD, // ������������
	LOGI_SEL_SUB, // ������������
} ;
//=============================================================================
//	RGBAͨ���������Ķ���ֵ��Ϊ��mask
enum MFC_IMAGE_CHANNEL
{
	MFC_CHANNEL_RED   = 1 << 0,// 0x01,
	MFC_CHANNEL_GREEN = 1 << 1,// 0x02,
	MFC_CHANNEL_BLUE  = 1 << 2,// 0x04,
	MFC_CHANNEL_ALPHA = 1 << 3,// 0x08,
	MFC_CHANNEL_RGB   = MFC_CHANNEL_RED|MFC_CHANNEL_GREEN|MFC_CHANNEL_BLUE,// 0x07,
	MFC_CHANNEL_RGBA  = MFC_CHANNEL_RGB|MFC_CHANNEL_ALPHA,// 0x0F,
	MFC_CHANNEL_GRAY  = 1 << 4,// 0x10
} ;
//=============================================================================
// ����ʽ
enum MFC_LINE_STYLE
{
	MFC_LINE_STYLE_SOLID = 0,
	MFC_LINE_STYLE_DASH  = 1,
	MFC_LINE_STYLE_DOT   = 2,
} ;
//=============================================================================
// ɫ������������ֵ��˳��һ�����ܱ䣩
enum MFC_TONE_REGION
{
	/// shadow region of image.
	MFC_TONE_SHADOWS = 0,
	/// midtone region of image.
	MFC_TONE_MIDTONES = 1,
	/// highlight region of image.
	MFC_TONE_HIGHLIGHTS = 2,
};


#endif