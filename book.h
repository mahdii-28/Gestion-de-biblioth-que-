#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#ifndef BOOK_H
#define BOOK_H
//book type structure
typedef struct {
char id[10];
char title[1001];
char author[1001];
int a_cop;
int t_cop;
}book;
//functions related to book
book create_book(char i[] , char t[] , char a[] , int n );
bool compare_book(book b1 , book b2);
void add_c(book *b , int c);
void remove_c(book *b , int c);
bool reserve_book(book *b );
bool return_book(book *b ) ;
void copy_book(book *b1 , book b2);


#endif
