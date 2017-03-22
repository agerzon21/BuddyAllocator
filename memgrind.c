/*
* [PROGRAM]/FILE: memgrind.c
* AUTHOR(s): Aleksandr Gerzon & Tim Katzgrau
* NET ID(s): ag1184 & tjk155
* CLASS: CS214 - Systems Programming
* DATE DUE: October 14, 2016
* -------------------------------------
* FINISHED? 			[YES] NO
* TESTED? (Cases)  		YES [NO]
* WORKS? (ILab) 	 	[YES] NO
*/

#include "mymalloc.h"
#include <sys/time.h>

/* TEST CASE (0) IS ONE OF THE NON-REQUIRED TEST CASES CREATED */

void testCaseE(){
	struct timeval tval_before, tval_after, tval_result;
	gettimeofday(&tval_before, NULL);
	int n;
	char *a; char *b; char *c; char *d; char *e; char *f; char *g; char *h; char *i; char *j; char *k; char *l; char *m;
	
	for(n = 0; n<100; n++){
		a = (char *)malloc(sizeof(char)); *a = '0';
		b = (char *)malloc(sizeof(char)); *b = '1';
		c = (char *)malloc(sizeof(char)); *c = '2';
		d = (char *)malloc(sizeof(char)); *d = '3';
		e = (char *)malloc(sizeof(char)); *e = '4';
		f = (char *)malloc(sizeof(char)); *f = '5';
		g = (char *)malloc(sizeof(char)); *g = '6';
		h = (char *)malloc(sizeof(char)); *h = '7';
		i = (char *)malloc(sizeof(char)); *i = '8';
		j = (char *)malloc(sizeof(char)); *j = '9';
		k = (char *)malloc(sizeof(char)); *k = 'a';
		l = (char *)malloc(sizeof(char)); *l = 'b';
		m = (char *)malloc(sizeof(char)); *m = 'c';
			
		printf("CHARACTERS [ALLOCATED]: | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c |\n", *a,*b,*c,*d,*e,*f,*g,*h,*i,*j,*k,*l,*m);
			
		free(a);
		free(b);
		free(c);
		free(d);
		free(e);
		free(f);
		free(g);
		free(h);
		free(i);
		free(j);
		free(k);
		free(l);
		free(m);
			
		printf("CHARACTERS [FREED]: | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c | %c |\n", *a,*b,*c,*d,*e,*f,*g,*h,*i,*j,*k,*l,*m);
	}

	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf("Time elapsed for Workload E: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("Average for Workload E: %f\n", ((double)((tval_result.tv_usec)*(.000001))+(double)tval_result.tv_sec)/100);	
}

/* TEST CASE (1) MALLOCS 3,000 1-BYTE POINTERS AND THEN FREES THEM ONE BY ONE */

void testCaseA(){
	int i, e,k;
	char *f[3000];
	struct timeval tval_before, tval_after, tval_result;	
	gettimeofday(&tval_before, NULL);
	for(k = 0; k<100; k++){
		for (i = 0; i < 3000; i++) {
			f[i] = (char *)malloc(sizeof(char));
		}
				
		for (e = 0; e < 3000; e++) {
			free(f[e]);
		}
	}
	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf("Time elapsed for Workload A: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("Average for Workload A: %f\n", ((double)((tval_result.tv_usec)*(.000001))+(double)tval_result.tv_sec)/100);	
}

/* TEST CASE (2) MALLOCS 1-BYTE POINTER AND IMMIEDIATELY FREES IT 3,000 TIMES */

void testCaseB(){
		
	int k;
	struct timeval tval_before, tval_after, tval_result;
	char *f;
		
	for(k = 0; k < 3000 ; k++){
		f = (char *)malloc(sizeof(char)); *f = '6';
		free(f);
	}

	int a;
	gettimeofday(&tval_before, NULL);
	for (a = 1; a <= 100; a++) {
		for(k = 0; k < 3000 ; k++){
			f = (char *)malloc(sizeof(char)); *f = '6';
			free(f);
		}

	}
	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf("Time elapsed for Workload B: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("Average for Workload B: %f\n", ((double)((tval_result.tv_usec)*(.000001))+(double)tval_result.tv_sec)/100);
}

/* RANDOMLY CHOOSES BETWEEN MALLOC AND FREEING 3000 TIMES */

void testCaseC(){
	int currentMalloc = 0;
	int i;
    struct timeval tval_before, tval_after, tval_result;


	char *f[3000];
	int freed[3000] = {0};
	srand(time(NULL));
	int is3000 = 0;
	gettimeofday(&tval_before, NULL);
	for(i = 0; i < 100; i++){
		is3000 = 0;
		while(is3000 < 3000){

				currentMalloc = (rand() % 3000);
				int x = rand();
				if(x%2 == 1){
					if(freed[currentMalloc] == 0){
				    	f[currentMalloc] = (char *)malloc(sizeof(char));
				    	freed[currentMalloc] = 1;
				    	is3000++;

				    }
				}else{
					if(freed[currentMalloc] == 1){
						
							free(f[currentMalloc]);
							freed[currentMalloc] = 0;
						
					}

				}

		}
		
		is3000 = 2999;
		while(is3000 > 0){

			if(freed[is3000] == 1){
				free(f[is3000]);
				freed[is3000] = 0;
			}
			is3000--;
		}

	}

	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf("Time elapsed for Workload C: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("Average for Workload C: %f\n", ((double)((tval_result.tv_usec)*(.000001))+(double)tval_result.tv_sec)/100);

} 

void testCaseD(){
	int currentMalloc = 0;
	int i;
    struct timeval tval_before, tval_after, tval_result;


	char *f[3000];
	int freed[3000] = {0};
	srand(time(NULL));
	int is3000 = 0;
	gettimeofday(&tval_before, NULL);
	for(i = 0; i < 100; i++){
		is3000 = 0;
		while(is3000 < 3000){

				currentMalloc = (rand() % 3000);
				int x = rand();
				if(x%2 == 1){

					if(freed[currentMalloc] == 0){
				    	f[currentMalloc] = malloc(sizeof(char)*(1+(x%10)));
				    	freed[currentMalloc] = 1;
				    	is3000++;

				    }
				}else{
					if(freed[currentMalloc] == 1){
						
							free(f[currentMalloc]);
							freed[currentMalloc] = 0;
						
					}

				}

		}
		
		is3000 = 2999;
		while(is3000 > 0){

			if(freed[is3000] == 1){
				free(f[is3000]);
				freed[is3000] = 0;
			}
			is3000--;
		}

	}

	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf("Time elapsed for Workload D: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("Average for Workload D: %f\n", ((double)((tval_result.tv_usec)*(.000001))+(double)tval_result.tv_sec)/100);

} 

/*>>>PLEASE DO NOT USE<<< TEST CASE (5) HAS NOT YET BEEN VETTED >>>PLEASE DO NOT USE<<<*/
void testCaseF(){
		
	int k, l;
	struct timeval tval_before, tval_after, tval_result;
	gettimeofday(&tval_before, NULL);
	for(k = 0; k < 100 ; k++){
		for (l = 0; l <30; l++) {
			char * f = (char *)malloc(sizeof(char)); *f = '6';
			int * g = (int *)malloc(sizeof(int)); *g = 6;
			free(f);
			free(g);
		}
			
	}
	gettimeofday(&tval_after, NULL);
	timersub(&tval_after, &tval_before, &tval_result);
	printf("Time elapsed for Workload F: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
	printf("Average for Workload F: %f\n", ((double)((tval_result.tv_usec)*(.000001))+(double)tval_result.tv_sec)/100);	
	
}


int main() {
		
	testCaseE(); 
	testCaseA(); 
	testCaseB(); 
	testCaseC();
	testCaseD();
	testCaseF();

	
	return 0;

}