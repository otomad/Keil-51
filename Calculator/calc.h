#ifndef __CALC_H__
#define __CALC_H__

typedef enum {
	NONE, // 当读取到这个时跳过
	END, // 表示结束
	PLUS_POS, // 加正
	PLUS_NEG, // 加负，即减
	TIMES_POS, // 乘正
	TIMES_NEG, // 乘负
	DIV_POS, // 除正
	DIV_NEG // 除负
} signName;

struct Datum {
	signName sign;
	double num;
};

struct Datum *convertData(char *str, struct Datum *dat);
double calcData(struct Datum *dat);
double calc(char *str);

#endif
