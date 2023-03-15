#include "hashset.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void HashSetNew(hashset *h, int elemSize, int numBuckets,
		HashSetHashFunction hashfn, HashSetCompareFunction comparefn, HashSetFreeFunction freefn)
{
	//check the passed values
	assert(elemSize>0 && numBuckets>0 && hashfn!=NULL && comparefn !=NULL);

	//assign value to the variables
	h->elem_size_bytes = elemSize;
	h->num_buckets = numBuckets;
	h->hashsetfn = hashfn;
	h->comparefn = comparefn;
	h->freefn = freefn;
	h->hash = malloc(numBuckets*sizeof(vector));


	//create the vectors
	for(int i=0;i<h->num_buckets;i++){
		VectorNew(&h->hash[i],elemSize,freefn,0);
	}
	
}

void HashSetDispose(hashset *h)
{
	if(h->freefn!=NULL){
		for(int i=0;i<h->num_buckets;i++){
			VectorDispose(&h->hash[i]);
		}
	}
	free(h->hash);
}

int HashSetCount(const hashset *h)
{ 
	if(h->num_buckets == '\0'){
		return 0;
	} else{
		int hashset_count;
		for(int i=0;i<h->num_buckets;i++){
			hashset_count+= VectorLength(&h->hash[i]);
		}
		return hashset_count;
	}
}

void HashSetMap(hashset *h, HashSetMapFunction mapfn, void *auxData)
{
	assert(mapfn!=NULL);
	for(int i=0;i<h->num_buckets;i++){
		VectorMap(&h->hash[i],mapfn,auxData);
	}
}

void HashSetEnter(hashset *h, const void *elemAddr)
{
	//check variables
	assert(elemAddr!=NULL);
	int hashnum = h->hashsetfn(elemAddr,h->num_buckets);
	assert(hashnum < h->num_buckets && hashnum>=0);

	//search if it already exists
	int index = VectorSearch(&h->hash[hashnum],elemAddr,h->comparefn,0,false);

	//enter new hash
	if(index != -1){
		VectorReplace(&h->hash[hashnum],elemAddr,index);
	}else{
		VectorInsert(&h->hash[hashnum],elemAddr,0);
	}

}

void *HashSetLookup(const hashset *h, const void *elemAddr)
{ 	
	//check variables
	assert(elemAddr!=NULL);
	int hashnum = h->hashsetfn(elemAddr,h->num_buckets);
	assert(hashnum < h->num_buckets && hashnum>=0);

	//search for index
	int index = VectorSearch(&h->hash[hashnum],elemAddr,h->comparefn,0,false);

	//return index
	if(index == -1){
		return NULL; 
	}else{
		return VectorNth(&h->hash[hashnum],index);
	}
}
