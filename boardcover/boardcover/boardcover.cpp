// boardcover.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"


int main()
{
	int total = 0;
	int Height = 0;
	int Width = 0;
	int i,j,k;
	char**arr = 0;
	for (i = 0; i < total; i++)
	{
		scanf("%d %d", &Height,&Width);
		//공간 생성
		arr = (char**)malloc(sizeof(char*)*Height);
		for (j = 0; j < Height; j++)
		{
			arr[j] = (char*)malloc(sizeof(char)*Width);
		}

		//값 받기
		for (j = 0; j < Height; j++)
		{
			scanf("%s", arr[j]);
		}
		//값 출력
		for (j = 0; j < Height; j++)
		{
			printf("%s", arr[j]);
		}


		
	}

    return 0;
}

