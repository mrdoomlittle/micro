# include "eeprom.h"
# include <string.h>
# define PAGE_SHIFT 6
# define PAGE_SIZE 64
mdl_u8_t page_buff[PAGE_SIZE];
void eeprom_init(struct eeprom *__eeprom, void *__driver_ctx) {
	__eeprom->driver_ctx = __driver_ctx;
	memset(page_buff, 0xFF, PAGE_SIZE);
}

void eeprom_page_update(struct eeprom *__eeprom, mdl_u8_t __page) {
	_24lc256_page_write((struct _24lc256*)__eeprom->driver_ctx, page_buff, __page*PAGE_SIZE);}

void eeprom_put_byte(struct eeprom *__eeprom, mdl_u8_t __byte, mdl_u16_t __addr) {
	mdl_u16_t static page_addr = 0;
	mdl_u16_t _page_addr = (__addr>>PAGE_SHIFT)*PAGE_SIZE;

	// update page
	if (page_addr != _page_addr) {
		eeprom_page_update(__eeprom, page_addr>>PAGE_SHIFT);
		memset(page_buff, 0xFF, PAGE_SIZE);
		page_addr = _page_addr;
	}
	*(page_buff+(__addr-_page_addr)) = __byte;
}

# ifndef __EEPROM_LIGHT
void eeprom_put_16l(struct eeprom *__eeprom, mdl_u16_t __val, mdl_u16_t __addr) {
	eeprom_put_8l(__eeprom, __val, __addr);
	eeprom_put_8l(__eeprom, __val>>8, __addr+1);
}

void eeprom_get_16l(struct eeprom *__eeprom, mdl_u16_t *__val, mdl_u16_t __addr) {
	mdl_u8_t part;

	part = 0;
	eeprom_get_8l(__eeprom, &part, __addr);
	*__val |= (mdl_u16_t)part;

	part = 0;
	eeprom_get_8l(__eeprom, &part, __addr+1);
	*__val |= (mdl_u16_t)part<<8;
}

void eeprom_put_32l(struct eeprom *__eeprom, mdl_u32_t __val, mdl_u16_t __addr) {
	eeprom_put_16l(__eeprom, __val, __addr);
	eeprom_put_16l(__eeprom, __val>>16, __addr+2);
}

void eeprom_get_32l(struct eeprom *__eeprom, mdl_u32_t *__val, mdl_u16_t __addr) {
	mdl_u16_t part;

	part = 0;
	eeprom_get_16l(__eeprom, &part, __addr);
	*__val |= (mdl_u32_t)part;

	part = 0;
	eeprom_get_16l(__eeprom, &part, __addr+2);
	*__val |= (mdl_u32_t)part << 16;
}

void eeprom_put_64l(struct eeprom *__eeprom, mdl_u64_t __val, mdl_u16_t __addr) {
	eeprom_put_32l(__eeprom, __val, __addr);
	eeprom_put_32l(__eeprom, __val>>32, __addr+4);
}

void eeprom_get_64l(struct eeprom *__eeprom, mdl_u64_t *__val, mdl_u16_t __addr) {
	mdl_u32_t part;

	part = 0;
	eeprom_get_32l(__eeprom, &part, __addr);
	*__val |= (mdl_u64_t)part;

	part = 0;
	eeprom_get_32l(__eeprom, &part, __addr+4);
	*__val |= (mdl_u64_t)part<<32;
}
# endif
void eeprom_get_byte(struct eeprom *__eeprom, mdl_u8_t *__byte, mdl_u16_t __addr) {
	_24lc256_get_byte((struct _24lc256*)__eeprom->driver_ctx, __byte, __addr);
}
