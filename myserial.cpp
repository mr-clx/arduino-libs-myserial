#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifndef SERIAL_WAIT_MS
#define SERIAL_WAIT_MS 1500 // 1500 msec to wait byte
#endif

#include "myserial.h"
#include "myserial_protocol_common.h"

void serial_write_cmd_begin(uint16_t cmd) {
	Serial.write(SERIAL_TOKEN_MASTER);
	// printf("%d;", cmd);
	Serial.print(cmd);
	Serial.print(SERIAL_TOKEN_FLD_SEPARATOR);
}

void serial_write_cmd_end() {
	Serial.println();
}


void serial_write_init(uint16_t ver, uint8_t deviceType, uint16_t deviceId) {
	serial_write_cmd_begin(SERIAL_CMD_PROTOVER);
	Serial.print(ver);
	Serial.print(SERIAL_TOKEN_FLD_SEPARATOR);
	Serial.print(deviceType);
	Serial.print(SERIAL_TOKEN_FLD_SEPARATOR);
	Serial.print(deviceId);
	serial_write_cmd_end();
}

void serial_write_err_timeout() {
	serial_write_cmd_begin(SERIAL_CMD_ERROR_FMT);
	Serial.print(F("READ_TIMEOUT"));
	serial_write_cmd_end();
}

void serial_write_fmt_err(){
	serial_write_cmd_begin(SERIAL_CMD_ERROR_IO);
	Serial.print(F("FMT_ERROR"));
	serial_write_cmd_end();
}

void serial_write_err_not_supported() {
	serial_write_cmd_begin(SERIAL_CMD_ERROR_NOT_SUPPORTED);
	Serial.print(F("NOT_SUPPORTED"));
	serial_write_cmd_end();
}



uint8_t serial_read_until(char* buf, unsigned char until_char, uint8_t max_size, bool *eol) {
  uint8_t i=0;
  uint8_t charsLeft = min(max_size, 255); // read only up to 255 chars
  
  bool e=*eol; //Serial.print(" EOL="); Serial.println(e);
 
  while (!e && charsLeft) {

    // wait data available (todo: optimize)
    uint32_t wait_start=millis();
    uint32_t ms=wait_start;
    while(!Serial.available()) {
      if ((uint32_t)abs(ms-wait_start)>=SERIAL_WAIT_MS) {
        //Serial.print(" time elapsed=");  Serial.println(ms-wait_start));
        serial_write_err_timeout();
        *eol=true;
        return 0;
      } else {
        ms = millis();
      }
    }
    
    byte c = Serial.read(); // todo: check timeout !

    //Serial.print(" read char="); Serial.println(c);

    e=(c=='\n' || c<' ' || c==0 /*|| c=='.'*/);
    if (c==until_char || e) {
      // Serial.println("  EOL");//  Serial.println(ci);
      
      if (buf && i<max_size) 
        buf[i]=0; // write end of str
      break;
    } else {
      if (buf && i<max_size) {
        buf[i]=c;
	  }
      i++;
    }
  
    charsLeft--;
  }
  
  *eol=e;
  // Serial.print("  bytes=");  Serial.println(i);
  // Serial.print("len="); Serial.print(strlen(buf)); Serial.print(" ");
  // Serial.print("buf="); Serial.println(buf);
  
  
  return i;
}

// read string until it's end
// close command read
void serial_read_until_eol(bool *eol)
{
	serial_read_until(NULL, char(0), 255, eol);
}


bool serial_read_str_uint32(uint32_t *v, unsigned char until_char, bool *eol) {
	// up to 5 chars
	char buf[8];
	uint8_t wasRead=serial_read_until(&buf[0], until_char, sizeof(buf)-1, eol);
	if (!wasRead)
	return false;

	uint32_t n = 0;
	uint32_t base = 1; // dec
	wasRead--;
	while (wasRead!=255) {
		char c = buf[wasRead];
		if (c<'0' || c>'9') {
			serial_write_fmt_err;
			return false;
		}
		n += (c-'0')*base;
		base *=10; // next base degree
		wasRead--;
	}
	// Serial.print("v="); Serial.println(v);
	*v=n;
	return true;
	// todo: timeout
}


bool serial_read_str_uint16(uint16_t *v, unsigned char untilChar, bool *eol) {
   uint32_t n=*v;
   bool r = serial_read_str_uint32(&n, untilChar, eol); // trim fields
   // todo: check range
   *v = (uint16_t) n&0xFFFF;
   return r;
}

// todo: read hex
// read byte value from serial
bool serial_read_str_byte(uint8_t *v, unsigned char untilChar, bool *eol) {
   uint32_t n=*v;
   bool r = serial_read_str_uint32(&n, untilChar, eol); // trim fields
   // todo: check range
   *v = (byte) n&0xFF;
   return r;
}


// todo read long hex string
uint8_t serial_read_str_bytes(uint8_t* buf, uint8_t max_size, unsigned char byte_separator, bool *eol) {
  bool e=false;
  uint8_t i=0;
  while (i<max_size && !e) 
  {
    serial_read_str_byte(&buf[i], byte_separator, &e);
    i++;
  }
  *eol=e;
  return i;
}


bool myserial_read_eol=0; // for external use

//
// begin read command token+cmd code
//
int16_t myserial_read_cmd_begin() {
	myserial_read_eol=false;
	while (Serial.available()) {
		char c = Serial.read();
		if (c==SERIAL_TOKEN_PC) {
			uint8_t cmd;
			//bool eol=false;
			if (!serial_read_str_byte(&cmd, SERIAL_TOKEN_FLD_SEPARATOR, &myserial_read_eol) || myserial_read_eol)
			return -1;
			else return cmd;
			// return Serial.parseInt();
		}
	}
}


void myserial_read_until_eol() {
	if (!myserial_read_eol) serial_read_until(NULL, (char) 0, 255, &myserial_read_eol);
}

bool myserial_read_uint32(uint32_t *v) {
	if (myserial_read_eol) return false;
	return serial_read_str_uint32(v, SERIAL_TOKEN_FLD_SEPARATOR, &myserial_read_eol); 
}

bool myserial_read_uint16(uint16_t *v) {
	if (myserial_read_eol) return false;
	return serial_read_str_uint16(v, SERIAL_TOKEN_FLD_SEPARATOR, &myserial_read_eol); 
}

bool myserial_read_byte(uint8_t *v) {
	if (myserial_read_eol) return false;
	return serial_read_str_byte(v, SERIAL_TOKEN_FLD_SEPARATOR, &myserial_read_eol); 
}

uint8_t myserial_read_bytes(uint8_t* buf, uint8_t maxSize) {
	if (myserial_read_eol) return false;
	return serial_read_str_bytes(buf, maxSize, SERIAL_TOKEN_FLD_SEPARATOR, &myserial_read_eol);
}

uint8_t myserial_read_str(char* buf, uint8_t maxSize) {
	return serial_read_until(buf, SERIAL_TOKEN_FLD_SEPARATOR, maxSize, &myserial_read_eol);
}

