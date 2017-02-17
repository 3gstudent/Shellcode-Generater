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
	unsigned char *BinData = NULL;
	size_t size = 0;	
	BinData = ReadBinaryFile(szFilePath, &size);
	void *sc = VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (sc == NULL)	
		return 0;	
	memcpy(sc, BinData, size);
	(*(int(*)()) sc)();	
	return 0;
}
