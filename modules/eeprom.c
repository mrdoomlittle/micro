# include "eeprom.h"
# include <string.h>
# define PAGE_SIZE 64
mdl_u8_t page_buff[PAGE_SIZE];
// how many 64 in no
# define SH 6
void eeprom_init(struct eeprom_t *__eeprom, void *__driver_context) {
	__eeprom->driver_context = __driver_context;
	memset(page_buff, 0xFF, PAGE_SIZE);
}

void eeprom_page_update(struct eeprom_t *__eeprom, mdl_u8_t __page) {
	_24lc256_page_write((struct _24lc256_t*)__eeprom->driver_context, page_buff, __page*PAGE_SIZE);}

void eeprom_put_byte(struct eeprom_t *__eeprom, mdl_u8_t __byte, mdl_u16_t __addr) {
	mdl_u16_t static page_addr = 0;
	mdl_u16_t _page_addr = (__addr >> SH)*PAGE_SIZE;

	// update page
	if (page_addr != _page_addr) {
		eeprom_page_update(__eeprom, page_addr >> SH);
		memset(page_buff, 0xFF, PAGE_SIZE);
		page_addr = _page_addr;
	}

	mdl_u8_t page_off = __addr-page_addr;
	page_buff[page_off] = __byte;
}

# ifndef __EEPROM_LIGHT
void eeprom_put_w16(struct eeprom_t *__eeprom, mdl_u16_t __w16, mdl_u16_t __addr) {
	eeprom_put_w8(__eeprom, __w16, __addr);
	eeprom_put_w8(__eeprom, __w16 >> 8, __addr+1);
}

void eeprom_get_w16(struct eeprom_t *__eeprom, mdl_u16_t *__w16, mdl_u16_t __addr) {
	mdl_u8_t _16b_part;

	_16b_part = 0;
	eeprom_get_w8(__eeprom, &_16b_part, __addr);
	*__w16 |= (mdl_u16_t)_16b_part;

	_16b_part = 0;
	eeprom_get_w8(__eeprom, &_16b_part, __addr+1);
	*__w16 |= (mdl_u16_t)_16b_part << 8;
}

void eeprom_put_w32(struct eeprom_t *__eeprom, mdl_u32_t __w32, mdl_u16_t __addr) {
	eeprom_put_w16(__eeprom, __w32, __addr);
	eeprom_put_w16(__eeprom, __w32 >> 16, __addr+1);
}

void eeprom_get_w32(struct eeprom_t *__eeprom, mdl_u32_t *__w32, mdl_u16_t __addr) {
	mdl_u16_t _32b_part;

	_32b_part = 0;
	eeprom_get_w16(__eeprom, &_32b_part, __addr);
	*__w32 |= (mdl_u32_t)_32b_part;

	_32b_part = 0;
	eeprom_get_w16(__eeprom, &_32b_part, __addr+1);
	*__w32 |= (mdl_u32_t)_32b_part << 16;
}

void eeprom_put_w64(struct eeprom_t *__eeprom, mdl_u64_t __w64, mdl_u16_t __addr) {
	eeprom_put_w32(__eeprom, __w64, __addr);
	eeprom_put_w32(__eeprom, __w64 >> 32, __addr+1);
}

void eeprom_get_w64(struct eeprom_t *__eeprom, mdl_u64_t *__w64, mdl_u16_t __addr) {
	mdl_u32_t _64b_part;

	_64b_part = 0;
	eeprom_get_w32(__eeprom, &_64b_part, __addr);
	*__w64 |= (mdl_u64_t)_64b_part;

	_64b_part = 0;
	eeprom_get_w32(__eeprom, &_64b_part, __addr+1);
	*__w64 |= (mdl_u64_t)_64b_part << 32;
}
# endif
void eeprom_get_byte(struct eeprom_t *__eeprom, mdl_u8_t *__byte, mdl_u16_t __addr) {
	_24lc256_get_byte((struct _24lc256_t*)__eeprom->driver_context, __byte, __addr);
}
