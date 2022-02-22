#include <inc/memlayout.h>
#include <kern/kheap.h>
#include <kern/memory_manager.h>
struct allocated_mallocs
{
	unsigned int size_of_malloc;
    unsigned int start_address_malloc;
    unsigned int last_address_inmalloc;
};
static struct allocated_mallocs mallocsData[100];
unsigned int start_of_new_kmalloc = KERNEL_HEAP_START;
//unsigned int last_address_allocated = 0;
int malloc_count = 0;

void* kmalloc(unsigned int size)
{
	//cprintf("2wl Address 5ales %d\n ",size);
	//TODO: [Task 2021 - Kernel Heap] [1] kmalloc()
	// Write your code here, remove the panic and write your code
	//panic("kmalloc() is not implemented yet...!!");
	//refer to the project documentation for details.

	//change this "return" according to your answer
	//unsigned int kol_mara_start_gedida = start_of_new_kmalloc;
	unsigned int start_of_heap = start_of_new_kmalloc;
	unsigned int new_size = ROUNDUP(size + start_of_new_kmalloc,PAGE_SIZE);
	unsigned int va = 0;
	if(KERNEL_HEAP_MAX - start_of_heap > size)//a5r kernel
	   {
	for  (va = start_of_heap; va < new_size; va += PAGE_SIZE)
	{
		unsigned int *ptr_page_table = NULL;
		struct Frame_Info *frame = get_frame_info(ptr_page_directory, (void *)va, &ptr_page_table);
		int ret = allocate_frame(&frame);
	    ret = map_frame(ptr_page_directory, frame, (void*)va, PERM_WRITEABLE);
	}
	//cprintf("Address 1 %d\n ",start_of_new_kmalloc);
	//cprintf("Address 2 %d\n ",va);
	    start_of_new_kmalloc = va; //keda deh bdayt kallocate gedida
	    mallocsData[malloc_count].start_address_malloc = start_of_heap;
	    mallocsData[malloc_count].size_of_malloc = new_size;
	    mallocsData[malloc_count].last_address_inmalloc = va;
	   // cprintf("Address 1 %d\n ",mallocsData[malloc_count].start_address_malloc);
	    //cprintf("Address 2 %d\n ",mallocsData[malloc_count].size_of_malloc);
	    malloc_count++;
		return (void*)start_of_heap;
	   }
	return NULL;
}

void kfree(void* virtual_address)
{
	//TODO: [Task 2021 - Kernel Heap] [2] kfree()
	// Write your code here, remove the panic and write your code
	//panic("kfree() is not implemented yet...!!");

	//you need to get the size of the given allocation using its address
	//refer to the project presentation and documentation for details
	 //hna blf 3la kol malloc 7gztha w lw la2et el va harou7 a3ml unmap mn 3ndha
	unsigned int size = 0;
	for (int i = 0 ; i < malloc_count; i++)
	{
		if(mallocsData[i].start_address_malloc == (int)virtual_address)
		{
			int size =  mallocsData[i].size_of_malloc;
			for (int va = (int)virtual_address ; va < size; va+=PAGE_SIZE)
		    {
				unsigned int *ptr_page_table= NULL;
			   	struct Frame_Info *frame2 = get_frame_info(ptr_page_directory,(void*)va,&ptr_page_table);
			    unmap_frame(ptr_page_directory,(void*)va);
		    }
		mallocsData[i].size_of_malloc = 0;
        mallocsData[i].start_address_malloc = 0;
		}
	}
}

unsigned int kheap_virtual_address(unsigned int physical_address)
{
	//TODO: [Task 2021 - Kernel Heap] [3] kheap_virtual_address()
	// Write your code here, remove the panic and write your code
	//panic("kheap_virtual_address() is not implemented yet...!!");

	//return the virtual address corresponding to given physical_address
	//refer to the project documentation for details

	//change this "return" according to your answer
    //cprintf("malloc count %d\n",malloc_count);

	unsigned int last_address = mallocsData[malloc_count-1].last_address_inmalloc;
	unsigned int *ptr_page_table1 = NULL;
	struct Frame_Info *frame1 = to_frame_info(physical_address);
	//uint32* ptr_new_dir = K_VIRTUAL_ADDRESS(physical_address);
	//to frame info deh elmfrod bta5od physical address w btrg3 pointer 3lframe deh
	//only takes 40% from kernel  loop through last addrees et7gz
    for (unsigned int va = KERNEL_HEAP_START; va <= last_address; va+=PAGE_SIZE)
    {
    	unsigned int *ptr_page_table2 = NULL;
    	struct Frame_Info *frame2 = get_frame_info(ptr_page_directory,(void*)va,&ptr_page_table2);
    		if(frame2 == frame1)
    	{
    		return va;
    	}


    }
	return 0;
}

unsigned int kheap_physical_address(unsigned int virtual_address)
{
	//TODO: [Task 2021 - Kernel Heap] [4] kheap_physical_address()
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");

	//return the physical address corresponding to given virtual_address
	//refer to the project documentation for details

	//change this "return" according to your answer

	unsigned int last_address = mallocsData[malloc_count-1].last_address_inmalloc;
	unsigned int *ptr_page_table1 = NULL;
	struct Frame_Info *frame1 = get_frame_info(ptr_page_directory, (void *)virtual_address,&ptr_page_table1);
	if(frame1 != NULL)
	{
		unsigned int pa = to_physical_address(frame1);
		return pa;
	}

	/*for (unsigned int va = KERNEL_HEAP_START; va <= last_address; va+=PAGE_SIZE)
	{
		unsigned int *ptr_page_table2 = NULL;
	    struct Frame_Info *frame2 = get_frame_info(ptr_page_directory, (void *)va, &ptr_page_table2);
	    if(frame2 == frame1 && frame2 != NULL && frame1 != NULL)
	    {
	    	unsigned int pa = to_physical_address(frame2);
	    	return pa;
	    }
	}*/ // evaluation = 80% //Very slow
	return 0;
}
