#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"str_manip.h"
//'\n'removal
void remove_nl(char str[]) {
int i=0;
while (str[i]!='\0'){
    if (str[i]=='\n'){
        str[i]='\0';
        break;
    }
    else{
        i++;
    }
}
return;
}
//character replacer
void char_replacer(char str[],char Red ,char Ring){
for (int i=0 ; str[i]!='\0' ; i++){
    if (str[i]==Red){
        str[i]=Ring;
    }
}
return;
}
//add ".txt" to a string
void txt_adder(char str[]){
strcat(str , ".txt");
return;
}
//turn all letters uppercaser
void uppercaser(char str[]){
for (int i=0 ; str[i]!='\0' ;  i++){
    if (('a'<=str[i]) && (str[i]<='z')){
        str[i]-='a'-'A';
    }
}
return;
 }
//file complete
