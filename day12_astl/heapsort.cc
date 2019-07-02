#include <fun.h>
using namespace std;

template<class Type,typename CompareType = std::less<Type> >
class HeapSort
{
private:
    CompareType Compare;
    vector<Type> data;
    void swap(Type *a, Type *b) {
        Type temp = *b;
        *b = *a;
        *a = temp;
    }
    void adjustheap(Type s, int len)
    {
        Type key = data[s];
        for (int i = 2*s+1; i < len; i =2*s+1)
        {
            if (i + 1 < len && Compare((Type)data[i],(Type)data[i + 1])<0)
                i++;
            if (Compare(key,(Type)data[i])<0)
            {
                swap(&data[s], &data[i]);
                s = i;
            }
            else
            {
                break;
            }
        }
    }
public:
    void heap()
    {
        int len = data.size();
        for (int i = len/2-1; i >= 0; i--)
        {
            adjustheap(i, len);

        }
        for (int i = 1; i < len; i++)
        {
            swap(&data[0], &data[len - i]);
            adjustheap(0, len-i);
        }

    }
    void input(Type *array,int capacity)
    {   
        for(int i = 0;i< capacity; i++)
        {
            data.push_back(array[i]);
        }

    }
    void output()
    {
        for(auto a:data)
        {
            cout<<a<<endl;
        }
    }
};
template<class T>
class compare
{
public:
    int operator()(T a,T b)
    {
        return a-b;
    }
};
int main()
{
    int a[10] = {121,2,24,33,42,58,94,2,1,31};
    HeapSort<int,compare<vector<int>::value_type> > sort;
    sort.input(a,10);
    sort.heap();
    sort.output();
    return 0;
}

