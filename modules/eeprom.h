# ifndef __micro__eeprom__h
# define __micro__eeprom__h
# include "../micro.h"
# include <mdlint.h>
# include "../drivers/24lc256.h"
# define __EEPROM_LIGHT
struct eeprom {
	void *driver_ctx;
};

void eeprom_init(struct eeprom*, void*);
void eeprom_page_update(struct eeprom*, mdl_u8_t);
void eeprom_put_byte(struct eeprom*, mdl_u8_t, mdl_u16_t);
void eeprom_get_byte(struct eeprom*, mdl_u8_t*, mdl_u16_t);

# define eeprom_put_8l(__eeprom, __byte, __addr) eeprom_put_byte(__eeprom, __byte, __addr);
# define eeprom_get_8l(__eeprom, __byte, __addr) eeprom_get_byte(__eeprom, __byte, __addr);
# ifndef __EEPROM_LIGHT
void eeprom_put_16l(struct eeprom*, mdl_u16_t, mdl_u16_t);
void eeprom_get_16l(struct eeprom*, mdl_u16_t*, mdl_u16_t);

void eeprom_put_32l(struct eeprom*, mdl_u32_t, mdl_u16_t);
void eeprom_get_32l(struct eeprom*, mdl_u32_t*, mdl_u16_t);

void eeprom_put_64l(struct eeprom*, mdl_u64_t, mdl_u16_t);
void eeprom_get_64l(struct eeprom*, mdl_u64_t*, mdl_u16_t);
# endif
# endif /*__micro__eeprom__h*/
