#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
int main(){  
    char *pathvar = getenv("LISTENQ");   
    printf("%d\n", getpid());
    printf("pathvar is : %s\n",pathvar);  
    return 0;  
}  