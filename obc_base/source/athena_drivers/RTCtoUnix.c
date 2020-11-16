#include <stdio.h>

//Assumes: Month is current month, day is current day (in week, or in month if week is set to 0). Year is current year
//minutes is minutes already passed, hrs is hrs already passed. Same with weeks
int addmonth(int unixt, int year, int month);
int checkLeap(int year, int days, int month);
int main(){
    //First, calculate seconds sincce UNIX time 0
    int unixt;
    int years;
    int months;
    int week;
    int days;
    int mins;
    int hr;
    int sec;
    printf("Years");
    scanf("%d", &years);
    printf("Enter months");
    scanf("%d", &months);
    printf("Enter weeks");
    scanf("%d", &week);
    printf("Days");
    scanf("%d", &days);
    printf("Enter mins");
    scanf("%d", &mins);
    printf("Enter hrs");
    scanf("%d", &hr);
    printf("Enter secs");
    scanf("%d", &sec);
    unixt = 946684800; //Unix time Jan 1, 2000
    unixt += (((((week*7)+days-1)*24)+hr)*60+mins)*60+sec;
    unixt += addmonth(unixt,years,months);

}

int addmonth(int unixt,int year,int month){

    int days = 0;
    days=checkLeap(year, days, month);
    days+= (month-1)*30.5+0.5;
    days+=365*(year);
    unixt+=days*24*3600; 
    printf("%d", unixt);  


}

int checkLeap(int year,int days, int month){
    //Checks if the current year is a leap year. If it is, return -2. If not, return -1
    //Also accounts for past leap years (adds one day for each)
    if (month>2){
        if (year%4!=0){
            days = days-2;
        }else{
            days = days -1;
    }}
    if (year>0){
        days = days + (year-1)/4+1;
    }
    if (month==9){
        days++;
    }

    if (month==11){
        days++;
    }

    return(days);
}
