
#include <stdio.h>
#include <string.h>

void prepareNew()
{
	FILE* file;
	
	remove("db.dat");
	file = fopen("db.dat", "w+b");
	file = freopen("db.dat","r+b",file);
	
	fclose(file);
}

int main(void)
{
	FILE* file;
	
	prepareNew();
	
	file = fopen("db.dat","r+b");

	fclose(file);
	return 0;
}
