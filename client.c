#include <stdio.h>

int main(int argc, char** argv){
	if (argc == 1){
		printf("insert client name\n");
		return 0;
	}
	printf("client name : %s\n", argv[1]);
	return 0;
}
