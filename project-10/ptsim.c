#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MEM_SIZE 16384  // MUST equal PAGE_SIZE * PAGE_COUNT
#define PAGE_SIZE 256  // MUST equal 2^PAGE_SHIFT
#define PAGE_COUNT 64
#define PAGE_SHIFT 8  // Shift page number this much
#define PAGE_MASK (pow(PAGE_SHIFT, 2) - 1)

#define PTP_OFFSET 64 // How far offset in page 0 is the page table pointer table

int verbose = 0;

// Simulated RAM
unsigned char mem[MEM_SIZE];

//
// Convert a page,offset into an address
//
int get_address(int page, int offset)
{
    return (page << PAGE_SHIFT) | offset;
}

int get_physical_page_number_of_process_page_table(int proc_num) {
  int i = mem[64 + proc_num];
  if (verbose) {
    printf("Physical page number of process page table: %i\n", i);
  }
  return i;
}

int get_value_from_page_and_offset(int page, int offset) {
  return mem[get_address(page, offset)];
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
    // printf("mem[%i]: %i\n", ptp_addr, mem[ptp_addr]);
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
  int virtual_page = virtual_addr >> PAGE_SHIFT;
  int offset = virtual_addr & 255;

  int process_page_table_physical_address = get_page_table(proc_num);
  int address_of_physical_page_number = process_page_table_physical_address + virtual_page;
  int physical_page_number = mem[address_of_physical_page_number];

  return get_address(physical_page_number, offset);
}

void StoreValue(int proc_num, int virt_addr, int value) {
  int phys_addr = GetPhysicalAddress(proc_num, virt_addr);
  mem[phys_addr] = value;
  printf("Store proc %d: %d => %d, value=%d\n",
    proc_num, virt_addr, phys_addr, value);
}

int LoadValue(int proc_num, int virt_addr) {
  int phys_addr = GetPhysicalAddress(proc_num, virt_addr);
  int value = mem[phys_addr];
  printf("Load proc %d: %d => %d, value=%d\n",
    proc_num, virt_addr, phys_addr, value);
  return value; 
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
  if (verbose) {
    printf("mem[%i] = 0;\n", p);
  }
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
    if (verbose) {
      printf("Process page table physical page number: %i\n", process_page_table_physical_page_number);
      printf("mem[%i] = %i;\n", 64 + proc_num, process_page_table_physical_page_number);
    }
    mem[64 + proc_num] = process_page_table_physical_page_number;



    for (int virtual_page_number_of_data_page = 0; virtual_page_number_of_data_page < page_count; virtual_page_number_of_data_page++) {
      int process_new_data_page_physical_page_number = AllocatePage();
      // printf("Process new data page physical page number: %i\n", process_new_data_page_physical_page_number);
      if (process_new_data_page_physical_page_number == 0xff) {
          printf("OOM: proc %d: data page\n", proc_num);
      }

      int address_to_store_mapping_at = get_address(process_page_table_physical_page_number, virtual_page_number_of_data_page);
      if (verbose) {
        printf("Storing physical page %i at index %i of process %i\n", process_new_data_page_physical_page_number, virtual_page_number_of_data_page, proc_num);
        printf("Address associated: %i\n", address_to_store_mapping_at);
      }
      mem[address_to_store_mapping_at] = process_new_data_page_physical_page_number;
    }
}

void KillProcess(int proc_num) 
{
  // Gets memory address of the process's page table 
  int process_page_table_physical_page_number = get_page_table(proc_num);
  int process_page_table_memory_address = get_address(process_page_table_physical_page_number, 0);

  // printf("get_page_table(%i); => %i\n", proc_num, get_page_table(proc_num));

  // printf("Process %i page table has page table memory address of %i\n", proc_num, get_page_table(proc_num));
  // printf("process_page_table_memory_address: %i", process_page_table_memory_address);
  // for (int offset = 0; offset < 64; offset++) {
  //   int mem_addr = process_page_table_memory_address + offset;
  //   printf("Memory address %i: %i\n", mem_addr, mem[mem_addr]);
  // }

  // Iterates through all 64 bytes of that page that hold the process's mappings 
  for (int offset = 0; offset < 64; offset++) {
    // Gets the physical page number at that offset by accessing the original memory 
    // address plus the offset. 
    int process_curr_physical_page_number = mem[process_page_table_memory_address + offset]; 
    // If that page number isn't `0`, then deallocates it. 
    if (process_curr_physical_page_number != 0) {
      // printf("DeallocatePage(%i)\n", process_curr_physical_page_number);
      DeallocatePage(process_curr_physical_page_number);
    }
  }
  // Deallocates the original page. 
  DeallocatePage(process_page_table_physical_page_number);
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

        else if (strcmp(argv[i], "kp") == 0) {
          KillProcess(atoi(argv[i + 1]));
          i = i + 1;
        }

        else if (strcmp(argv[i], "sb") == 0) {
          StoreValue(atoi(argv[i + 1]), atoi(argv[i + 2]), atoi(argv[i + 3]));
          i = i + 3;
        }

        else if (strcmp(argv[i], "lb") == 0) {
          LoadValue(atoi(argv[i + 1]), atoi(argv[i + 2]));
          i = i + 2;
        }

        // TODO: more command line arguments
    }
}

/*
Expected output of ./ptsim np 1 2 pfm np 2 3 pfm kp 1 pfm np 3 5 pfm ppt 3 kp 3 pfm 

%  ./ptsim np 1 2 pfm np 2 3 pfm kp 1 pfm np 3 5 pfm ppt 3 kp 3 pfm
--- PAGE FREE MAP ---
####............          [Zero page and process 1 allocated]
................
................
................
--- PAGE FREE MAP ---
########........          [And process 2 added on]
................
................
................
--- PAGE FREE MAP ---
#...####........          [Process 1 killed]
................
................
................
--- PAGE FREE MAP ---
###########.....          [Process 3 created]
................
................
................
--- PROCESS 3 PAGE TABLE ---
00 -> 02
01 -> 03
02 -> 08                  [See the jump in physical page numbers]
03 -> 09
04 -> 0a
--- PAGE FREE MAP ---
#...####........          [Process 3 killed]
................
................
................

*/
