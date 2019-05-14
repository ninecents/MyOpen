// tools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include <Windows.h>

int main(int argc, char **argv)
{
//     std::cout << "Hello World!\n"; 

	if (argc != 2)
	{
		std::cout << "必须有一个参数！\n";
		return -1;
	}

	HMODULE module = LoadLibraryA(argv[1]);
	printf("module = 0X%08X\n", (unsigned int)module);

	char szModuleFileName[MAX_PATH] = { 0 };
	GetModuleFileNameA(module, szModuleFileName, MAX_PATH);
	printf("%s 的全路径是 %s\n", argv[1], szModuleFileName);
	getchar();

	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
