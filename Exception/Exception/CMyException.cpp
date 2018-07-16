#include "stdafx.h"
#include "MyException.h"
#include<iostream>


using namespace std;

class CMyException
{
private:
	int ExceptionCode;		//예외 처리 변수
public:
	CMyException(int);		//생성자 및 소멸자 선언
	~CMyException();
	int GetExceptionCode() { return ExceptionCode; }
};

CMyException::CMyException(int Exception)
{
	ExceptionCode = Exception;
	switch (ExceptionCode)
	{
	case 1:
		std::cout << "메모리가 부족합니다." << std::endl;
		break;
	case 2:
		std::cout << "하드디스크가 꽉 찼습니다." << std::endl;
		break;
	case 3:
		std::cout << "연산이 잘못되었습니다." << std::endl;
		break;
	}
}

CMyException::~CMyException(void)
{
}

void Calc()
{
	if (1) throw CMyException(2);
}

void main()
{
	try
	{
		Calc();
		cout << "작업을 완료했습니다." << endl;
	}
	catch (CMyException &e)
	{
		cout << "예외 처리 코드 = " << e.GetExceptionCode() << endl;
	}
}