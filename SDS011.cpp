#include "SDS011.h"

SDS011::SDS011()
{
  uart = new SoftwareSerial(9, 8);
}

void SDS011::Read(float* p10, float* p25)
{
  uint8_t buff;
  //float p10;
  //float p25;
  int value;
  int len = 0;
  int pm10_serial = 0;
  int pm25_serial = 0;
  int checksum_is;
  int checksum_ok = 0;
  int error = 1;
  while ((uart->available() > 0) && (uart->available() >= (10-len))) {
    buff = uart->read();
    value = int(buff);
    switch (len) {
      case (0): if (value != 170) { len = -1; }; break;
      case (1): if (value != 192) { len = -1; }; break;
      case (2): pm25_serial = value; checksum_is = value; break;
      case (3): pm25_serial += (value << 8); checksum_is += value; break;
      case (4): pm10_serial = value; checksum_is += value; break;
      case (5): pm10_serial += (value << 8); checksum_is += value; break;
      case (6): checksum_is += value; break;
      case (7): checksum_is += value; break;
      case (8): if (value == (checksum_is % 256)) { checksum_ok = 1; } else { len = -1; }; break;
      case (9): if (value != 171) { len = -1; }; break;
    }
    len++;

  
    if (len == 10 && checksum_ok == 1) {
      *p10 = (float)pm10_serial/10.0;
      *p25 = (float)pm25_serial/10.0;
      len = 0; checksum_ok = 0; pm10_serial = 0.0; pm25_serial = 0.0; checksum_is = 0;
      error = 0;
      //Serial.println(p10);
      //Serial.println(p25);
    }
}
    
}

void SDS011::Sleep()
{
  for (uint8_t i = 0; i < 19; i++) {
    uart->write(SLEEPCMD[i]);
  }

uart->flush();
  while (uart->available()>0) {
  }
}

void SDS011::Wake()
{
  for (uint8_t i = 0; i < 19; i++) {
    uart->write(WAKECMD[i]);
  }

uart->flush();
  while (uart->available()>0) {
  }
}
