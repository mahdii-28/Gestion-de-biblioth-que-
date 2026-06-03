#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"str_manip.h"
#include"date.h"

// verifying that the date provided is correct (EX : no 44/13/2023 or 30/02/2024)
bool ver_cor_form(date d) {
    // Basic checks
    bool Ryear = (d.year >= 1800);
    bool Rmonth = (d.month >= 1 && d.month <= 12);
    bool Rday = (d.day >= 1 && d.day <= 31);
    if (!(Ryear && Rmonth && Rday)) return false;

    // Month-specific day validation
    switch (d.month) {
        case 4: case 6: case 9: case 11:
            return (d.day <= 30);  // April, June, Sept, Nov have max 30 days
        case 2: {
            bool isLeap = (d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0);
            return (d.day <= (isLeap ? 29 : 28));
        }
        default:
            return true; // months with 31 days
    }
}
//date creation
date create_date(int y , int m , int d  ){
date l;
l.day=d;
l.month=m;
l.year=y;
return(l);
}
//putting the date in the format "day/month/year"
void repr_date(date d, char output[]) {
    output[0] = '\0';  // start with empty string
    char dc[3], mc[3], yc[5];

    // Format day and month with leading zero if needed
    snprintf(dc, sizeof(dc), "%02d", d.day);
    snprintf(mc, sizeof(mc), "%02d", d.month);
    snprintf(yc, sizeof(yc), "%04d", d.year);

    // Concatenate into output string
    strcat(output, dc);
    strcat(output, "/");
    strcat(output, mc);
    strcat(output, "/");
    strcat(output, yc);
}

//comparing 2 dates
comp compare(date d1 , date d2){
if(d1.year<d2.year){
    return(C_LESS);
}
else if (d1.year>d2.year){
    return(C_GREATER);
}
else if (d1.month<d2.month){
    return(C_LESS);
}
else if(d1.month>d2.month){
    return(C_GREATER);
}
else if (d1.day<d2.day){
    return(C_LESS);
}
else if (d1.day>d2.day){
   return(C_GREATER);
}
else{
    return(C_EQUAL);
}
}
//copies a date
void copy_date(date *d1 , date d2){
d1->day=d2.day;
d1->month=d2.month;
d1->year=d2.year;
}
//file complete
