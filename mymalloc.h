/*
* PROGRAM/[FILE]: mymalloc.h
* AUTHOR(s): Aleksandr Gerzon & Tim Katzgrau
* NET ID(s): ag1184 & tjk155
* CLASS: CS214 - Systems Programming
* DATE DUE: October 14, 2016
* -------------------------------------
* FINISHED? 			[YES] NO
* LINKED?		  		[YES] NO
* WORKS? (ILab) 	 	[YES] NO
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//make sure the token is not already defined
#ifndef MYMALLOC_H
#define MYMALLOC_H

#define malloc( x ) mymalloc(x, __FILE__, __LINE__ )
#define free( x ) myfree(x, __FILE__, __LINE__ )

//define the size of the memory that sbrk would retrieve (2^9[layers of buddy blocks] * 14[size of metadata])
#define SBRK_GET 7168
//define the size of the metadata 
#define METADATA 14

void * mymalloc(size_t size, char * program, int line);
void myfree(void *x, char * program, int line);
void storeMeta(short address, short size, short parent, short left, short right, short metaCount, short arraySpace);
void freeArray(short ptr, short arraySize);
void updateMeta(short address, short meta);
void updateAlloc(short address);
short storeNode(short address, short size, char * program, int line);
int storeIt(short store, short total, short extra);

#endif