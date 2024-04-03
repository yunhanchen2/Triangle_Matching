#include <iostream>
#include <algorithm>
#include <fstream>

using namespace std;

//class queue

class ListNode{
private:
    int data;
    ListNode* next;
public:
    ListNode(int i){
        data=i;
    }
    int getdata(){
        return data;
    }
    void setnext(ListNode * p){
        next=p;
    }
    ListNode * getnext(){
        return next;
    }
};



class MyQueue{
private:
    ListNode *front;
    ListNode *rear;
    int size;
public:
    MyQueue(){
        size=0;
        front=NULL;
        rear=NULL;
    }
    bool IsEmpty();
    void Enqueue(int data);
    int Dequeue();
};

bool MyQueue::IsEmpty() {
    return(front==NULL);
}

void MyQueue::Enqueue(int data) {
    ListNode * node=new ListNode(data);
    if(IsEmpty()){
        front=node;
        rear=node;
    }else{
        rear->setnext(node);
        rear=node;
    }
    size++;
}

int MyQueue::Dequeue() {
    int ret_value=0;
    if(!IsEmpty()){
        ret_value=front->getdata();
        front=front->getnext();
        if(front==NULL){
            rear=NULL;
        }
    }
    return ret_value;
}


bool compare(const int* a, const int* b) {
    if (a[0] != b[0]) {
        return a[0] < b[0];  // 按照第一个数从小到大排列
    }
    return a[1] < b[1];  // 对于相同的第一个数，按照第二个数从小到大排列
}

struct CSRGraph {
    int *col_indices;
    int *row_offsets;
};



void removeDuplicates(int **arr, int& size) {
    int newSize = 0;
    for (int i = 0; i < size; i++) {
        if (i == 0 || !(arr[i][0] == arr[i - 1][0] && arr[i][1] == arr[i - 1][1])) {  // 如果当前元素不等于前一个元素，则将其保留
            arr[newSize][0] = arr[i][0];
            arr[newSize][1] = arr[i][1];
            newSize++;
        }
    }

    size = newSize;  // 更新数组大小
}

void get_true_index(int arr[], int n) {
    if (n == 0) {
        return;
    }

    int slow = 0; // 慢指针
    for (int fast = 1; fast < n; fast++) { // 快指针
        if (arr[fast] != arr[slow]) {
            slow++;
            arr[slow] = arr[fast]; // 将非重复元素移到慢指针位置
        }
    }
}

int main(int argc,char* argv[]) {
    if (argc > 1) {
        char *pathname = argv[1];
        std::ifstream fin;
        fin.open(pathname);
        int n, m;

        fin >> n >> m;

        n = n * 2;

        CSRGraph graph;

        graph.col_indices = new int[n];
        graph.row_offsets = new int[m + 1];

        int size = n;

        int **array = new int *[size];  // 动态分配二维数组的行
        for (int i = 0; i < size; ++i) {
            array[i] = new int[2];  // 动态分配二维数组的列
        }

        int count_the_same = 0;

        for (int i = 0; i < size; ++i) {
            int m1, n1;
            fin >> m1 >> n1;

            array[i][0] = m1;
            array[i][1] = n1;
            i++;
            array[i][0] = n1;
            array[i][1] = m1;

        }

        fin.close();

        size = size - count_the_same;
        n = n - count_the_same;

        // 按照要求对数组进行排序
        std::sort(array, array + size, compare);

        removeDuplicates(array, n);

        //find the true index
        int *true_index = new int[m];

        for (int i = 0; i < n; i++) {
            true_index[i] = array[i][0];
        }

        get_true_index(true_index, n);

        int *query_list = new int[array[n - 1][0] + 1];
        for (int i = 0; i < (array[n - 1][0] + 1); i++) {
            query_list[i] = 0;
        }
        for (int i = 1; i < m + 1; i++) {
            query_list[true_index[i - 1]] = i;
        }


        //get row_offsets
        for (int i = 0; i < m + 1; i++) {
            graph.row_offsets[i] = 0;
        }

        for (int i = 0; i < n; i++) {
            graph.row_offsets[query_list[array[i][0]] - 1]++;
        }

        int temp = 0;
        int accumulate = 0;

        for (int i = 0; i < m; i++) {
            temp = graph.row_offsets[i];
            graph.row_offsets[i] = accumulate;
            accumulate += temp;
        }
        graph.row_offsets[m] = accumulate;



        //get col_indices
        for (int i = 0; i < n; i++) {
            graph.col_indices[i] = 0;
        }

        for (int i = 0; i < n; i++) {
            graph.col_indices[i] = array[i][1];
        }


        //create queue
        MyQueue M1;
        MyQueue M21;
        MyQueue M22;
        MyQueue M31;
        MyQueue M32;
        MyQueue M33;

        //create counters
        int n1 = 0;
        int n2 = 0;
        int n3 = 0;

        //find the triangle
        for (int i = 1; i < m + 1; i++) {
            if ((graph.row_offsets[i] - graph.row_offsets[i - 1]) >= 2) {
                M1.Enqueue(true_index[i - 1]);
                n1++;
            }
        }

        for (int i = 0; i < n1; i++) {
            int temp = M1.Dequeue();
            for (int j = graph.row_offsets[query_list[temp] - 1]; j < graph.row_offsets[query_list[temp]]; j++) {
                if (((graph.row_offsets[query_list[graph.col_indices[j]]] -
                      graph.row_offsets[query_list[graph.col_indices[j]] - 1]) >= 2) && graph.col_indices[j] > temp) {
                    M21.Enqueue(temp);
                    M22.Enqueue(graph.col_indices[j]);
                    n2++;
                }
            }
        }

        for (int i = 0; i < n2; i++) {
            int temp1 = M21.Dequeue();
            int temp2 = M22.Dequeue();
            for (int j = graph.row_offsets[query_list[temp1] - 1]; j < graph.row_offsets[query_list[temp1]]; j++) {
                for (int k = graph.row_offsets[query_list[temp2] - 1]; k < graph.row_offsets[query_list[temp2]]; k++) {
                    if (graph.col_indices[j] == graph.col_indices[k]) {
                        if (((graph.row_offsets[query_list[graph.col_indices[j]]] -
                              graph.row_offsets[query_list[graph.col_indices[j]] - 1]) >= 2) &&
                            (graph.col_indices[j] > temp1) && (graph.col_indices[j] > temp2)) {
                            M31.Enqueue(temp1);
                            M32.Enqueue(temp2);
                            M33.Enqueue(graph.col_indices[j]);
                            n3++;
                        }
                    }
                }
            }
        }

        //cut duplicate:

        for (int i = 0; i < n3; i++) {
            cout << M31.Dequeue() << " " << M32.Dequeue() << " " << M33.Dequeue() << endl;
        }

        cout << "the total number of triangles are: " << n3 << endl;

        return 0;
    }
}