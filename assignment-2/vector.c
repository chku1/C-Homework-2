#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <search.h>

void VectorNew(vector *v, int elemSize, VectorFreeFunction freeFn, int initialAllocation)
{
    //check the passed information
    assert(elemSize>0);
    assert(initialAllocation>=0);
    if(initialAllocation == 0) initialAllocation = 4;

    //allocate memory on the heap
    v->arr = malloc(elemSize*initialAllocation);

    //give values to vector variables
    v->elem_size_bytes = elemSize;
    v->freefn = freeFn;
    v->allocated_size = initialAllocation;
    v->logical_size = 0;
}

void VectorDispose(vector *v)
{
    if(v->freefn != NULL){
        for (int i = 0; i<v->logical_size; i++){
            v->freefn(((char *)v->arr + i*v->elem_size_bytes));
        }
    }
}

int VectorLength(const vector *v)
{   
    
    if(v->logical_size == '\0')
    {
        return 0;
    }else{
        return v->logical_size;    
    } 
}

void *VectorNth(const vector *v, int position)
{  
    assert(v->logical_size>position && position>=0);
    if(v->logical_size == '\0')
    {
        return NULL;
    }else{
        return (char *)v->arr + position*v->elem_size_bytes;
    }
}

void VectorReplace(vector *v, const void *elemAddr, int position)
{
    //check if position is correct
    assert(v->logical_size>position && position>=0);

    //free the space
    if(v->freefn != NULL){
        v->freefn((char *)v->arr + position*v->elem_size_bytes);
    }

    //write new element
    memcpy((char *)v->arr + position*v->elem_size_bytes,elemAddr,v->elem_size_bytes);
}

void VectorInsert(vector *v, const void *elemAddr, int position)
{
    //check if position is correct
    assert(v->logical_size>=position && position>=0);

    //check if vector has enough length
    if(v->logical_size == v->allocated_size){
        v->allocated_size = v->allocated_size*2;
        v->arr = realloc(v->arr,v->allocated_size*v->elem_size_bytes);
    }

    //shift the elements
    memmove((char *)v->arr + (position+1)*v->elem_size_bytes, (char *)v->arr + position*v->elem_size_bytes,(v->logical_size - position)*v->elem_size_bytes);
   

    //increase the length of vector and insert the element
    v->logical_size++;
    memcpy((char *)v->arr + position*v->elem_size_bytes,elemAddr,v->elem_size_bytes);
}

void VectorAppend(vector *v, const void *elemAddr)
{
    //increase the length
    v->logical_size++;

    //check if vector has enough length
    if(v->logical_size == v->allocated_size){
        v->allocated_size = v->allocated_size*2;
        v->arr = realloc(v->arr,v->allocated_size*v->elem_size_bytes);
    }

    //insert element at the end
    memcpy((char *)v->arr + (v->logical_size-1)*v->elem_size_bytes,elemAddr,v->elem_size_bytes);
}

void VectorDelete(vector *v, int position)
{
    //check if position is correct
    assert(v->logical_size>position && position>=0);

    //delete the element
    if(v->freefn != NULL){
        v->freefn((char *)v->arr + position*v->elem_size_bytes);
    }

    //shift the elements
    memmove((char *)v->arr + position*v->elem_size_bytes, (char *)v->arr + (position+1)*v->elem_size_bytes,(v->logical_size - position)*v->elem_size_bytes);

    //decrease the size
    v->logical_size--;
}

void VectorSort(vector *v, VectorCompareFunction compare)
{
    assert(compare!=NULL);
    qsort(v->arr,v->logical_size,v->elem_size_bytes,compare);
}

void VectorMap(vector *v, VectorMapFunction mapFn, void *auxData)
{
    assert(mapFn!=NULL);
    for(int i=0;i<v->logical_size;i++){
        mapFn(VectorNth(v,i),auxData);
    }
}


int VectorSearch(const vector *v, const void *key, VectorCompareFunction searchFn, int startIndex, bool isSorted)
{ 
    //check the passed information
    assert(startIndex>=0 && v->logical_size>=startIndex);
    assert(searchFn!=NULL && key!=NULL);    

    //search functions
    void * elem = NULL;
     if(isSorted){
         elem = bsearch(key,(char *)v->arr+startIndex*v->elem_size_bytes,v->logical_size-startIndex,v->elem_size_bytes,searchFn);
     }else{
         for(int i=startIndex;i<v->logical_size;i++){

             if(searchFn((char *)v->arr + i*v->elem_size_bytes,key)==0){
                 return i;
             }
         }
     }
    

    //return the answer
    if(elem != NULL){
        return ((char *)elem - (char *)v->arr)/v->elem_size_bytes;
    }else{
        return -1;
    }
}
