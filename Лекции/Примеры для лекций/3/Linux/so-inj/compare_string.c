#include <stdio.h>
#include <string.h>

int main(){
    char input_password[20];
    printf("Hello. What's your password?\n");
    scanf("%19s", input_password);
    if(strcmp(input_password, "password") == 0) {
        printf("Well done\n");
    } else {
        printf(":( Nope, %s is incorrect\n", input_password);
    }
    return 0;
}