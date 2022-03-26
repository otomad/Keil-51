unsigned int str_getLength(char *str) {
	#define UPPER_LIMIT 255
	unsigned int len;
	for (len = 0; len <= UPPER_LIMIT; len++)
		if (str[len] == '\0')
			return len;
	return 0;
}

char *str_appendChar(char *raw, char newChar) {
	unsigned int len = str_getLength(raw);
	raw[len] = newChar;
	raw[len + 1] = '\0';
	return raw;
}

char *str_removeLast(char *raw) {
	unsigned int len = str_getLength(raw);
	if (len == 0) return raw;
	raw[len - 1] = '\0';
	return raw;
}

char *str_rightAlign(char *raw, unsigned int width) {
	unsigned int len = str_getLength(raw);
	int i, j;
	raw[width] = '\0';
	for (i = width - 1, j = len - 1; i >= 0; i--, j--)
		raw[i] = j >= 0 ? raw[j] : ' ';
	return raw;
}