/*******************************************************************************
The MIT License (MIT)

Copyright (c) 2016 Yanzheng Li

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*******************************************************************************/
#ifndef COREVM_LINEAR_MAP_H_
#define COREVM_LINEAR_MAP_H_

#include <vector>

#define CACHE_CONSCIOUS 1


namespace corevm {


namespace runtime {


template<typename K, typename V, typename CoreType=std::vector<std::pair<K, V>>>
class LinearMap
{
public:
  typedef K key_type;
  typedef V mapped_type;
  typedef std::pair<K, V> value_type;

private:
  typedef CoreType core_type;

  struct KeyPred
  {
    KeyPred(K key): m_value(key) {}
    bool operator()(const value_type& pair) const
    {
      return pair.first == m_value;
    }

  private:
    K m_value;
  };

public:
  typedef typename core_type::iterator iterator;
  typedef typename core_type::const_iterator const_iterator;

  iterator begin() { return m_vec.begin(); }
  iterator end() { return m_vec.end(); }
  const_iterator begin() const { return m_vec.begin(); }
  const_iterator end() const { return m_vec.end(); }
  const_iterator cbegin() const { return m_vec.cbegin(); }
  const_iterator cend() const { return m_vec.cend(); }

  size_t size() const { return m_vec.size(); }

  bool empty() const { return m_vec.empty(); }

  iterator find(K k)
  {
    return std::find_if(m_vec.begin(), m_vec.end(), KeyPred(k));
  }

  const_iterator find(K k) const
  {
    return std::find_if(m_vec.begin(), m_vec.end(), KeyPred(k));
  }

  iterator erase(iterator pos)
  {
    return m_vec.erase(pos);
  }

  void erase(K k)
  {
    m_vec.erase(std::remove_if(m_vec.begin(), m_vec.end(), KeyPred(k)));
  }

  void insert(const value_type& pair)
  {
    auto itr = find(pair.first);
    if (itr != end())
    {
      *itr = pair;
    }
    else
    {
      m_vec.push_back(pair);
    }
  }

  mapped_type& operator[](key_type k)
  {
    auto itr = find(k);

    if (itr != end())
    {
#if CACHE_CONSCIOUS
      std::swap(m_vec.front(), *itr);
      return m_vec.front().second;
#else
      return itr->second;
#endif
    }

    m_vec.push_back(std::make_pair(k, V()));

    return m_vec.back().second;
  }

private:
  core_type m_vec;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_LINEAR_MAP_H_ */
