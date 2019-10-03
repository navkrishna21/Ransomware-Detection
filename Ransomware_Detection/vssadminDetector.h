#ifndef VSSADMINDETECTOR_H
#define VSSADMINDETECTOR_H
#include <cstdio>
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include <tlhelp32.h>

DWORD WINAPI vssadmin_detector(LPVOID lpParam);

#endif