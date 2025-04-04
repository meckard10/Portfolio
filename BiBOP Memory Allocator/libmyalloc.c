/* libmyalloc.c
 * Melody Eckard
 * 
 * Memory allocator library that implements the standard C allocation API. BiBOP (Big Bag 
 * Of Pages) style allocator that uses pages to store blocks of memory.
 *
 * To run a program with the library use the command
 * 	LD_PRELOAD=./libmyalloc.so ./program
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>

#define PAGESIZE 4096

typedef struct page_header{
        int memsize;
        void *nextpage;
	void *freelist;
} Header;

int fd = -1;

//array of pages
void *pages[] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

int headsize = sizeof(Header);

/* This function creates a new page and divides it into sections depending on the
 * given size. It returns a pointer to the start of the page. the sections are linked
 * together and make up the free list. A header provides the size of memory chunks,
 * a pointer to the start of the free list, and a pointer to a next page if there is one.
 */
void *newpage(int size){
        void *next;
        int i = 0;
        int nextsize = sizeof(next);

        //make a page in memory that points to a header
        Header *header = mmap(NULL, PAGESIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

        header->memsize = size;
        header->freelist = (char*)header + headsize;
        header->nextpage = NULL;

	//next pointers point to start of next node
        next = header->freelist;
        for(i = PAGESIZE-headsize;i > size+nextsize;i = i-size-nextsize){
                //shifting the correct amount
                if((i-size-nextsize)<(size+nextsize)){
                        //last one points to null
                        *((void**)next) = NULL;
                }
                else{
                        *((void**)next) = (char*)next + nextsize + size;
                }
                next = *((void**)next);
        }
        //make sure last one points to null

        return header;

}

/* This function is an implementation of malloc using a BiBOP style. It will create and
 * maintain pages broken up into chunks of memory in powers of 2 up to 1024. This malloc
 * will accept a size of memory to be allocated and return a pointer to that block of memory.
 * If a requested amount of memory is larger than 1024 bytes, the appropriate amount of pages
 * will be made for that chunk.
 */
void *malloc(size_t size){
        int i, mysize;
        Header *myheader;
        void *next;
        //to save the pointer to return
        void *ptr = NULL;
	int ptrsize = sizeof(ptr);
	if(fd == -1){
		fd = open("/dev/zero", O_RDWR);
	}
        
        //could make the pages array 2d to save the set sizes for speed [size, ptr]
        if(size <= 0) return NULL;
        else if(size <= 2 && size > 0) i = 0, mysize = 2;
        else if(size <= 4 && size > 2) i = 1, mysize = 4;
        else if(size <= 8 && size > 4) i = 2, mysize = 8;
        else if(size <= 16 && size > 8) i = 3, mysize = 16;
        else if(size <= 32 && size > 16) i = 4, mysize = 32;
        else if(size <= 64 && size > 32) i = 5, mysize = 64;
        else if(size <= 128 && size > 64) i = 6, mysize = 128;
        else if(size <= 256 && size > 128) i = 7, mysize = 256;
        else if(size <= 512 && size > 256) i = 8, mysize = 512;
        else if(size <= 1024 && size > 512) i = 9, mysize = 1024;
	else{
                //too big
                mysize = size;

		ptr = mmap(NULL, mysize+sizeof(mysize)+ptrsize, PROT_READ | PROT_WRITE,
                                        MAP_PRIVATE, fd, 0);
                *((int*)ptr) = mysize;
		ptr = (int*)ptr + 1;
		*((void**)ptr) = NULL;

                //pointer to the data
		return (char*)ptr + ptrsize;
        }

	//finding the correct pointer
        myheader = pages[i];
        //make first page
        if(pages[i] == NULL){
                pages[i] = newpage(mysize);
                myheader = pages[i];
        }
        //need additional page
        while(myheader->freelist == NULL){
                if(myheader->nextpage == NULL){
                        myheader->nextpage = newpage(mysize);
                }
                myheader = myheader->nextpage;
        }
        ptr = myheader->freelist;
        next = *((void**)ptr);
        //relinking the free list
        myheader->freelist = next;
        //set the returned next pointer to point to the header(for free)
        *((void**)ptr) = pages[i];

        //return a pointer to the data section
        return (char*)ptr + ptrsize;
}

/* This function checks a pointer before the given data pointer to find the start of a
 * page's header or to determine if it is a large block. Large blocks are unmapped, and
 * smaller blocks are added back to thier page's freelist.
 */
void free(void *ptr){
	Header *header;
	//does nothing if ptr is NULL
	if(ptr != NULL){
        void *myptr = (char*)ptr - sizeof(myptr);
        if(*((void**)myptr) == NULL){
                //the previous pointer will only be NULL if its a big allocation
                //otherwise it will be a pointer to the header of the page the
                //small memory is on.
		myptr = (int*)myptr - 1;
                int size = *((int*)myptr);
		munmap(myptr, size+sizeof(ptr)+sizeof(size));
        }
        else{
                header = *((void**)myptr);
                //adding freed block to begining of free list
                *((void**)myptr) = header->freelist;
                header->freelist = myptr;
        }
	}
	
}

/* This function allocates all the memory it requires to one block instead of breaking it
 * up by the size of its elements. This design saves on memory space and is simple to write
 * to.
 */
void *calloc(size_t nmemb, size_t size){
	void *ptr, *p;
        int mysize = size * nmemb;
        //the calloced blocks need to be together so allocate a bigger block to
        //accomidate.
        ptr = malloc(mysize);
	//initializd to 0
	p = ptr;
        for(int i = 0; i<nmemb; i++){
                *((void**)p) = 0;
                p = (char*)p + size;
        }
        return ptr;
}

/* This function copies the requested amount of memory to the block size that best 
 * accomidates it, and frees the old memory.
 */
void *realloc(void *ptr, size_t size){
	void *p;
        //make new space
        p = malloc(size);
	if(ptr != NULL){
		//copy memory
		memcpy(p, ptr, size);
        	//free old space
        	free(ptr);
	}
        return p;
}
