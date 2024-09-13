#pragma once

namespace Engine
{
	typedef unsigned char			_ubyte;
	typedef signed char				_byte;
	typedef char					_char;
	typedef wchar_t					_tchar;
	typedef wstring					_wstring;

	typedef unsigned short			_ushort;
	typedef signed short			_short;

	typedef unsigned int			_uint;
	typedef signed int				_int;

	typedef unsigned long			_ulong;
	typedef signed long				_long;

	typedef float					_float;
	typedef double					_double;

	typedef bool					_bool;

	/* ���Ϳ� ����� �����ϱ����� �뵵�� Ÿ��. (��, ������) */
	typedef XMFLOAT2				_float2;
	typedef XMFLOAT3				_float3;
	typedef XMFLOAT4				_float4;
	typedef XMFLOAT4X4				_float4x4;

	/* ���Ϳ� ����� SIMD����(����)������ �뵵�� Ÿ��. (Stack�޸𸮾ȿ��� �����ض�) */
	/* ����� ����, ���Ÿ���� �ּҸ� �����ϸ� ����. */
	typedef XMVECTOR				_vector;
	typedef FXMVECTOR				_fvector; /* 1 ~ 3 */
	typedef GXMVECTOR				_gvector; /* 4 */
	typedef HXMVECTOR				_hvector; /* 5 ~ 6 */
	typedef CXMVECTOR				_cvector; /* 7 ~ */

	typedef XMMATRIX				_matrix;
	typedef FXMMATRIX				_fmatrix;
	typedef CXMMATRIX				_cmatrix;
}