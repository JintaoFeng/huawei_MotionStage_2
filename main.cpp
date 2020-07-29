#include "mainwindow.h"

#include <QApplication>

//#include <Windows.h>
//#include <DbgHelp.h>

//程式异常捕获
//LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
//{
//    //创建 Dump 文件
//    HANDLE hDumpFile = CreateFile(L"crash.dmp", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
//    if (hDumpFile != INVALID_HANDLE_VALUE)
//    {
//    //Dump信息
//    MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
//    dumpInfo.ExceptionPointers = pException;
//    dumpInfo.ThreadId = GetCurrentThreadId();
//    dumpInfo.ClientPointers = TRUE;

//    //写入Dump文件内容
//    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
//    }

//    //这里弹出一个错误对话框并退出程序

//    QMessageBox::critical(NULL, "程序崩溃", "抱歉，请联系程序猿解决！");

//    return EXCEPTION_EXECUTE_HANDLER;
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
 //   SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);//注冊异常捕获函数
    MainWindow w;
    w.show();
//    axis axis1;
//    axis1.show();
    return a.exec();
}
