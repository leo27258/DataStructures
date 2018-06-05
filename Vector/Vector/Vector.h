#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef int Rank;
#define DEFAULT_CAPACITY 3

template<typename T>
class Vector
{
public:
    Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
    {
        m_capacity = c;
        m_elem = new T[m_capacity];
        for (m_size = 0; m_size < s;)
        {
            m_elem[m_size++] = v;
        }
    }

    Vector(const T* A, Rank lo, Rank hi) { copyFrom(A, lo, hi); }

    Vector(const T* A, Rank n) { copyFrom(A, 0, n); }

    Vector(const Vector<T>& V, Rank lo, Rank hi) { copyFrom(V.m_elem, lo, hi); }

    Vector(const Vector<T>& V) { copyFrom(V.m_elem, 0, V.m_size); }

    ~Vector() { delete[] m_elem; }

    //只读接口
    Rank size() const { return m_size; }//规模
    bool empty() const { return !m_size; }//判断空
    int disordered() const;//判断是否已排序
    Rank find(const T& e) const { return find(e, 0, m_size); }//整体查找
    Rank find(const T& e, Rank lo, Rank hi) const;//区间查找
    Rank search(const T& e) const//有序向量整体查找
    {
        return (m_size <= 0) ? -1 : search(e, 0, m_size);
    }
    Rank search(const T& e, Rank lo, Rank hi) const;//有序向量区间查找
    //可写接口
    T& operator[](Rank r) const;//重载下标操作符
    Vector<T>& operator=(const Vector<T>& V);//重载赋值操作符
    T remove(Rank r);//删除元素
    int remove(Rank lo, Rank hi);//删除区间
    Rank insert(Rank r, const T& e);//插入元素
    Rank insert(const T& e) { return insert(m_size, e); }//插入到尾部
    void sort(Rank lo, Rank hi);//区间排序
    void sort() { sort(0, m_size); }//整天排序
    void unsort(Rank lo, Rank hi);//区间打乱
    void unsort() { unsort(0, m_size); }//整体打乱
    int deduplicate();//无序去重
    int uniquify();//有序去重

    void traverse(void(*)(T&));//遍历使用函数指针
    template<typename VST> void traverse(VST&);//遍历使用函授对象（仿函数）

protected:
    Rank m_size;//规模
    int m_capacity;//容量
    T* m_elem;//数据区

    void copyFrom(const T* A, Rank lo, Rank hi);//复制数组区间[lo,hi)
    void expand();//空间不足时扩容
    void shrink();//过小时压缩
    bool bubble(Rank lo, Rank hi);//扫描交换
    void bubbleSort(Rank lo, Rank hi);//冒泡排序
    Rank max(Rank lo, Rank hi);//选取最大元素
    void selectionSort(Rank lo, Rank hi);//选择排序
    void merge(Rank lo, Rank hi);//归并算法
    void mergeSort(Rank lo, Rank hi);//归并排序
    Rank partition(Rank lo, Rank hi);//轴点构造算法
    void quickSort(Rank lo, Rank hi);//快速排序
    void heapSort(Rank lo, Rank hi);//堆排序
};

template<typename T>
void Vector<T>::copyFrom(const T* A, Rank lo, Rank hi)
{
    m_capacity = (hi - lo) * 2;
    m_elem = new T[m_capacity];
    m_size = 0;
    while (lo < hi)
    {
        m_elem[m_size++] = A[lo++];
    }
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& V)
{
    if (m_elem)
    {
        delete m_elem;
        m_elem = nullptr;
    }

    copyFrom(V.m_elem, 0, V.size());

    return *this;
}

template<typename T>
void Vector<T>::expand()
{
    if (m_size < m_capacity)
    {
        return;
    }

    if (m_capacity < DEFAULT_CAPACITY)
    {
        m_capacity = DEFAULT_CAPACITY;
    }
    m_capacity <<= 1;//容量加倍

    T* oldElem = m_elem;
    m_elem = new T[m_capacity];

    for (int i = 0; i < m_size; ++i)
    {
        m_elem[i] = oldElem[i];
    }

    delete[] oldElem;
}

template<typename T>
void Vector<T>::shrink()
{
    if (m_capacity < DEFAULT_CAPACITY << 1)
    {
        return;
    }

    if (m_size << 2 > m_capacity)
    {
        return;
    }

    m_capacity >>= 1;

    T* oldElem = m_elem;
    m_elem = new T[m_capacity];
    for (int i = 0; i < m_size; ++i)
    {
        m_elem[i] = oldElem[i];
    }

    delete[] oldElem;
}

template<typename T>
T& Vector<T>::operator[](Rank r) const
{
    return m_elem[r];
}

template<typename T>
void Vector<T>::unsort(Rank lo, Rank hi)
{
    T* V = m_elem + lo;
    for (Rank i = hi - lo; i > 0; --i)
    {
        swap(V[i - 1], V[rand() % i]);
    }
}
template<typename T>
Rank Vector<T>::find(const T& e, Rank lo, Rank hi) const
{
    while ((lo < hi--) && m_elem[hi] != e)
    {
    }

    return hi;
}

template<typename T>
Rank Vector<T>::insert(Rank r, const T& e)
{
    expand();

    for (int i = m_size; i > r; --i)
    {
        m_elem[i] = m_elem[i - 1];
    }
    m_elem[r] = e;

    m_size++;
    
    return r;
}

template<typename T>
int Vector<T>::remove(Rank lo, Rank hi)
{
    if (lo >= hi)
    {
        return 0;
    }

    while (hi < m_size)
    {
        m_elem[lo++] = m_elem[hi++];
    }

    m_size = lo;

    return hi - lo;
}

template<typename T>
T Vector<T>::remove(Rank r)
{
    T e = m_elem[r];

    remove(r, r + 1);

    return e;
}

template<typename T>
int Vector<T>::deduplicate()
{
    int oldSize = m_size;
    Rank i = 1;
    while (i < m_size)
    {
        (find(m_elem[i], 0, i) < 0) ? i++ : remove(i);//查找相同元素并删除，没有则查其后继
    }

    return oldSize - m_size;
}

template<typename T>
void Vector<T>::traverse(void(*visit)(T&))
{
    for (int i = 0; i < m_size; ++i)
    {
        visit(m_elem[i]);
    }
}

template<typename T> template<typename VST>
void Vector<T>::traverse(VST& visit)
{
    for (int i = 0; i < m_size; ++i)
    {
        visit(m_elem[i]);
    }
}

template<typename T>
int Vector<T>::disordered() const
{
    int n = 0;
    for (int i = 1; i < m_size; ++i)
    {
        if (m_elem[i-1] > m_elem[i])
        {
            n++;
        }
    }
    return n;
}

template<typename T>
int Vector<T>::uniquify()
{
    Rank i = 0;
    Rank j = 0;

    while (++j < m_size)
    {
        if (m_elem[i] != m_elem[j])//跳过相同的元素，发现不同时将其移动至i右侧
        {
            m_elem[++i] = m_elem[j];
        }
    }
    m_size = i + 1;
    return j - i;
}

template<typename T>
Rank Vector<T>::search(const T& e, Rank lo, Rank hi) const
{
    return binSearchA(m_elem, e, lo, hi);
}

template<typename T>
static Rank binSearchA(const T* V, const T& e, Rank lo, Rank hi)
{
    while (lo < hi)
    {
        Rank min = (lo + hi) >> 1;
        if (e < V[min])
        {
            hi = min;
        }
        else if (e > V[min])
        {
            lo = min + 1;
        }
        else
            return min;
    }

    return -1;
}

template<typename T>
Rank binSearch(const T* V, const T& e, Rank lo, Rank hi)
{
    int min = (hi + lo) / 2;

    if (V[min] == e)
    {
        return min;
    }
    if (V[min] > e)
    {
        return binSearch(V, e, lo, min );
    }
    else
    {
        return binSearch(V, e, min+1 , hi);
    }

    return -1;
}
#endif