/*
* [PROGRAM]/FILE: mymalloc.c
* AUTHOR(s): Aleksandr Gerzon & Tim Katzgrau
* NET ID(s): ag1184 & tjk155
* CLASS: CS214 - Systems Programming
* DATE DUE: October 14, 2016
* -------------------------------------
* FINISHED?       [YES] NO
* TESTED? (Cases)     YES [NO]
* WORKS? (ILab)     [YES] NO
*/

//header file with all includes, functions, and other definitions
#include "mymalloc.h"

//global block of memory from which malloc will dynamically allocate memory from
static char memBlock[SBRK_GET];

//keeps track of how many nodes are in the list
int listSize = 0;

//frees an array at a address
void freeArray(short ptr, short arraySize) {
  if (*((short*)&memBlock[ptr]) == 0) {
    return;
  }
  
 int i;
  
  //clears the array
  for (i = 0; i < arraySize; i++) {
    if ((ptr + i) < SBRK_GET)
      memBlock[ptr + i] = '\0';
    else {
      printf("2::::: FATAL ERROR IN ARRAY SIZE!!!\n");
    }
  }
}

//frees a node
void freeNode(short ptr) {
  if (*((short*)&memBlock[ptr]) == 0) {
    return;
  }
  
  int i;
  
  //clears the METADATA
  for (i = (METADATA - 1); i >= 0; i--) {
    if ((ptr - i) < SBRK_GET)
      memBlock[ptr - i] = '\0';
    else {
      printf("1:::: FATAL ERROR IN ARRAY SIZE!!!\n");
    }
  }
}

//updates metaCounter value of each node
void updateMeta(short address, short meta) {
  if (*((short*)&memBlock[address - 10]) != 0) {
    (*((short*)&memBlock[address - 4])) += meta;
    updateMeta(*((short*)&memBlock[address - 10]), meta);
  }
  else {
    (*((short*)&memBlock[address - 4])) += meta;
  }
}

//updates the space remaining in each Node
void updateSpace(short address, short update) {
  if (*((short*)&memBlock[address - 10]) != 0) {
    (*((short*)&memBlock[address - 2])) -= (update);
    updateSpace(*((short*)&memBlock[address - 10]), update);
  }
  else {
    (*((short*)&memBlock[address - 2])) -= (update);
  }
}

//recursive method that sees whether or not children can be combined after one is freed
void recFree(short address) {
  short LChild = (*((short*)&memBlock[address - 8]));
  short RChild = (*((short*)&memBlock[address - 6]));
  short RChildSpace = ((*((short*)&memBlock[RChild - 2])) / METADATA);
  short RChildMeta = (*((short*)&memBlock[RChild - 4]));
  short RChildSize = ((*((short*)&memBlock[RChild - 12])) / METADATA);
  short LChildSpace = ((*((short*)&memBlock[LChild - 2])) / METADATA);
  short LChildMeta = (*((short*)&memBlock[LChild - 4]));
  short LChildSize = ((*((short*)&memBlock[LChild - 12])) / METADATA);
  
  //if both Nodes have no data stored
  if ((RChildSize - RChildMeta == RChildSpace) && (LChildSize - LChildMeta == LChildSpace)){
    //update the space and meta of the parent
    updateMeta(address, 2);
    updateSpace(address, (2 * METADATA));
    //clear the Left Child
    freeNode(*((short*)&memBlock[address - 8]));
    (*((short*)&memBlock[address - 8])) = 0;
    listSize--;
    //clear the Right Child
    freeNode(*((short*)&memBlock[address - 6]));
    (*((short*)&memBlock[address - 6])) = 0;
    listSize--;
    //check if node has parent
    if (*((short*)&memBlock[address - 10]) != 0) {
      recFree(*((short*)&memBlock[address - 10]));
    }
    else {
      freeNode(address);
      listSize--;
    }
  }
}

//recursive functipn that adds a new node to the tree
short storeNode(short address, short size, char * program, int line) {
  
  //TOTAL BLOCK SIZE - (METACOUNT * METADATA)
  short spaceMeta = ((*((short*)&memBlock[address - 2])) / METADATA);
  short retFin = 0;
  short savAd = address;
  
  //check if there is any space remaining within the current accessed node
  if (spaceMeta == 0) {
    return 0;
  }
  
  //check to make sure current accessed node does NOT have children
  else if (*((short*)&memBlock[address - 4]) <= 2) {
    //check if [spaceLeft >= size >= spaceLeft/2]
    if (((*((short*)&memBlock[address - 2])) >= size) && (((*((short*)&memBlock[address - 2])) / 2) <= size)) {
      //if its not the first node, check if it already has occupied value
      updateSpace(address, (spaceMeta * METADATA));
      return address;
    }
    //check if child nodes can be created and hold memory (if not they are pointless and only hold their own metadata)
    else if (((*((short*)&memBlock[address - 2])) <= METADATA * 3)) {
      if (size >= METADATA * 2) {
        return 0;
      }
      else {
        updateSpace(address, (spaceMeta * METADATA));
        return address;
      }
    }
    //otherwise assign children and check the children
    else {
      short tempLeft = address + METADATA;
      short tempRight = ((*((short*)&memBlock[address - 12])) / 2) + address - ((*((short*)&memBlock[address - 4])) * METADATA) + METADATA;
      //left = address + METADATA;
      *((short*)&memBlock[address - 8]) = tempLeft;
      //right = blockSize/2 + address - (metaCount * METADATA)
      *((short*)&memBlock[address - 6]) = tempRight;
      //create left child (node)
      storeMeta(tempLeft, ((*((short*)&memBlock[address - 12])) / 2), address, 0, 0, (*((short*)&memBlock[address - 4])) + 1, (((*((short*)&memBlock[address - 12])) / 2) - ((*((short*)&memBlock[address - 4])) * METADATA) - (METADATA)));
      //increment memory
      listSize++;
      //create right child (node)
      storeMeta(tempRight, ((*((short*)&memBlock[address - 12])) / 2), address, 0, 0, 1, ((*((short*)&memBlock[address - 12])) / 2) - (METADATA));
      //increment memory
      listSize++;
      updateMeta(address, 2);
      updateSpace(address, (2 * METADATA));
      retFin = storeNode((short)tempRight, (short)size, program, line);
      //check which of the two children is unallocated
      if (retFin == 0) {
        retFin = storeNode((short)tempLeft, (short)size, program, line);
      }
      return retFin;
    }
  }
  
  //if node has children, check where the data best fits, checking the right child first, then the left child
  else {
    savAd = address;
    retFin = storeNode(*((short*)&memBlock[savAd - 6]), (short)size, program, line);
    //check which of the two children is unallocated
    if (retFin == 0) {
      retFin = storeNode(*((short*)&memBlock[savAd - 8]), (short)size, program, line);
    }
    return retFin;
  }
}

//stores all individual data within the StoreMeta method and updates its ExtraSize in the process
int storeIt(short store, short total, short extra) {
  
  int newExtra;
  *((short*)&memBlock[total + extra]) = store;
  newExtra = extra + sizeof(store);
  return newExtra;
  
}

//copies the given METADATA into the apporpriate place within the array
void storeMeta(short address, short size, short parent, short left, short right, short metaCount, short arraySpace) {
  
  int extraSize = 0;
  int totSize = (address - METADATA);
  
  //store the address
  //STORAGE OF ADDRESS: memory[address - 14]
  extraSize = storeIt(address, totSize, extraSize);
  //store the size
  //STORAGE OF BLOCK SIZE: memory[address - 12]
  extraSize = storeIt(size, totSize, extraSize);
  //store the parent
  //STORAGE OF PARENT: memory[address - 10]
  extraSize = storeIt(parent, totSize, extraSize);
  //store the left child
  //STORAGE OF LEFT CHILD: memory[address - 8]
  extraSize = storeIt(left, totSize, extraSize);
  //store the right child
  //STORAGE OF RIGHT CHILD: memory[address - 6]
  extraSize = storeIt(right, totSize, extraSize);
  //store the count of metadata
  //STORAGE OF METADATA COUNT: memory[address - 4]
  extraSize = storeIt(metaCount, totSize, extraSize);
  //store the size of the storing array
  //STORAGE OF ARRAY SPACE: memory[address - 2]
  extraSize = storeIt(arraySpace, totSize, extraSize);
  //STORAGE OF ARRAY: memory[address]
  
}

//malloc function which allocates dynamic memory from sbrk using the size of the object given
void *mymalloc(size_t size, char * program, int line) {
  //check to make sure that the memory being requested is less than the total memory available
  if (size > SBRK_GET) {
    printf("ERROR: Not Enough Memory!\nLine: %d; Program: %s\n", line, program);
    exit(1);
  }
  
  if (size == 0)
    return NULL;
  
  //if a list has not been started, create a new root node
  if (listSize == 0) {
    storeMeta(METADATA, SBRK_GET, 0, 0, 0, 1, SBRK_GET - METADATA);
    listSize++;
  }
  
  short usedBlocked = size;
  short modded = 0;
  short dataSize;
  
  if (usedBlocked%METADATA != 0) {
    
    usedBlocked = size%METADATA;
    usedBlocked = size - usedBlocked;
    modded = 1;
    
  }
  
  dataSize = ((usedBlocked/METADATA) + modded);
  
  short retAdd = storeNode((short)(METADATA), (dataSize * METADATA), program, line);
  
  /***************************************************************************************
  * TO PRINT EVERY TIME THERE IS NO MEMORY LEFT, UNCOMMENT THE PRINT F LINE
  * FOR EASIER OUTPUT, THIS LINE IS COMMENTED OUT
  ***************************************************************************************/
  if (retAdd == 0) {
    
    //printf("(^)ERROR: Not Enough Memory!\nLine: %d; Program: %s\n", line, program);
    return NULL;
    
  }
  
  //prints out the address at which the array will begin for the returned data
  //printf("-----------------------------\n|   FINISHED MALLOC: %d   |\n-----------------------------\n", *((short*)&memBlock[retAdd - METADATA]));
  //prints out the number of nodes currently present within the tree
  //printf("---------------------\n|     NODES: %d     |\n---------------------\n", listSize);

  return &memBlock[retAdd];
  
}

//free function which frees previously allocated memory
void myfree(void *x, char * program, int line) {
  
  //if an address has not been allocated do not free it
  if (x == NULL)
    return;
  
  //find where the METADATA starts
  x -= (METADATA);
  //convert it to a short so the data can be accessed by the program
  short freeAdd = *((short*)x);
  
  //check the size of the allocated array (in METADATA's)
  short arraySize = ((*((short*)&memBlock[freeAdd - 12])) / METADATA) - (*((short*)&memBlock[freeAdd - 4]));
  //retrieve all the attributes of the address to be freed;
  short temptemp = (*((short*)&memBlock[freeAdd - 10]));
  if (temptemp != 0) {
    updateSpace(freeAdd, ((arraySize * METADATA) * (-1)));
    freeArray(freeAdd, arraySize);
    recFree(temptemp);
  }
  else {
    freeArray(freeAdd, arraySize);
    freeNode(freeAdd);
    listSize--;
  }
  
  //print the remaining amount of nodes in the list
  //printf("------------------------------\n|     NODES REMAINING: %d     |\n------------------------------\n", listSize);
  //Notifiy the user that the free was correctly run with no errors
  //printf("---------------------\n|   FINISHED FREE   |\n---------------------\n");
  
  return;
  
}