

void buildtable16(void)
{
	unsigned short i, j;
	unsigned short data, reg;

	//printf("unsigned int crc16_table[256] = {");
	for (i = 0; i < 256; i++) {
		reg = 0;
		data = i << 8;
		for (j = 0; j < 8; j++) {
			if ((data ^ reg) & 0x8000)
				reg = (reg << 1) ^ 0x1021;
			else
				reg <<= 1;

			data <<= 1;
		}
		//if ((i % 8) == 0)
		//	printf("\n\t");

		//printf("0x%04x, ", reg);
	}
	//printf("\n};\n");
}

static inline unsigned short shitfer(unsigned char ch, unsigned short init)
{
	unsigned char mask;
	unsigned int crc = init;

	mask = 0x80;
	while (mask > 0) {
		crc <<= 1;

		if (ch & mask)
			crc += 1;

		mask >>= 1;
		if (crc > 0xffff) {
			crc &= 0xffff;
			crc ^= 0x1021;
		}
	}

	return (unsigned short)(crc & 0xffff);
}

unsigned short crc16(const unsigned char *buffer, int len)
{
	unsigned short crc;
	unsigned char *p = buffer;

	crc = 0;
	while (len-- > 0)
		crc = shitfer(*p++, crc);

	crc = shitfer(0, crc);
	crc = shitfer(0, crc);

	return crc;
}
/*
int main()
{
	buildtable16();
	printf("crc16(\"123456789\") = 0x%x\n", crc16("123456789", 9));
	return 0;
}*/
