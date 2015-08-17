#ifndef __MYSERIAL_PROTOCOL_COMMON_H__
#define __MYSERIAL_PROTOCOL_COMMON_H__

#define SERIAL_TOKEN_PC  '#'
#define SERIAL_TOKEN_MASTER '$'
#define SERIAL_TOKEN_FLD_SEPARATOR ';'

// PROTOCOL VERSION
// Serial -> Device
#define SERIAL_CMD_PROTOVER   1  // => master protocol_version (uint16)
// Reports protocol version
//   value[0] word = SERIAL_PROTOVER
//   value[1] byte = DEVICE_TYPE
//   value[2] word = DEVICE_ID

// Device -> Serial
#define SERIAL_CMD_ERROR_IO     200
// Command error
//   value[0] string text

#define SERIAL_CMD_ERROR_FMT    201
// Command values format error
//   value[0] string text

#define SERIAL_CMD_ERROR_NOT_SUPPORTED  202
// Command values format error
//   value[0] string text

#endif