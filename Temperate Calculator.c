#include <stdio.h>
#include <math.h>
#define UNVALID_DATA -999
#define NUM_MONTH 12
#define MAX_SIZE 50000
#define SCALE 0.5
#define FIVE_YEARS 5
#define TWO 2
int read_data(int *station,int *year,int *month,int *day,
	double *max_temp,double *min_temp);
int year_ave(double *min_temp,double *max_temp,double *year_min_ave,
	double *year_max_ave,int *year_min_day,int *year_max_day,
	int *year_array,int num_day,int *year);
void month_ave(double *min_temp,double *max_temp,double *month_min_ave,
	double *month_max_ave,int *month_min_day,int *month_max_day,
	int num_day,int *month);
int year_score(int *year,int *month,double *min_temp,double *max_temp,
	double *month_min_ave,double *month_max_ave,int num_day,
	int *year_array,int each_year,int *num_month);
void print_stage1(int num_day,int *day,int *month,int *year,
	double *min_temp,double *max_temp);
void print_stage2(int *year_array,double *year_min_ave,
	double *year_max_ave,int *year_min_day,int *year_max_day,int num_year);
void print_stage3(double *month_min_ave,double *month_max_ave,
	int *month_min_day,int *month_max_day);
void print_stage4(int num_year,int *year,int *month,double *min_temp,
	double *max_temp,double *month_min_ave,double *month_max_ave,
	int num_day,int *year_array);
int mygetchar();
int month_score(int each_month,double *month_min_ave,double *month_max_ave,
	double month_min_total,double month_max_total,int min_day,int max_day);
void total_temp(double *total_min,double *total_max,int *min_day,
	int *max_day,double *min_temp,double *max_temp,int each_day);

int main(int argc,char *argv[]){
	double max_temp[MAX_SIZE],min_temp[MAX_SIZE],year_min_ave[MAX_SIZE],
	year_max_ave[MAX_SIZE],month_min_ave[NUM_MONTH-1],
	month_max_ave[NUM_MONTH-1];
	int station[MAX_SIZE],year[MAX_SIZE],month[MAX_SIZE],day[MAX_SIZE],
	year_array[MAX_SIZE],year_min_day[MAX_SIZE],year_max_day[MAX_SIZE],
	month_min_day[NUM_MONTH-1],month_max_day[NUM_MONTH-1],num_year,num_day;
	/* STAGE 1 */
	num_day=read_data(station,year,month,day,max_temp,min_temp);
	print_stage1(num_day,day,month,year,min_temp,max_temp);
	/* STAGE 2 */
	num_year=year_ave(min_temp,max_temp,year_min_ave,year_max_ave,
		year_min_day,year_max_day,year_array,num_day,year);
	print_stage2(year_array,year_min_ave,year_max_ave,year_min_day,
			year_max_day,num_year);
	/* STAGE 3 */
	month_ave(min_temp,max_temp,month_min_ave,month_max_ave,month_min_day,
		month_max_day,num_day,month);
	print_stage3(month_min_ave,month_max_ave,month_min_day,month_max_day);
	/* SATGE 4 */
	print_stage4(num_year,year,month,min_temp,max_temp,month_min_ave,
		month_max_ave,num_day,year_array);
	return 0;
}

/* print_stage1 takes number of lines and the other five arrays and print the
number of records and details of first and last data from the input file */
void print_stage1(int num_day,int *day,int *month,int *year,
	double *min_temp,double *max_temp){
	printf("\nStage 1\n-------\n");
	printf("Input has %d records\n",num_day+1);
	printf("First record in data file:\n  date: %.2d/%.2d/%d\n  min : " 
		"%.1f degrees C\n  max : %.1f degrees C\n",day[0],month[0],year[0],
		min_temp[0],max_temp[0]);
	printf("Last record in data file:\n  date: %d/%d/%d\n  min : "
		"%.1f degrees C\n  max : %.1f degrees C\n",day[num_day],
		month[num_day],year[num_day],min_temp[num_day],max_temp[num_day]);
}
/* print_stage2 takes five arrays that store average minimum and maximum
temperatures, numbers of days those temperature recored and distint years. It
returns average temperatures for each year */
void print_stage2(int *year_array,double *year_min_ave,double 
	*year_max_ave,int *year_min_day,int *year_max_day,int num_year){
	int each_year;
	printf("\nStage 2\n-------");
	for (each_year=0;each_year<=num_year;each_year++){
		printf("\n%d: average min: %5.2f degrees C (%d days)\n      average"
		" max: %5.2f degrees C (%d days)\n",year_array[each_year],
			year_min_ave[each_year],year_min_day[each_year],
			year_max_ave[each_year],year_max_day[each_year]);
	}
}
/* print_stage3 takes arrays of monthly average temperatures and numbers of 
days recorded. Then it prints them out visually month by month, when it 
reaches the end of arrays, prints out the axis */
void print_stage3(double *month_min_ave,double *month_max_ave,
	int *month_min_day,int *month_max_day){
	int space,star,each_month;
	double rounded_min,rounded_max;
	char* month_name[NUM_MONTH]={"Jan","Feb","Mar","Apr","May",
		"Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
	printf("\n\nStage 3\n-------");
	for (each_month=0;each_month<NUM_MONTH;each_month++){
		printf("\n%s (%4d,%4d) |",month_name[each_month],
			month_min_day[each_month],month_max_day[each_month]);
		rounded_min=round(month_min_ave[each_month]);
		rounded_max=round(month_max_ave[each_month]);
		/* Round the average temperatures to the closest 0.5 if it is between
		0.25 and 0.75, to the closest 0.0 if it is not */
		if (rounded_min+SCALE/TWO<month_min_ave[each_month]){
			rounded_min+=SCALE;
		}else if (rounded_min-SCALE/TWO>month_min_ave[each_month]){
			rounded_min-=SCALE;
		}
		if (rounded_max+SCALE/TWO<month_max_ave[each_month]){
			rounded_max+=SCALE;
		}else if (rounded_max-SCALE/TWO>month_max_ave[each_month]){
			rounded_max-=SCALE;
		}
		for (space=1;space<rounded_min/SCALE;space++){
			printf(" ");
		}
		for (star=1;star<=(rounded_max-rounded_min+SCALE)/SCALE;star++){
			printf("*");
		}
	}
	printf("\n                +---------+---------+---------+---------+"
		"---------+---------+\n                0         5        10        "
		"15        20        25        30\n");
}
/* print_stage4 prints the scores for first five years and last five years */
void print_stage4(int num_year,int *year,int *month,double *min_temp,
	double *max_temp,double *month_min_ave,double *month_max_ave,
	int num_day,int *year_array){
	int each_year,num_month,score;
	printf("\nStage 4\n-------\n");
	for (each_year=0;each_year<=num_year;each_year++){
		num_month=0;
		score=year_score(year,month,min_temp,max_temp,month_min_ave,
			month_max_ave,num_day,year_array,each_year,&num_month);
		if (each_year==FIVE_YEARS){				
			printf("  --\n");
		}
		if (each_year<FIVE_YEARS||each_year>num_year-FIVE_YEARS){ 
			printf("  %d: score is %2d/%2d\n",year_array[each_year],score,
				num_month*TWO);
		}
	}
}
/* read_data takes the data input and stores them in six different arrays,
it returns numbers of days (lines) that have been read from the input file */
int read_data(int *station,int *year,int *month,int *day,
	double *max_temp,double *min_temp){
	int num_day=0,location,yy,mm,dd;
	double max,min;
	/* Use mygetchar function to remove first line of data file */
	if (mygetchar()=='P'){
		while (mygetchar()!='\n'){}
	}
	/* The loop makes sure there are six input being read and store them in 
	corresponding arrays */
	while (scanf("IDCJAC0010,%d,%d,%d,%d,%lf,%lf\n",&location,&yy,&mm,
		&dd,&max,&min)==6){
		station[num_day]=location;
		year[num_day]=yy;
		month[num_day]=mm;
		day[num_day]=dd;
		max_temp[num_day]=max;
		min_temp[num_day]=min;
		num_day++;
	}
	return num_day-1;
}
/* mygethar funtion gets rid of '\r'charaters */
int mygetchar(){
	int c;
	while ((c=getchar())=='\r') {}
	return c;
}
/* year_ave computes average minimum and maximum temperatures and store them in
year_max_ave and year_min_ave respectively. It also records the numbers of days
when the minimum and maximum temperatures occured, seperately store them in
year_min_day and year_max_day. It returns number of distinct years recorded */
int year_ave(double *min_temp,double *max_temp,double *year_min_ave,
	double *year_max_ave,int *year_min_day,int *year_max_day,
	int *year_array,int num_day,int *year){
	double year_min_total = 0.0,year_max_total = 0.0;
	int min_day = 0,max_day = 0,each_day,num_year = 0;
	for (each_day=0;each_day<=num_day;each_day++){	
		/* If this year is a different year or in case last data is read */
		if (((year[each_day-1]!=year[each_day])&&each_day!=0)||
			(each_day==num_day)){
			if (each_day==num_day){
				total_temp(&year_min_total,&year_max_total,&min_day,
					&max_day,min_temp,max_temp,each_day);
			}
			year_array[num_year]=year[each_day-1];
			year_min_ave[num_year]=year_min_total/min_day;
			year_max_ave[num_year]=year_max_total/max_day;
			year_min_day[num_year]=min_day;
			year_max_day[num_year]=max_day;
			num_year++;
			year_min_total=0.0;
			year_max_total=0.0;
			min_day=0;
			max_day=0;
		}
		total_temp(&year_min_total,&year_max_total,&min_day,&max_day,
			min_temp,max_temp,each_day);
	}
	return num_year-1;
}
/* month_ave computes monthly average minimum and maximum temperature, at the
same time, tracks numbers of days those temperatures occured, store them in
four different arrays with lengeth of twelve seperately */
void month_ave(double *min_temp,double *max_temp,double *month_min_ave,
	double *month_max_ave,int *month_min_day,int *month_max_day,
	int num_day,int *month){
	double month_min_total,month_max_total;
	int min_day,max_day,each_month,each_day;
	for (each_month=1;each_month<=NUM_MONTH;each_month++){
		month_min_total=0.0;
		month_max_total=0.0;
		min_day=0;
		max_day=0;
		for (each_day=0;each_day<=num_day;each_day++){
			if (month[each_day]==each_month){
				total_temp(&month_min_total,&month_max_total,&min_day,
					&max_day,min_temp,max_temp,each_day);
			}
		}
		/* Calculate the average at the end of each month and record them */
		month_min_ave[each_month-1]=month_min_total/min_day;
		month_max_ave[each_month-1]=month_max_total/max_day;
		month_min_day[each_month-1]=min_day;
		month_max_day[each_month-1]=max_day;
	}
}
/* total_temp adds temperate of each day to the total year or month temperate,
and increments the number of days by one, if the data is valid. It stores those
new values in memory to be accessible by outside functions */
void total_temp(double *total_min,double *total_max,int *min_day,
	int *max_day,double *min_temp,double *max_temp,int each_day){
	if (min_temp[each_day]!=UNVALID_DATA){
		(*total_min)+=min_temp[each_day];
		(*min_day)++;
	}
	if (max_temp[each_day]!=UNVALID_DATA){
		(*total_max)+=max_temp[each_day];
		(*max_day)++;
	}
}
/* yearly_score returns the score for each year by computing average minimum
and maxmum temperatures of each month then comparing them with yearly averages 
stored in arrays from previous stage */
int year_score(int *year,int *month,double *min_temp,double *max_temp,
	double *month_min_ave,double *month_max_ave,int num_day,
	int *year_array,int each_year,int *num_month){
	int min_day=0,max_day=0,each_day,score=0,each_month=0;
	double month_min_total=0.0,month_max_total=0.0;
	for (each_day=0;each_day<=num_day;each_day++){
		/* If this is the same year */
		if (year[each_day]==year_array[each_year]){
			/* If next month is a different month */
			if (month[each_day]!=month[each_day+1]){
				total_temp(&month_min_total,&month_max_total,&min_day,
					&max_day,min_temp,max_temp,each_day);
				/* Increment the total yearly score by score for this month */
				score+=month_score(each_month,month_min_ave,month_max_ave,
					month_min_total,month_max_total,min_day,max_day);
				(*num_month)++;
				each_month++;
				month_min_total=0.0;
				month_max_total=0.0;
				min_day=0;
				max_day=0;
			}else{
			/* If next month is the same month */
			total_temp(&month_min_total,&month_max_total,&min_day,&max_day,
				min_temp,max_temp,each_day);
			}
		}
	}
	return score;
}
/* month_score calulates the average temperatures and return monthly score */
int month_score(int each_month,double *month_min_ave,double *month_max_ave,
	double month_min_total,double month_max_total,int min_day,int max_day){
	int monthly_score=0;
	if (month_min_total/min_day>month_min_ave[each_month]){
		monthly_score++;
	}
	if (month_max_total/max_day>month_max_ave[each_month]){
		monthly_score++;
	}
	return monthly_score;
}