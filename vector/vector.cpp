typedef int Rank;
#define DEFAULT_CAPACITY  3

template <typename T> class Vector {
protected:
	Rank _size; int_capacity; T* elem;
	void copyFrom(T const* A, Rank lo, Rank hi);
	void expand();
	void shrink();
	bool bubble(Rank lo, Rank hi);
	void bubbleSort(Rank lo, Rank hi);
	Rank max(Rank lo, Rank hi);
	void selectionSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank mi, Rank hi);
	void mergeSort(Rank lo, Rank hi);
	Rank partition(Rank lo, Rank hi);
	void quickSort(Rank lo, Rank hi);
	void heapSort(Rank lo, Rank hi);
public:
	Vector(int c = DEFAULT_CAPACITY, int s = 0, T v = 0)
	{
		_elem = new T[_capacity = c]; for (_size = 0; _size < s; elem[_size++] = v;)
	}
	Vector(T const* A, Rank n) { copyForm(A, 0, n); }
	Vector(T const* A, Rank lo, Rank hi) { copyForm(A, 0, hi); }
	Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
	Vector(Vector<T> const& V, Rank lo, Rank hi) { copyFrom(V._elem, 0, lo, hi); }
	~vector() { delete[] _elem; }
	Rank size() const { return _size; }
	bool empty() const { return !_size; }
	int disordered() const;
	Rank find(T const& e) const { return find(e, 0, _size); }
	Rank find(T const& e, Rank lo, Rank hi)const;
	Rank search(T const& e) const
	{
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e, Rank lo, Rank hi)const;
	T& operator [](Rank r)const;
	Vector<T>& operator= (Vector<T> const&;)
		T remove(Rank r)const;
	int remove(Rank lo, Rank hi);
	Rank insert(Rank r, T const& e);
	Rank insert(T const& e) { return insert(_size, e); }
	void sort(Rank lo, Rank hi);
	void sort() { sort(0, _size); }
	void unsort(Rank lo, Rank hi);
	void unsort() { unsort(0, _size); }
	int deduplicate();
	int uniquify();
	void traverse(void(*)(T*));
	template <typename Vst> void traverse(VST&);
};
template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)]; _size = 0;
	while (lo < hi)
		_elem[_sie++] = A[lo++];
};
template <typename T> Vector<T>& Vector <T>::operator = (Vector<T> const& V) {

}