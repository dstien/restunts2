#pragma once

#define printf  _printf
#define strcmp  _strcmp
#define stricmp _stricmp

extern void __cdecl _printf(const char*, ...);
extern int __cdecl _strcmp(const char*, const char*);
extern int __cdecl _stricmp(const char*, const char*);
