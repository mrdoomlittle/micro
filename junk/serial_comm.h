# ifndef __serial__comm__h
# define __serial__comm__h
# include "micro.h"
# include <eint_t.h>
# include <string.h>
# define BAUD 19200
//# define DELAY (16000000/16/BAUDRATE) - 1

#include <util/setbaud.h>
struct scomm_t {
	mdl_u8_t rx_pid, tx_pid;
};

void scomm_init(struct scomm_t*, mdl_u8_t, mdl_u8_t);
void scomm_send_byte(struct scomm_t*, mdl_u8_t);
void scomm_recv_byte(struct scomm_t*, mdl_u8_t*);

void __inline__ scomm_send(struct scomm_t *__scomm, mdl_u8_t *__data, mdl_uint_t __bc) {
	for (mdl_uint_t point = 0; point != __bc; point ++) scomm_send_byte(__scomm, *(__data + point));}
void __inline__ scomm_recv(struct scomm_t *__scomm, mdl_u8_t *__data, mdl_uint_t __bc) {
	for (mdl_uint_t point = 0; point != __bc; point ++) scomm_recv_byte(__scomm, __data + point);}

void __inline__ scomm_print(struct scomm_t *__scomm, char const *__str) {
	scomm_send(__scomm, (mdl_u8_t *)__str, strlen(__str));}
void __inline__ scomm_println(struct scomm_t *__scomm, char const *__str) {
	scomm_print(__scomm, __str);
	scomm_send_byte(__scomm, '\n');
}
# endif /*__serial__comm__h*/
