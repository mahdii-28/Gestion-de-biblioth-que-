#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"str_manip.h"
#include"book.h"
//book creation
book create_book(char i[] , char t[] , char a[] , int n ){
book b;
strcpy(b.id, i);
strcpy(b.title, t);
strcpy(b.author, a);
b.a_cop=n;
b.t_cop=n;
return(b);
}
bool compare_book(book b1 , book b2){
return((strcmp(b1.id,b2.id)==0)&& (strcmp(b1.title,b2.title)==0)&&(strcmp(b1.author,b2.author)==0)&&(b1.a_cop==b2.a_cop)&&(b2.t_cop==b1.t_cop) );
}
//book adding
void add_c(book *b , int c){
b->t_cop+=c;
b->a_cop+=c;
}
//book removing
void remove_c(book *b , int c){
if (b->t_cop < c){
    printf("Pas assez de livres disponibles\n");
}
else if(b->a_cop < c){
  printf("Pas assez de livres disponibles \n");
}
else{
b->t_cop-=c;
b->a_cop-=c;
}
}
//book reservation
bool reserve_book(book *b ){
if (b->a_cop==0){
  printf("Pas assez de livres disponibles\n");
  return(false);
}
else{
 b->a_cop--;
 return(true);
}
}
//book returning
bool return_book(book *b ) {
if (b->a_cop<b->t_cop){
b->a_cop++;
return (true);
}
else {
 printf("ce livre n'est pas possedé par la bibliothéque\n");
 return (false);
}

}
// create a copy of a book (used later in the library)
void copy_book(book *b1 , book b2){
strcpy(b1->author , b2.author);
strcpy(b1->id , b2.id);
strcpy(b1->title , b2.title);
b1->a_cop=b2.a_cop;
b1->t_cop=b2.t_cop;
}
