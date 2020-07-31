/*
 * ex12.c
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

int spawn_childs(int n){
   int i;
   for (i = 0; i < n; i++){
      if (fork() == 0){
         return 2 * (i + 1);
      }
   }
}

int main()
{
	int nChilds = 6;
	int n;

	n = spawn_childs(nChilds);

	if (n != 0){
		printf("Valor de saida do filho %d é: %d\n", n/2, n);
	}

	return 0;
}

