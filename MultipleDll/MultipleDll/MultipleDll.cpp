// MultipleDll.cpp: DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"


extern "C" _declspec(dllexport) int Multiple(int a, int b)
{
	return a * b;
}