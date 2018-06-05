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

    //ֻ���ӿ�
    Rank size() const { return m_size; }//��ģ
    bool empty() const { return !m_size; }//�жϿ�
    int disordered() const;//�ж��Ƿ�������
    Rank find(const T& e) const { return find(e, 0, m_size); }//�������
    Rank find(const T& e, Rank lo, Rank hi) const;//�������
    Rank search(const T& e) const//���������������
    {
        return (m_size <= 0) ? -1 : search(e, 0, m_size);
    }
    Rank search(const T& e, Rank lo, Rank hi) const;//���������������
    //��д�ӿ�
    T& operator[](Rank r) const;//�����±������
    Vector<T>& operator=(const Vector<T>& V);//���ظ�ֵ������
    T remove(Rank r);//ɾ��Ԫ��
    int remove(Rank lo, Rank hi);//ɾ������
    Rank insert(Rank r, const T& e);//����Ԫ��
    Rank insert(const T& e) { return insert(m_size, e); }//���뵽β��
    void sort(Rank lo, Rank hi);//��������
    void sort() { sort(0, m_size); }//��������
    void unsort(Rank lo, Rank hi);//�������
    void unsort() { unsort(0, m_size); }//�������
    int deduplicate();//����ȥ��
    int uniquify();//����ȥ��

    void traverse(void(*)(T&));//����ʹ�ú���ָ��
    template<typename VST> void traverse(VST&);//����ʹ�ú��ڶ��󣨷º�����

protected:
    Rank m_size;//��ģ
    int m_capacity;//����
    T* m_elem;//������

    void copyFrom(const T* A, Rank lo, Rank hi);//������������[lo,hi)
    void expand();//�ռ䲻��ʱ����
    void shrink();//��Сʱѹ��
    bool bubble(Rank lo, Rank hi);//ɨ�轻��
    void bubbleSort(Rank lo, Rank hi);//ð������
    Rank max(Rank lo, Rank hi);//ѡȡ���Ԫ��
    void selectionSort(Rank lo, Rank hi);//ѡ������
    void merge(Rank lo, Rank hi);//�鲢�㷨
    void mergeSort(Rank lo, Rank hi);//�鲢����
    Rank partition(Rank lo, Rank hi);//��㹹���㷨
    void quickSort(Rank lo, Rank hi);//��������
    void heapSort(Rank lo, Rank hi);//������
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
    m_capacity <<= 1;//�����ӱ�

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
        (find(m_elem[i], 0, i) < 0) ? i++ : remove(i);//������ͬԪ�ز�ɾ����û���������
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
        if (m_elem[i] != m_elem[j])//������ͬ��Ԫ�أ����ֲ�ͬʱ�����ƶ���i�Ҳ�
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