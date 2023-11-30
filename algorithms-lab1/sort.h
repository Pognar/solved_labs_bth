#ifndef SORT_H
#define SORT_H

template<typename T> void buildmaxheap(T arr[], int n, int i){

    int root = i;
    int leftchild = 2 * i + 1;
    int rightchild = 2 * i + 2;

    if(leftchild < n && arr[leftchild] > arr[root]){
        root = leftchild;
    }
    if(rightchild < n && arr[rightchild] > arr[root]){
        root = rightchild;
    }
    if(root != i){
        int holder = 0;
        holder = arr[i];
        arr[i] = arr[root];
        arr[root] = holder;
        buildmaxheap(arr, n, root);
    }

}
template<typename T> int partition(T arr[], int ln, int n){
    int pivot = arr[n];
    int i = (ln - 1);
    int holder = 0;
 
    for (int j = ln; j <= n - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            holder = 0;
            holder = arr[i];
            arr[i] = arr[j];
            arr[j] = holder;
        }
    }
    holder = 0;
    holder = arr[i+1];
    arr[i+1] = arr[n];
    arr[n] = holder;
    holder = i + 1;
    return holder;
}
template<typename T> void q_sort(T arr, int i, int n){

    if(i < n){
        int q = partition(arr, i, n);
        q_sort(arr, i, q - 1);
        q_sort(arr, q + 1, n);
    }

}

template <typename T>void quicksort(T arr[], int n){
    //quicksort
    q_sort(arr, 0, n - 1);

}
template <typename T>void heapsort(T arr[], int n){
    //heapsort
    for(int i = n / 2 - 1; i >= 0; i--){
        buildmaxheap(arr, n, i);
    }
    for(int i = n - 1; i > 0; i--){
        int holder = 0;
        holder = arr[i];
        arr[i] = arr[0];
        arr[0] = holder;
        buildmaxheap(arr, i, 0);
    }
}
template <typename T>void insertionsort(T arr[], int n){
    //insertionsort
    int i;
    int j;
    int key;
    for (j = 1; j < n; j++)
    {
        key = arr[j];
        i = j - 1;
        while (i >= 0 && arr[i] > key)
        {
            arr[i + 1] = arr[i];
            i = i - 1;
        }
        arr[i + 1] = key;
    }
}

#endif //SORT_H
//
