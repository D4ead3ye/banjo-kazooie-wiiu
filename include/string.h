#ifndef BK_STRING_H
#define BK_STRING_H

#include <ultra64.h>

/* Custom string functions for Banjo-Kazooie */
void strcatc(char *dst, char src);
void strFToA(char *dst, f32 val);
void _strFToA(char *dst, f32 val, s32 decPlaces);
void strIToA(char *str, s32 num);
void _strIToA(char *str, s32 num, char prefix);
s32 strcmpToTok(char *str1, char* str2, char* str3);
char *strtok(char *str, const char *delim);
void strcpyToTok(char *arg0, char *arg1, char *arg2);
void strToUpper(char *str);

#endif /* BK_STRING_H */
