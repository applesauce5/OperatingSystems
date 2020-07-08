#ifndef MY_VM_H_INCLUDED
#define MY_VM_H_INCLUDED
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


//Assume the address space is 32 bits, so the max memory size is 4GB
//Page size is 4KB

//Add any important includes here which you may need

#define PGSIZE 4096

// Maximum size of your memory
#define MAX_MEMSIZE ((unsigned long)(4UL*1024*1024*1024))

#define MEMSIZE 1024*1024*1024

// Represents a page table entry
typedef unsigned long pte_t;

// Represents a page directory entry
typedef unsigned long pde_t;

// Page directory
pde_t ** PageDirectory;

//#define TLB_SIZE 

//Structure to represents TLB
struct tlb {
    //Assume your TLB is a direct mapped TLB of TBL_SIZE (entries)
    // You must also define wth TBL_SIZE in this file.
    //Assume each bucket to be 4 bytes
};
struct tlb tlb_store;

// Variables
unsigned int * phyMemStart; //set start of physical memory to null;

// BitMap Functions

#define PGDIR_SHIFT       22
#define PTR_PER_PGTBL  1024

#define PGTBL_MASK          (PTR_PER_PGTBL-1)
#define PGDIR_INDEX(x)   ((x) >> PGDIR_SHIFT)
#define PGTBL_INDEX(x)   (((x) >> PAGE_SHIFT) & PGTBL_MASK)

#define SetBit(A,k)     ( A[(k/32)] |= (1 << (k%32)))
#define ClearBit(A,k)   ( A[(k/32)] &= ~(1 << (k%32)))
#define TestBit(A,k)    ( A[(k/32)] & (1 << (k%32)))

#define Offset(number,k,p)      (((1 << k) - 1) & (number >> (p - 1)))

#define PhyBitMapSize (((MEMSIZE)/(PGSIZE))/32)
//-1 because 2^32 is out of range for unsigned long 
#define VMBitMapSize (((MAX_MEMSIZE-1)/PGSIZE)/32)

int *PhyBitMap;
int *VMBitMap;

// Methods and routines used
void printBits(unsigned int num);
void SetPhysicalMem();
pte_t* Translate(pde_t **pgdir, void *va);
pte_t* get_phy_frame(pde_t **pgdir, void *va, int numPages);
int PageMap(pde_t **pgdir, void *va, void* pa, int numPages);
bool check_in_tlb(void *va);
void put_in_tlb(void *va, void *pa);
void *m_alloc(unsigned int num_bytes);
void a_free(void *va, int size);
void PutVal(void *va, void *val, int size);
void GetVal(void *va, void *val, int size);
void MatMult(void *mat1, void *mat2, int size, void *answer);

#endif
