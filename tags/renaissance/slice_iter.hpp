
#ifndef __SLICE_ITER_HPP__
#define __SLICE_ITER_HPP__

#include <valarray>
using namespace std;

template<class T> class Slice_iter {
  valarray<T>* v;
  slice s;
  size_t curr;    // index of current element

  T& ref(size_t i) const { return (*v)[s.start()+i*s.stride()]; }
public:
  Slice_iter(valarray<T>* vv, slice ss) :v(vv), s(ss), curr(0) { }

  Slice_iter end() const
  {
    Slice_iter t = *this;
    t.curr = s.size();      // index of last-plus-one element
    return t;
  }

  Slice_iter& operator++() { curr++; return *this; }
  Slice_iter operator++(int) { Slice_iter t = *this; curr++; return t; }

  T& operator[](size_t i) { return ref(i); }              // C style subscript
  const T& operator[](size_t i) const { return ref(i); }              // C style subscript
  T& operator()(size_t i) { return ref(i); }              // Fortran-style subscript
  T& operator*() { return ref(curr); }                    // current element

  //removed '<>' symbol after operator symbol
  friend bool operator==(const Slice_iter& p, const Slice_iter& q);
  friend bool operator!=(const Slice_iter& p, const Slice_iter& q);
  friend bool operator< (const Slice_iter& p, const Slice_iter& q);

};

template<class T> class Cslice_iter 
{
  valarray<T>* v;
  slice s;
  size_t curr; // index of current element
  const T& ref(size_t i) const { return (*v)[s.start()+i*s.stride()]; }
public:
  Cslice_iter(valarray<T>* vv, slice ss): v(vv), s(ss), curr(0){}
  Cslice_iter end() const
  {
    Cslice_iter t = *this;
    t.curr = s.size(); // index of one plus last element
    return t;
  }

  Cslice_iter& operator++() { curr++; return *this; }
  Cslice_iter operator++(int) { Cslice_iter t = *this; curr++; return t; }
        
  const T& operator[](size_t i) const { return ref(i); }
  const T& operator()(size_t i) const { return ref(i); }
  const T& operator*() const { return ref(curr); }

  //removed '<>' symbol after operator symbol
  friend bool operator==(const Cslice_iter& p, const Cslice_iter& q);
  friend bool operator!=(const Cslice_iter& p, const Cslice_iter& q);
  friend bool operator< (const Cslice_iter& p, const Cslice_iter& q);

};

template<class T>
bool operator==(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
  return p.curr==q.curr
    && p.s.stride()==q.s.stride()
    && p.s.start()==q.s.start();
}

template<class T>
bool operator!=(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
  return !(p==q);
}

template<class T>
bool operator<(const Slice_iter<T>& p, const Slice_iter<T>& q)
{
  return p.curr<q.curr
    && p.s.stride()==q.s.stride()
    && p.s.start()==q.s.start();
}


//-------------------------------------------------------------

template<class T>
bool operator==(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
  return p.curr==q.curr
    && p.s.stride()==q.s.stride()
    && p.s.start()==q.s.start();
}

template<class T>
bool operator!=(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
  return !(p==q);
}

template<class T>
bool operator<(const Cslice_iter<T>& p, const Cslice_iter<T>& q)
{
  return p.curr<q.curr
    && p.s.stride()==q.s.stride()
    && p.s.start()==q.s.start();
}


//-------------------------------------------------------------

#endif //__SLICE_ITER_HPP__
