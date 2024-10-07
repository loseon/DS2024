typedef int Rank;
#define ListNodeposi(T) ListNodeposi<T>*
template <typename T> struct ListNode {
  T data; ListNodeposi(T) pred; ListNodeposi(T) succ;
  ListNode(){}
  ListNode (T e,ListNodeposi(T) p =NULL,ListNodeposi(T) s= NUll)
      :data ( e ),pred(p),succ (s){}
    ListNodeposi(T) insertAsPred (T const& e);
    ListNodeposi(T) insertAsSucc (T const& e);
};



