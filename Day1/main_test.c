#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
	printf("전달된 인자 갯수:%d\n",argc);
	printf("실 행 파 일 : %s\n", argv[0]);
	for( i=1; i<argc; i++){
	printf("전 달 한  데 이 터 : %s\n",argv[i]);
	}
	
	return 0;
}
