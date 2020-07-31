/*
 * ex11.c
 * 
 * Copyright 2020 Unknown <guest@porteus>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define ARRAY_SIZE 1000

int main_a()
{
	int numbers[ARRAY_SIZE];
	int i, j, max, max_temp;
	time_t t;
	srand((unsigned) time(&t));
	for(i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand() % 256;
	}
	
	max = numbers[0];
	for(i = 0; i  < 5; i++){
		if(fork() == 0){
			max = numbers[i * (ARRAY_SIZE/5)];
			for(j = i * (ARRAY_SIZE/5) + 1; j < (i + 1) * (ARRAY_SIZE/5); j++){
				if(numbers[j] > max){
					max = numbers[j];
				}
			}
			exit(max);
		} else {
			wait(&max_temp);
			if(WEXITSTATUS(max_temp) > max){
				max = WEXITSTATUS(max_temp);
			}
		}
	}
	printf("\n%d\n", max);
	return 0;
}

int main_b()
{
	int numbers[ARRAY_SIZE], result[ARRAY_SIZE];
	int i, j, max, max_temp;
	time_t t;
	srand((unsigned) time(&t));
	for(i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand() % 256;
	}
	
	max = numbers[0];
	for(i = 0; i  < 5; i++){
		if(fork() == 0){
			max = numbers[i * (ARRAY_SIZE/5)];
			for(j = i * (ARRAY_SIZE/5) + 1; j < (i + 1) * (ARRAY_SIZE/5); j++){
				if(numbers[j] > max){
					max = numbers[j];
				}
			}
			exit(max);
		} else {
			wait(&max_temp);
			if(WEXITSTATUS(max_temp) > max){
				max = WEXITSTATUS(max_temp);
			}
		}
	}
	printf("%d\n", max);
	
	if(fork() == 0){
		for(i = 0; i < ARRAY_SIZE / 2; i++){
			result[i]=((int) numbers[i]/max)*100;
			printf("%d, ", result[i]);
		}
	}
	return 0;
}

int main_c()
{
	int numbers[ARRAY_SIZE], result[ARRAY_SIZE];
	int i, j, max, max_temp;
	time_t t;
	srand((unsigned) time(&t));
	for(i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand() % 256;
	}
	
	max = numbers[0];
	for(i = 0; i  < 5; i++){
		if(fork() == 0){
			max = numbers[i * (ARRAY_SIZE/5)];
			for(j = i * (ARRAY_SIZE/5) + 1; j < (i + 1) * (ARRAY_SIZE/5); j++){
				if(numbers[j] > max){
					max = numbers[j];
				}
			}
			exit(max);
		} else {
			wait(&max_temp);
			if(WEXITSTATUS(max_temp) > max){
				max = WEXITSTATUS(max_temp);
			}
		}
	}
	printf("%d\n", max);
	
	if(fork() == 0){
		for(i = 0; i < ARRAY_SIZE / 2; i++){
			result[i]=((int) numbers[i]/max)*100;
			printf("%d, ", result[i]);
		}
	} else {
		for(i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++){
			result[i]=((int) numbers[i]/max)*100;
			printf("%d, ", result[i]);
		}
	}
	return 0;
}

int main_d()
{
	int numbers[ARRAY_SIZE], result[ARRAY_SIZE];
	int i, j, max, max_temp;
	time_t t;
	srand((unsigned) time(&t));
	for(i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand() % 256;
	}
	
	max = numbers[0];
	for(i = 0; i  < 5; i++){
		if(fork() == 0){
			max = numbers[i * (ARRAY_SIZE/5)];
			for(j = i * (ARRAY_SIZE/5) + 1; j < (i + 1) * (ARRAY_SIZE/5); j++){
				if(numbers[j] > max){
					max = numbers[j];
				}
			}
			exit(max);
		} else {
			wait(&max_temp);
			if(WEXITSTATUS(max_temp) > max){
				max = WEXITSTATUS(max_temp);
			}
		}
	}
	printf("%d\n", max);
	
	if(fork() == 0){
		for(i = 0; i < ARRAY_SIZE / 2; i++){
			result[i]=((int) numbers[i]/max)*100;
			printf("%d, ", result[i]);
		}
	} else {
		wait(0);
		for(i = ARRAY_SIZE / 2; i < ARRAY_SIZE; i++){
			result[i]=((int) numbers[i]/max)*100;
			printf("%d, ", result[i]);
		}
	}
	return 0;
}
