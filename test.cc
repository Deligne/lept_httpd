#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include<sys/wait.h>  


int main(){  
	char buf[100];
	fgets(buf, 100, stdin);
	long arg1, arg2;
	sscanf(buf, "%ld%ld", &arg1, &arg2);
	printf("%ld\n", arg2);
    exit(0);
}  