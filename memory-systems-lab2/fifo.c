#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define     START       0;

//returns true if value exists in array
bool in_page(int arr[], int size, int value){
    for(int i = 0; i < size; i++){
        if(value == arr[i]){
            return true;
        }
    }
    return false;
}

//returns true if i and k are equal
bool are_equal(int i, int k){
    if(i == k){
        return true;
    }
    return false;
}

int p_replacement(int n_pages, int p_size, char *f_name){
    int page_array[n_pages];
    int page_faults, array_size, numf_f, index, mem_read;
    numf_f = 0;
    index = 0;
    page_faults = 0;
    mem_read = 0;
    array_size = sizeof(page_array) / sizeof(page_array[0]);

    
    for(int i = 0; i < array_size; i++){
        page_array[i] = -1;
    }
    
    FILE* m_file;
    m_file = fopen(f_name, "r");

    //loop while fscanf does not return -1
    while(fscanf(m_file, "%d", &numf_f) != -1){

        //divide the current memory reference by its page size to get page number.
        numf_f = numf_f / p_size;
        //reset index if it has reached the end of the array
        if(are_equal(index, array_size)){
            index = START;
        }
        //if the current value is not in the array, increment pagefaults and the index
        if(!in_page(page_array, array_size, numf_f)){
            page_array[index] = numf_f;
            page_faults++;
            index++;
        }
        mem_read++;
    }
    fclose(m_file);
    printf("Memory traces read %d\n", mem_read);
    return page_faults;
}

int main(int argc, char **argv){

    int n_pages = atoi(argv[1]);
    int p_size = atoi(argv[2]);
    char *f_name = argv[3];
    printf("No physical pages = %d page size = %d\n\
Reading memory trace from %s\n", n_pages, p_size, f_name);
    int p_faults = p_replacement(n_pages, p_size, f_name);
    printf("Number of page faults %d\n", p_faults);

    return 0;

}