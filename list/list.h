#include "listNode.h"
template<typename T> class List {
    private:
       int _size; ListNodeposi(T) header; ListNodeposi(T) trailer;
protected:
    void init();
    int clear();
    void copyNodes ( ListNodeposi(T), int );
    void merge (ListNodeposi(T)&,int,List<T>&,ListNodePosi(T),int);
    void mergeSort(ListNodePosi(T)&, int);
    void selectionSort (ListNodeposi(T), int );
    void insertionSort (ListNodeposi(T), int );
public:
    List(){init()}
    List( List<T> const& L);
    List( List<T> const& L,Rank r, int n);
    List( ListNodeposi(T) p,int n);
    ~List();
    Rank size() const { return _szie;}
}