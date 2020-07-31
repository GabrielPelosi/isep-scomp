/*
 * ex09.c
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
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	int i, j, start;
	
	for(i = 0; i < 10; i++){
		if(fork() == 0){
			start = i * 100 + 1;
			for(j = start; j < start + 100; j++){
				 printf("%d, ", j);
			}
			exit(0);
		} else {
			wait(0);
		}
	}
	return 0;
}

