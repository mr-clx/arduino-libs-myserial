#ifndef __MYSERIAL_H__
#define __MYSERIAL_H__

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/*
 * Serial->Device: (32 chars max)
 *   (1)  "#",<next_block_size>," ",<cmd>," ",<params>
 *   (2)  "#",<cmd_int>," ",<params>\n
 * 
 * Device->Serial (32 chars max!)
 *   "$","cmd"," ",data
 */

extern void serial_write_cmd_begin(uint16_t cmd);

extern void serial_write_cmd_end();

extern void serial_write_err_timeout() ;

extern void serial_write_fmt_err();

extern void serial_write_cmd_protover(uint16_t ver);

extern uint8_t serial_read_until(char* buf,char until_char, uint8_t max_size, bool *eol);

extern void serial_read_until_eol(bool *eol);

extern bool serial_read_str_uint32(uint32_t *v, char until_char, bool *eol);

extern bool serial_read_str_byte(uint8_t *v, char untilChar, bool *eol);

extern bool serial_read_str_uint16(uint16_t *v, char untilChar, bool *eol);

extern uint8_t serial_read_str_bytes(uint8_t* buf, uint8_t max_size, char byte_separator, bool *eol);

extern int16_t serial_read_cmd_begin();

#endif
