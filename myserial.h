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

extern void serial_write_init(uint16_t ver, uint8_t deviceType, uint16_t deviceId);


extern void serial_write_err_not_supported();

extern void serial_write_err_timeout() ;

extern void serial_write_fmt_err();


extern uint8_t serial_read_until(char* buf, unsigned char until_char, uint8_t max_size, bool *eol);

extern void serial_read_until_eol(bool *eol);


extern bool serial_read_str_uint32(uint32_t *v, unsigned char until_char, bool *eol);

extern bool serial_read_str_uint16(uint16_t *v, unsigned char untilChar, bool *eol);

extern bool serial_read_str_byte(uint8_t *v, unsigned char untilChar, bool *eol);

extern uint8_t serial_read_str_bytes(uint8_t* buf, uint8_t max_size, unsigned char byte_separator, bool *eol);


// for external use

extern bool myserial_read_eol; 

extern int16_t myserial_read_cmd_begin();

extern void myserial_read_until_eol();

extern bool myserial_read_uint32(uint32_t *v);

extern bool myserial_read_uint16(uint16_t *v);

extern bool myserial_read_byte(uint8_t *v);

extern uint8_t myserial_read_bytes(uint8_t* buf, uint8_t maxSize);

extern uint8_t myserial_read_str(char* buf, uint8_t maxSize);

#endif
