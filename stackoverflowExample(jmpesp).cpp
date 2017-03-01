#include <stdio.h>
#include <windows.h>
#define PASSWORD "1234567"

int verify_password (char *password)
{
	int authenticated;
	char buffer[44];
	authenticated=strcmp(password,PASSWORD);
	strcpy(buffer,password);
	return authenticated;
}


int main()
{
	int valid_flag=0;
	char password[2048];
	FILE *fp;
	LoadLibrary("user32.dll");
	if(!(fp=fopen("shellcode2.bin","rb")))
		return 0;
	fread(password,1627,1,fp);
	valid_flag=verify_password(password);


	if(valid_flag)
	{
		printf("wrong\n");
	}
	else
	{
		printf("right\n");
	}
	fclose(fp);


	return 0;
}

