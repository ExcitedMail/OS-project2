#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main(){
	int wid = 1000;
	srand(time(NULL));

	for(int i = 0; i < wid; i++){
		for(int j = 0; j < rand()%100+10; j++){
			printf("%c", (char)(rand()%26+65));
		}
		printf("\n");
	}
	
	return 0;
}
