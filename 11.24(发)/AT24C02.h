#ifndef __AT24C02_H__
#define __AT24C02_H__

#define uchar unsigned char

void AT24C02_WriteByte(uchar WordAddress,Data);
uchar AT24C02_ReadByte(uchar WordAddress);

#endif