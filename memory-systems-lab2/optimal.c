#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define     MEM_TRACES      100000

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


int opt_predict(int mem_refs[], int page_array[], int page_n, int index) {
    int result, label, ref_i;
    label = index;

    for (int page_i = 0; page_i < page_n; page_i++) {
        for (ref_i = index; ref_i < MEM_TRACES; ref_i++) {
            if (are_equal(page_array[page_i], mem_refs[ref_i])) {
                if (label < ref_i) {
                    result = page_i;
                    label = ref_i;
                }
                break;
            }
        }
        if (are_equal(ref_i, MEM_TRACES)) { return page_i; }
    }
    return result;
}

int p_replacement(int n_pages, int p_size, char* f_name){

    int array_size, numf_f, page_faults, temp_index, index;
    array_size = n_pages;
    page_faults = 0;
    numf_f = 0;
    temp_index = 0;
    index = 0;

    int page_array[n_pages];
    int mem_refs[MEM_TRACES];

    //Initialize both arrays
    for(int i = 0; i < array_size; i++){
        page_array[i] = -1;
    }

    for(int i = 0; i < MEM_TRACES; i++){
        mem_refs[i] = -1;
    }

    FILE* m_file;
    m_file = fopen(f_name, "r");

    //loop while fscanf does not return -1
    while(fscanf(m_file, "%d", &numf_f) != -1){

        numf_f = numf_f / p_size; //Determines page number (address reference/page size)
        mem_refs[temp_index] = numf_f;
        temp_index++;
    }

    for(int i = 0; i < MEM_TRACES; i++){

        if(!in_page(page_array, array_size, mem_refs[i])){

            if(are_equal(index, array_size)){index++;}

            else{
                index = opt_predict(mem_refs, page_array, array_size, i);
            }
            page_array[index] = mem_refs[i];
            page_faults++;

        }
    }

    fclose(m_file);
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
