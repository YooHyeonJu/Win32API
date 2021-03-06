// Exception.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include<stdio.h>
#include<windows.h>

int main()
{
	int a, b;
	printf("나누어질 수 입력 : ");
	scanf_s("%d", &a);

	printf("나누는 수 입력 : ");
	scanf_s("%d", &b);

	_try{
		printf("나누는 결과 %d\n",a / b);
	}
		_except((GetExceptionCode() == EXCEPTION_INT_DIVIDE_BY_ZERO) ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{
		printf("나누어야 할 수가 0이므로 연산을 수행할 수 없음.\n");
	}
    return 0;
}

