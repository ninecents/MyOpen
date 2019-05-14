#include <ntifs.h>

//提供一个卸载函数，让程序能卸载，如果没有这个函数，驱动将不能卸载。
VOID UnDriver(PDRIVER_OBJECT driver)
{
	UNREFERENCED_PARAMETER(driver);
	KdPrint(("UnDriver.\n"));
}

//入口函数，相当于main。
NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING reg_path)
{
	UNREFERENCED_PARAMETER(driver);
	UNREFERENCED_PARAMETER(reg_path);
	KdPrint(("Hello World!\n"));
	driver->DriverUnload = UnDriver;
	return STATUS_SUCCESS;
}