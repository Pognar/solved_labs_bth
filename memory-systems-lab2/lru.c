#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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

//returns the index with the smallest value
int min(int arr[], int size){

    int curr_val, index;
    curr_val = arr[0];
    index = 0;
    for (int i = 0; i < size; i++){
        if (curr_val > arr[i]){
            curr_val = arr[i];
            index = i;
        }
    }
    return index;
}

//returns the index where the value is
int find_index(int arr[], int size, int value){
    for(int i = 0; i < size; i++){
        if(value == arr[i]){
            return i;
        }
    }
    return value;
}

int p_replacement(int n_pages, int p_size, char* f_name){

    int array_size, numf_f, page_faults, index_s, index;
    array_size = n_pages;
    page_faults = 0;
    numf_f = 0;
    index_s = 0;
    index = 0;

    int page_array[n_pages];
    int page_index[n_pages];

    //Initialize both arrays
    for(int i = 0; i < array_size; i++){
        page_index[i] = -1;
        page_array[i] = -1;
    }

    FILE* m_file;
    m_file = fopen(f_name, "r");

    //loop while fscanf does not return -1
    while(fscanf(m_file, "%d", &numf_f) != -1){

        numf_f = numf_f / p_size; //Determines page number (address reference/page size)
        index_s++;
        
        //if the page number is found in the page array set that index as recently used.
        if(in_page(page_array, array_size, numf_f)){

            int temp_index = find_index(page_array, array_size, numf_f);
            page_index[temp_index] = index_s;

        }else{
            if(!are_equal(index, array_size)){

              index = min(page_index, array_size);

            }
            page_array[index] = numf_f;
            page_index[index] = index_s;
            page_faults++;
        }
    }
    fclose(m_file);
    return page_faults;
}

int main(int argc, char **argv){

    int n_pages = atoi(argv[1]); // 4
    int p_size = atoi(argv[2]); // 256
    char *f_name = argv[3];

    printf("No physical pages = %d page size = %d\n\
Reading memory trace from %s\n", n_pages, p_size, f_name);
    int p_faults = p_replacement(n_pages, p_size, f_name);
    printf("Number of page faults %d\n", p_faults);

    return 0;

}
