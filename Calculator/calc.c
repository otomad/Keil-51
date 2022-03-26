#include "calc.h"

#define LIMIT 20 // 虽然不限制也可以，但万一出现死循环也不太好
#define LENGTH 9 // 16 位的屏幕最多也只能计算八位

struct Datum *convertData(char *str, struct Datum *dat) {
	unsigned char i, j = 0;
	double n = 0;
	signName s = PLUS_POS;
	for (i = 0; i < LIMIT; i++) {
		if (str[i] >= '0' && str[i] <= '9')
			n = n * 10 + (str[i] - '0');
		else {
			dat[j].sign = s; // dat[j] = {s, n};
			dat[j].num = n;
			j++;
			if (str[i] == '\0') {
				dat[j].sign = END;
				dat[j].num = 0;
				break;
			}
			s = str[i] == '+' ? PLUS_POS :
				str[i] == '-' ? PLUS_NEG :
				str[i] == '*' && str[i + 1] == '-' ? (i++, TIMES_NEG) :
				str[i] == '/' && str[i + 1] == '-' ? (i++, DIV_NEG) :
				str[i] == '*' ? TIMES_POS :
				DIV_POS;
			n = 0;
		}
	}
	return dat;
}

double calcData(struct Datum *dat) {
	double ans = 0;
	unsigned char i;
	struct Datum *curData, *prevData;
	// 优先计算乘除法
	for (i = 0; i < LIMIT; i++) {
		if (dat[i].sign == END) break;
		if (dat[i].sign == NONE) continue;
		prevData = curData;
		curData = dat + i; // 等同于 &dat[i]
		if (curData->sign >= TIMES_POS) {
			if (curData->sign == TIMES_NEG || curData->sign == DIV_NEG) {
				prevData->sign = !(prevData->sign - PLUS_POS) + PLUS_POS;
				curData->sign--;
			}
			prevData->num = curData->sign == TIMES_POS ? prevData->num * curData->num : prevData->num / curData->num;
			curData->sign = NONE;
			curData = prevData;
		}
	}
	// 接下来从左往右计算
	for (i = 0; i < LIMIT; i++) {
		if (dat[i].sign == END) break;
		if (dat[i].sign == NONE) continue;
//		prevData = curData;
		curData = dat + i; // 等同于 &dat[i]
		ans = curData->sign == PLUS_POS ? ans + curData->num : ans - curData->num;
	}
	return ans;
}

// double calc(char *str, unsigned char length) {
double calc(char *str) {
	struct Datum xdata dat[LENGTH]; // 只能放在 xdata 区，放在 code 区就不起作用了
	convertData(str, dat);
	return calcData(dat);
}
