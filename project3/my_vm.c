#include "my_vm.h"

/*
Function responsible for allocating and setting your physical memory 
*/
void SetPhysicalMem() {
   // printf("Entering SetPhysicalMemory\n");
    //Allocate physical memory using mmap or malloc; this is the total size of
    //your memory you are simulating
    phyMemStart = malloc(MEMSIZE);
    if(phyMemStart == NULL) {
       printf("Physical memory failed to allocate. \n");
       exit(1);
    }
    
    //HINT: Also calculate the number of physical and virtual pages and allocate
    //virtual and physical bitmaps and initialize them
    
    else{
       memset(phyMemStart, 0, MAX_MEMSIZE);	
     //  printf("Start addr of physical memory is: %p\n", phyMemStart);
       PhyBitMap=malloc(sizeof(unsigned int)*PhyBitMapSize);
       memset(PhyBitMap,0,PhyBitMapSize); //Initialize bitMap to 0's

       VMBitMap=malloc(sizeof(unsigned int)*VMBitMapSize);
       memset(VMBitMap,0,VMBitMapSize);
    }
    
    PageDirectory = (pde_t **)malloc(sizeof(pde_t)*1024);    
   // printf("......... Done with SetPhysicalMem ...........\n");
}

/*
 * The function takes a virtual address and page directories starting address and
 * performs translation to return the physical address
*/
pte_t * Translate(pde_t **pgdir, void *va) {
    //HINT: Get the Page directory index (1st level) Then get the
    //2nd-level-page table index using the virtual address.  Using the page
    //directory index and page table index get the physical address
    unsigned int vmaddr  = *(unsigned int *)&va;
    unsigned int vmaddr1 = vmaddr >> 12;
    
    unsigned int pgdir_index = vmaddr1/1024;
    unsigned int pgtab_index = vmaddr1%1024;
    unsigned int location    = pgdir_index + pgtab_index;
    unsigned int offset      = Offset(vmaddr, 12,1);
 //   printf("Index of PageDirectory location.........................................%u\n",location);
    if(TestBit(VMBitMap,location)){
//	printf(">>>>>>>>page directory  .....................%u\n",pgdir_index);
//	printf(">>>>>>>>pgtab index     .....................%u\n",pgtab_index);
//	printf("Value of the virtual location PageDirc[][]........................%u\n",PageDirectory[pgdir_index][pgtab_index]);
     return (pte_t *)(phyMemStart + PageDirectory[pgdir_index][pgtab_index] + offset);
    }
    
    else{
     //If translation not successfull
     return NULL; 
    }
    
    return;
}



/*
The function takes a virtual address and page directories starting address and
performs translation to return the physical address
*/
pte_t * get_phy_frame(pde_t **pgdir, void *va, int numPages) {
    //HINT: Get the Page directory index (1st level) Then get the
    //2nd-level-page table index using the virtual address.  Using the page
    //directory index and page table index get the physical address
    unsigned int vmaddr  = *(unsigned int *)va;
    unsigned int pageDir  = vmaddr >> 22;
    int count = 0,where = -1;
    int i = 0;
    
    while(i<(PhyBitMapSize*32)){		
	if(!(TestBit(PhyBitMap,i))){
		if(count == 0){
			where = i;
		}	
		count++;
	} else if(TestBit(PhyBitMap,i)){
		count=0;
		where=-1;
	} if(count == numPages){
		break;
	}
    	i++;
    }
   
    // where=start of physical address 
    if(where != -1){
      // printf("Physical address found.\n");
      // printf("where is: %d", where);
       unsigned int frame = where * 4096;
       return (void *)frame;
    }// If translation not successfull
    else if(where == -1) {
	return NULL;
    }
}


/*
The function takes a page directory address, virtual address, physical address
as an argument, and sets a page table entry. This function will walk the page
directory to see if there is an existing mapping for a virtual address. If the
virtual address is not present, then a new entry will be added
*/
int PageMap(pde_t **pgdir, void *va, void *pa, int numPages){
    /*HINT: Similar to Translate(), find the page directory (1st level)
    and page table (2nd-level) indices. If no mapping exists, set the
    virtual to physical mapping */
    unsigned int vmaddr  = *(unsigned int *)va;
    unsigned int paaddr  = *(unsigned int *)pa;
    vmaddr = vmaddr >> 12;
    
    unsigned int pgdir_index = vmaddr/1024;
    unsigned int pgtab_index = vmaddr%1024;
    
    if(PageDirectory[pgdir_index] == NULL || PageDirectory[pgdir_index] == 0){
       PageDirectory[pgdir_index] = (pte_t *)malloc(sizeof(pte_t)*PGSIZE);
       
    }
       
       PageDirectory[pgdir_index][pgtab_index] = paaddr;
       
       unsigned int vm_position = vmaddr;
       unsigned int pa_position = paaddr/4096;
       
       unsigned int i;
       for(i=0; i<numPages;i++){
          unsigned int pos1 = i + vm_position;
          SetBit(VMBitMap,  pos1);
          unsigned int pos2 = i + pa_position;
          SetBit(PhyBitMap, pos2);
       }
      // printf("Done in pagemap, printing bits\n");
      // printBits(VMBitMap[0]);
      // printBits(PhyBitMap[0]);
       printf("\n");
       
       return 0;
    
    //return -1;
}

// Helper function to help look into bits in numbers

void printBits(unsigned int num){
	unsigned int size = sizeof(unsigned int);
	unsigned int maxPow = 1<<(size*8-1);
	
	int i = 0;
	while(i < size*8){
		printf("%u ",num&maxPow ? 1 : 0);
		num = num << 1;
		i++;
	}	
	printf("\n");
}

/*
 * Function that gets the next available page
 * This basically loops through the virtual addr bitmaps
 * And finds initial location of numPages contiguous pages
*/
void *get_next_avail(int num_pages) {
    //Use virtual address bitmap to find the next free page
    
    int count = 0;
    int where = -1;
   // printf("------------ In get_next_available --------------\n");
  
    int i = 0;
    
    while(i < (VMBitMapSize*32)){
       if(!(TestBit(VMBitMap,i))){
          if(count == 0){
	     where=i;
          }
	  count++;
       } else if(TestBit(VMBitMap,i)){
          count=0;
	  where = -1;
       }
       if(count == num_pages){
	  // return the virtual address space of where
	  break;
       }
       i++;
    }
    //printf("The value of where is:....%d and i is...: %d. \n", where,i);
    if(where != -1){
     //  printf("Virtual address found.\n");
       unsigned int pageDir = where/1024;
       unsigned int page = where%1024;
       unsigned int tmpAddr = ((pageDir<<10) | page)<<12;
       //printBits(tmpAddr);     
       return (void *)tmpAddr;
    }
    else if(where == -1) {
       // printf("get_next_avail resulted in error. \n");
	return NULL;
    }
}


/* Function responsible for allocating pages
and used by the benchmark
*/
void *m_alloc(unsigned int num_bytes) {

   //HINT: If the physical memory is not yet initialized, then allocate and initialize.
   /* HINT: If the page directory is not initialized, then initialize the
   page directory. Next, using get_next_avail(), check if there are free pages. If
   free pages are available, set the bitmaps and map a new page. Note, you will 
   have to mark which physical pages are used. */
   // printf("m_alloc has been called -- initiated.\n");
    
    // Counting the number of pages necessary
    unsigned int temp = num_bytes;
    int numPages=0;
    numPages = temp/4096;
    if((temp%4096)>0){
    	++numPages;
    }	
    
    if(phyMemStart == NULL || PageDirectory == NULL){
    	SetPhysicalMem();
    }
    
   // printf("m_alloc has been called -- processing.\n");
   // printf("Number of pages calculated is: %d\n", numPages);
    
    void * startOffree = get_next_avail(numPages);
   // printf("This allocation is starting from: %x\n", startOffree);
    
    void * phyAddr = get_phy_frame(PageDirectory, &startOffree, numPages);
   // printf("returned from get_next_avail\n");
    int worked = PageMap(PageDirectory,&startOffree,&phyAddr, numPages);
   // printf("returned from PageMap\n");
    unsigned int first   = *(unsigned int *)&startOffree;
    unsigned int second  = *(unsigned int *)&phyAddr;
    
   // printf("----------\n");
   // printf("addr is ..............: %x\n", (first+second));

    
    return (void*)((int)(first & second));
    
}

/* Responsible for releasing one or more memory pages using virtual address (va)
*/
void a_free(void *va, int size) {
  //  printf("\n");
  //  printf("---------- Start of one free -----------\n");
 //   printBits(VMBitMap[0]);
 //   printBits(PhyBitMap[0]);
    //Free the page table entries starting from this virtual address (va)
    // Also mark the pages free in the bitmap
    //Only free if the memory from "va" to va+size is valid
    unsigned int vmaddr  = *(unsigned int *)&va;
    vmaddr = vmaddr >> 12;
 //   printf("vmaddr in free is: %d\n", vmaddr);
    
    unsigned int pgdir_index = vmaddr/1024;
    unsigned int pgtab_index = vmaddr%1024;
    
    unsigned int temp = size;
    int numPages=0;
    numPages = temp/4096;
    if((temp%4096)>0){
    	++numPages;
    }
   // printf("num_pages is: %d\n", numPages);
    
    if( (vmaddr+(unsigned int)numPages) > (VMBitMapSize*32)){ 
      return;
    }
    
    int f;
    for(f=vmaddr; f<(vmaddr+(unsigned int)numPages); f++){
       unsigned int pgdir_index = vmaddr/1024;
       unsigned int pgtab_index = vmaddr%1024;
       
       unsigned int paaddr = PageDirectory[pgdir_index][pgtab_index];
    //   printf("paaddr in free is: %d.\n", paaddr);
       
       //clear bit in VMBitMap & PhyBitMap
       ClearBit(VMBitMap, vmaddr);
       ClearBit(PhyBitMap,(paaddr/4096));
       PageDirectory[pgdir_index][pgtab_index] = 0;
       memset((phyMemStart + (paaddr*4096)),0,4096);
    }
    
   // printBits(VMBitMap[0]);
   // printBits(PhyBitMap[0]);
   // printf("---------- Done with one free -----------\n");
   // printf("\n");
    return;
}


/* The function copies data pointed by "val" to physical
 * memory pages using virtual address (va)
*/
void PutVal(void *va, void *val, int size) {

    /* HINT: Using the virtual address and Translate(), find the physical page. Copy
       the contents of "val" to a physical page. NOTE: The "size" value can be larger
       than one page. Therefore, you may have to find multiple pages using Translate()
       function.*/
    int numPages = size/4096;
    if(size%4096!=0){
	numPages++;
    }
    
    unsigned int vmaddr  = *(unsigned int *)&va;
    unsigned int addr = vmaddr >> 12;
    unsigned int pgdir_index = addr/1024;
    unsigned int pgtab_index = addr%1024;
    unsigned int location    = pgdir_index + pgtab_index;
    //printf("Here is the location .......................................%u\n",location);
    if( (vmaddr+(unsigned int)numPages) > (VMBitMapSize*32)){ 
        return;
    }
    /**
    if(PageDirectory[pgdir_index][pgtab_index] == 0){
    	return;
    }
    **/
    
    int v;
    for(v=0;v<numPages;v++){
      unsigned int loc = location + v;
      if(!TestBit(VMBitMap,loc)){
//	printf("Sir issac was executed.........................................\n");
         return;
      }
    }
    
    int r, si;
    int s = size;
    int count = 0;
    pte_t * phyAddr = NULL;
    for(r=0; r<numPages; r++){
      pte_t * valocation = (pte_t *)va + r*4096;
      phyAddr = Translate(PageDirectory,(void *)valocation);
      if(s > 4096){
        memcpy(phyAddr, (val + count), 4096);
        count+=4096;
        s-=4096;
      }
      else{
        memcpy(phyAddr, (val + count), s);
        count+= s;
      } 
    }
    return;
    
}


/*Given a virtual address, this function copies the contents of the page to val*/
void GetVal(void *va, void *val, int size) {
    /* HINT: put the values pointed to by "va" inside the physical memory at given
    "val" address. Assume you can access "val" directly by derefencing them.
    If you are implementing TLB,  always check first the presence of translation
    in TLB before proceeding forward */
    int numPages = size/4096;
    if(size%4096!=0){
	numPages++;
    }
    
    unsigned int vmaddr  = *(unsigned int *)&va;
    unsigned int addr = vmaddr >> 12;
    unsigned int pgdir_index = addr/1024;
    unsigned int pgtab_index = addr%1024;
    unsigned int location    = pgdir_index + pgtab_index;
    
    if( (vmaddr+(unsigned int)numPages) > (VMBitMapSize*32)){ 
        return;
    }
    /**
    if(PageDirectory[pgdir_index][pgtab_index] == 0){
    	return;
    }
    **/
    
    int v;
    for(v=0;v<numPages;v++){
      unsigned int loc = location + v;
      if(!TestBit(VMBitMap,loc)){
//	printf("Sir Issac 2 was printed! ............................................\n");
         return;
      }
    }
    
    int r, si;
    int s = size;
    int count = 0;
    
    for(r=0; r<numPages; r++){
      pte_t * valocation = (pte_t *)va + r*4096;
      pte_t * phyAddr = Translate(PageDirectory,(void *)valocation);
      if(s > 4096){
        memcpy((val+count), phyAddr, 4096);
        count+=4096;
	s-=4096;
      }
      else{
        memcpy((val + count), phyAddr, s);
        count+= s;
      } 
    }
    
    return;
    
    
}



/*
This function receives two matrices mat1 and mat2 as an argument with size
argument representing the number of rows and columns. After performing matrix
multiplication, copy the result to answer.
*/
void MatMult(void *mat1, void *mat2, int size, void *answer) {

    /* Hint: You will index as [i * size + j] where  "i, j" are the indices of the
    matrix accessed. Similar to the code in test.c, you will use GetVal() to
    load each element and perform multiplication. Take a look at test.c! In addition to 
    getting the values from two matrices, you will perform multiplication and 
    store the result to the "answer array"*/
    unsigned int i, j, k, y, z;
    unsigned int ans = 0;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            ans = 0;
            for(k=0; k < size; k++){
              unsigned int address_a = (unsigned int)mat1 + ((i * size * sizeof(int))) + (k * sizeof(int));
              unsigned int address_b = (unsigned int)mat2 + ((k * size * sizeof(int))) + (j * sizeof(int));
              GetVal((void *)address_a, &y, sizeof(int));
              GetVal((void *)address_b, &z, sizeof(int));
              ans+= y * z;
            }
             unsigned int ans_pos = (unsigned int)answer + ((i* size * sizeof(int))) + (j * sizeof(int)); 
             PutVal((void*)ans_pos, &ans, sizeof(int));
        }
    }   
    
}
