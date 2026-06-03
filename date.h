#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"str_manip.h"
#ifndef DATE_H
#define DATE_H
//date type structure
typedef struct{
int year;
int month;
int day;
}date;
// comp data structure
typedef enum{
C_EQUAL ,
C_GREATER,
C_LESS}comp;

//functions related to date
bool ver_cor_form(date d);
date create_date(int y , int m , int d  );
void repr_date(date d , char output[]);
comp compare(date d1 , date d2);
void copy_date(date *d1 , date d2);
#endif
