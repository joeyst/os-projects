#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MEM_SIZE 16384  // MUST equal PAGE_SIZE * PAGE_COUNT
#define PAGE_SIZE 256  // MUST equal 2^PAGE_SHIFT
#define PAGE_COUNT 64
#define PAGE_SHIFT 8  // Shift page number this much

#define PTP_OFFSET 64 // How far offset in page 0 is the page table pointer table

// Simulated RAM
unsigned char mem[MEM_SIZE];

//
// Convert a page,offset into an address
//
int get_address(int page, int offset)
{
    return (page << PAGE_SHIFT) | offset;
}

//
// Initialize RAM
//
void initialize_mem(void)
{
    memset(mem, 0, MEM_SIZE);

    int zpfree_addr = get_address(0, 0);
    mem[zpfree_addr] = 1;  // Mark zero page as allocated
}

//
// Get the page table page for a given process
//
unsigned char get_page_table(int proc_num)
{
    int ptp_addr = get_address(0, PTP_OFFSET + proc_num);
    return mem[ptp_addr];
}

/*
GetPhysicalAddress(proc_num, virtual_addr):
    Get the virtual page (see code above)
    Get the offset
    Get the physical page from the page table

    Build the physical address from the phys page and offset

    Return it
*/
int GetPhysicalAddress(int proc_num, int virtual_addr) {
  int virtual_page = virtual_addr >> 8;
  int offset = virtual_addr & 255;

  int process_page_table_physical_address = get_page_table(proc_num);
  int address_of_physical_page_number = process_page_table_physical_address + virtual_page;
  int physical_page_number = mem[address_of_physical_page_number];

  return get_address(physical_page_number, offset);
}

int AllocatePage(void) 
{
  for (int page_number=0; page_number < PAGE_COUNT; page_number++) {
    if (mem[page_number] == 0) {
      mem[page_number] = 1;
      return page_number;
    }
  }
  return 0xff;
}

void DeallocatePage(int p)
{
  mem[p] = 0;
}

//
// Allocate pages for a new process
//
// This includes the new process page table and page_count data pages.
//
void new_process(int proc_num, int page_count)
{
    // Get the page table page
    int process_page_table_physical_page_number = AllocatePage();
    if (process_page_table_physical_page_number == 0xff) {
        printf("OOM: proc %d: page table\n", proc_num);
    }

    // Set this process's page table pointer in zero page
    mem[64 + proc_num] = process_page_table_physical_page_number;

    for (int virtual_page_number_of_data_page = 0; virtual_page_number_of_data_page < page_count; virtual_page_number_of_data_page++) {
      int process_new_data_page_physical_page_number = AllocatePage();
      if (process_new_data_page_physical_page_number == 0xff) {
          printf("OOM: proc %d: data page\n", proc_num);
      }

      int address_to_store_mapping_at = get_address(process_page_table_physical_page_number, virtual_page_number_of_data_page);
      mem[address_to_store_mapping_at] = process_new_data_page_physical_page_number;
    }
}

//
// Print the free page map
//
// Don't modify this
//
void print_page_free_map(void)
{
    printf("--- PAGE FREE MAP ---\n");

    for (int i = 0; i < 64; i++) {
        int addr = get_address(0, i);

        printf("%c", mem[addr] == 0? '.': '#');

        if ((i + 1) % 16 == 0)
            putchar('\n');
    }
}

//
// Print the address map from virtual pages to physical
//
// Don't modify this
//
void print_page_table(int proc_num)
{
    printf("--- PROCESS %d PAGE TABLE ---\n", proc_num);

    // Get the page table for this process
    int page_table = get_page_table(proc_num);

    // Loop through, printing out used pointers
    for (int i = 0; i < PAGE_COUNT; i++) {
        int addr = get_address(page_table, i);

        int page = mem[addr];

        if (page != 0) {
            printf("%02x -> %02x\n", i, page);
        }
    }
}

//
// Main -- process command line
//
int main(int argc, char *argv[])
{
    assert(PAGE_COUNT * PAGE_SIZE == MEM_SIZE);

    if (argc == 1) {
        fprintf(stderr, "usage: ptsim commands\n");
        return 1;
    }
    
    initialize_mem();

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "pfm") == 0) {
            print_page_free_map();
        }
        else if (strcmp(argv[i], "ppt") == 0) {
            int proc_num = atoi(argv[++i]);
            print_page_table(proc_num);
        }
        // `np n m` command. 
        else if (strcmp(argv[i], "np") == 0) {
          int process_number = atoi(argv[i + 1]);
          int number_of_pages = atoi(argv[i + 2]);
          // Adding two to `i` so that the for loop doesn't iterate over the `n` and `m` arguments. 
          i = i + 2;
          new_process(process_number, number_of_pages);
        }

        // TODO: more command line arguments
    }
}
