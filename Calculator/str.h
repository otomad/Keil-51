#ifndef __STR_H__
#define __STR_H__

unsigned int str_getLength(char *str);
char *str_appendChar(char *raw, char newChar);
char *str_removeLast(char *raw);
char *str_rightAlign(char *raw, unsigned int width);

#endif
