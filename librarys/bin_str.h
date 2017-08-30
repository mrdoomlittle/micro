# ifndef __bin__str__h
# define __bin__str__h

char static *_w8_bin_str(mdl_u8_t __data) {
	char static bin_str[9];
	for (mdl_uint_t point = 0; point != 8; point ++)
		bin_str[point] = ((__data >> (7-point)) & 1) == 1? '1' : '0';
	bin_str[8] = '\0';
	return bin_str;
}

char static *_w16_bin_str(mdl_u16_t __data) {
	char static bin_str[17];
	for (mdl_uint_t point = 0; point != 16; point ++)
		bin_str[point] = ((__data >> (15-point)) & 1) == 1? '1' : '0';
	bin_str[16] = '\0';
	return bin_str;
}

# endif /*__bin__str__h*/
