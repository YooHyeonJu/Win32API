#include "stdafx.h"
#include "MyException.h"
#include<iostream>


using namespace std;

class CMyException
{
private:
	int ExceptionCode;		//���� ó�� ����
public:
	CMyException(int);		//������ �� �Ҹ��� ����
	~CMyException();
	int GetExceptionCode() { return ExceptionCode; }
};

CMyException::CMyException(int Exception)
{
	ExceptionCode = Exception;
	switch (ExceptionCode)
	{
	case 1:
		std::cout << "�޸𸮰� �����մϴ�." << std::endl;
		break;
	case 2:
		std::cout << "�ϵ��ũ�� �� á���ϴ�." << std::endl;
		break;
	case 3:
		std::cout << "������ �߸��Ǿ����ϴ�." << std::endl;
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
		cout << "�۾��� �Ϸ��߽��ϴ�." << endl;
	}
	catch (CMyException &e)
	{
		cout << "���� ó�� �ڵ� = " << e.GetExceptionCode() << endl;
	}
}