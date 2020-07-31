/*
 * ex10.c
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

#define ARRAY_SIZE 2000
#define CHILD 10
int main()
{
	int numbers[ARRAY_SIZE];
	int i, j, n, index;
	time_t t;
	srand ((unsigned) time (&t));
	for (i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand () % 100;
	}
	n = rand() % 100;
	
	for(i = 0; i < CHILD; i++){
		if(fork() == 0){
			for(j = 0; j < (ARRAY_SIZE / CHILD); j++){
				if(numbers[i * (ARRAY_SIZE / CHILD) + j] == n){
					exit(j);
				}
			}
			exit(255);
		} else {
			wait(&index);
			if(WEXITSTATUS(index) == 255){
				printf("%d not found\n", n);
			} else {
				printf("%d found in %d\n", n, WEXITSTATUS(index));
			}
		}
	}
	return 0;
}

