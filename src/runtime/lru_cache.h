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
#ifndef COREVM_LRU_CACHE_H_
#define COREVM_LRU_CACHE_H_

#include <boost/bimap.hpp>
#include <boost/bimap/list_of.hpp>
#include <boost/bimap/set_of.hpp>
#include <boost/bimap/unordered_set_of.hpp>


namespace corevm {


namespace runtime {


template <typename K, typename V, size_t N, template <typename...> class SetType>
class LruCacheCore
{
public:
  typedef K key_type;
  typedef V value_type;

private:
  typedef boost::bimaps::bimap<SetType<key_type>,
    boost::bimaps::list_of<value_type>> container_type;

public:
  bool has(const key_type& k)
  {
    return m_container.left.find(k) != m_container.left.end();
  }

  bool get(const key_type& k, value_type& res)
  {
    const typename container_type::left_iterator it = m_container.left.find(k);

    if (it != m_container.left.end())
    {
      m_container.right.relocate(m_container.right.end(),
        m_container.project_right(it));

      res = it->second;

      return true;
    }

    return false;
  }

  void insert(const key_type& k, const value_type& v)
  {
    if (m_container.size() == N)
    {
      m_container.right.erase(m_container.right.begin());
    }

    m_container.insert(typename container_type::value_type(k, v));
  }

private:
  container_type m_container;
};

template <typename K, typename V, size_t N>
struct LruCache
{
  typedef LruCacheCore<K, V, N, boost::bimaps::unordered_set_of> type;
};


} /* end namespace runtime */


} /* end namespace corevm */


#endif /* COREVM_LRU_CACHE_H_ */
