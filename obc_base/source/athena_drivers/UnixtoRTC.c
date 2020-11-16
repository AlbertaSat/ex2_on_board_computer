//Year is current year (2000 is 0)
//Month is current month (January is 1)
//Weeks is weeks passed already
//days is current day (cannot be 0)
//hours, minutes and seconds all start at 0




#include <stdio.h>

int main(){
    int unixt;
    int years;
    int months;
    int days;
    int hrs;
    int mins;
    int secs;
    int weeks;
    printf("Unix time: ");
    scanf("%d", &unixt);
    unixt = unixt-946684800;

    //Finds out how many years have already passed
    for (years=0; unixt>=0; years++){
        if (years%4==0){
            unixt = unixt-366*24*3600;
        }else{
            unixt = unixt-365*24*3600;
        }
    }

    years = years-1;    
    if (years%4==0){
        unixt = unixt+366*24*3600;
    }else{
        unixt = unixt+365*24*3600;
    }

    //How many additional days have passed?
    days=unixt/86400;
    unixt = unixt-86400*days;

    //How many hours
    hrs = unixt/3600;
    unixt = unixt-3600*hrs;

    //how many minutes
    mins = unixt/60;
    unixt = unixt-60*mins;

    //Now, convert those days to months
    for (months = 1; days>=0; months++){
        if (months<=7){
            if (months%2==0){
                days = days-30;
                if (months == 2){
                    days = days+1;
                    if (years%4!=0){
                        days = days+1;
                    }
                }
            }else{
                days = days-31;
            }
            }else{
                if (months%2==0){
                    days = days-31;
                }else{
                    days = days-30;
                }
            }
    }
    months = months-1;
    if (months<=7){
    if (months%2==0){
        days = days+30;
        if (months == 2){
            days = days-1;
            if (years%4!=0){
                days = days-1;
            }
        }
    }else{
        days = days+31;
    }
    }else{
        if (months%2==0){
            days = days+31;
        }else{
            days = days+30;
        }
    }
    weeks=days/7;
    days = days%7;
    days++; //Display the current day

    printf("Years: %d, Months: %d, Weeks: %d, Days: %d, Hours: %d, Minutes: %d, Seconds: %d \n", years, months, weeks, days, hrs, mins, unixt);
    
