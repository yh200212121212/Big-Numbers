#include "pch.h"
#include <Random.h>

/* DEBUG_ARRAYIMPL-klasse til opt�lling af kald til ArrayImpl */
//#define DEBUG_ARRAYIMPL
#ifdef DEBUG_ARRAYIMPL

class AutoCounter {
public:
  TCHAR *m_name;
  int        m_counter;
  AutoCounter(TCHAR *name) { m_name = name; m_counter = 0; }
  ~AutoCounter() { _tprintf(_T("count(%s):%d\n"),m_name,m_counter); }
  AutoCounter &operator++(int k) { m_counter++; return *this;}
};

static AutoCounter resizeUpCounter(  _T("resize up"  ));
static AutoCounter resizeDownCounter(_T("resize down"));
static AutoCounter descounter(       _T("destructor" ));
static AutoCounter initcounter(      _T("init"       ));

#endif

static unsigned int _max(unsigned int a, unsigned int b) { 
  return a > b ? a : b;
}

DEFINECLASSNAME(ArrayImpl);

ArrayImpl::ArrayImpl(AbstractObjectManager &objectManager, unsigned long capacity) {
  init(objectManager,0,_max(1,capacity));
}

void ArrayImpl::resize() {
  setCapacity(_max(2 * m_size,m_capacity));
}

void ArrayImpl::setCapacity(unsigned long capacity) {
  if(capacity != m_capacity) {
    if((int)capacity < m_size) {
      capacity = m_size;
    }
    if(capacity == 0) {
      capacity = 1;
    }

    void **newBuffer = new void*[capacity];
    memcpy(newBuffer,m_elem,sizeof(m_elem[0])*m_size);
    delete[] m_elem;
    m_elem = newBuffer;

#ifdef DEBUG_ARRAYIMPL
    if(capacity > m_capacity)
      resizeUpCounter++;
    else
      resizeDownCounter++;
#endif

    m_capacity = capacity;
  }
}

void ArrayImpl::init(const AbstractObjectManager &objectManager, int size, unsigned long capacity) {
  m_objectManager = objectManager.clone();
  m_capacity = capacity;

  m_elem = new void*[m_capacity];

  m_updateCount = 0;

//  memset(m_elem,0,m_capacity * sizeof(m_elem[0]));
  m_size = size;
#ifdef DEBUG_ARRAYIMPL
  initcounter++;
#endif
}

ArrayImpl::~ArrayImpl() {
  clear();
  delete m_objectManager;
  delete[] m_elem;

#ifdef DEBUG_ARRAYIMPL
  descounter++;
#endif
}

AbstractCollection *ArrayImpl::clone(bool cloneData) const {
  ArrayImpl *copy = new ArrayImpl(*m_objectManager,m_capacity);
  if(cloneData) {
    copy->m_size = m_size;
    for(unsigned int i = 0; i < m_size;i++) {
      copy->m_elem[i] = m_objectManager->cloneObject(m_elem[i]);
    }
  }
  return copy;
}

void ArrayImpl::clear() {
  for(unsigned int i = 0; i < m_size; i++) {
    m_objectManager->deleteObject(m_elem[i]);
  }
//  memset(m_elem,0,m_size * sizeof(m_elem[0]));
  m_size = 0;
  setCapacity(10);
  m_updateCount++;
}

void ArrayImpl::indexError(const TCHAR *method, int index) const {
  throwMethodInvalidArgumentException(s_className, method, _T("Index %d out of range. Arraysize=%d."), index, m_size);
}

void ArrayImpl::selectError() const {
  throwMethodException(s_className, _T("select"), _T("Array is empty"));
}

void ArrayImpl::unsupportedOperationError(const TCHAR *method) const {
  throwMethodUnsupportedOperationException(s_className, method);
}

bool ArrayImpl::add(const void *e) {
  if(m_size >= m_capacity) {
    resize();
  }
  m_elem[m_size++] = m_objectManager->cloneObject(e);
  m_updateCount++;
  return true;
}

bool ArrayImpl::add(unsigned int i, const void *e, unsigned int count) {
  if(i > m_size) {
    indexError(_T("add"), i);
  }
  if(count == 0) {
    return false;
  }
  if(m_size + count > m_capacity) {
    setCapacity(2*(m_size + count) + 5);
  }
  if(i < m_size) {
    memmove(m_elem+i+count, m_elem+i, (m_size-i)*sizeof(m_elem[0]));
  }
  m_size += count;
  while(count--) {
    m_elem[i++] = m_objectManager->cloneObject(e);
  }
  m_updateCount++;
  return true;
}

void ArrayImpl::removeIndex(unsigned int i, unsigned int count) {
  if(count == 0) {
    return;
  }
  unsigned int j = i + count;
  if(j > m_size) {
    indexError(_T("removeIndex"),j);
  }
  for(unsigned int k = i; k < j; k++) {
    m_objectManager->deleteObject(m_elem[k]);
  }
  if(j < m_size) {
    memmove(m_elem+i, m_elem+j, (m_size-j)*sizeof(m_elem[0]));
//    m_elem[m_size] = NULL;
  }
  m_size -= count;
  if(m_size < m_capacity/2 && m_capacity > 10) {
    setCapacity(_max(10, m_size));
  }
  m_updateCount++;
}

bool ArrayImpl::remove(const void *e) {
  unsupportedOperationError(_T("remove"));
  return false;
}

void ArrayImpl::swap(unsigned int i1, unsigned int i2) {
  if(i1 >= m_size) {
    indexError(_T("swap"),i1);
  }
  if(i2 >= m_size) {
    indexError(_T("swap"),i2);
  }
  if(i1 != i2) {
    void *tmp  = m_elem[i1];
    m_elem[i1] = m_elem[i2];
    m_elem[i2] = tmp;
  }
}

bool ArrayImpl::contains(const void *e) const {
  unsupportedOperationError(_T("contains"));
  return false;
}

const void *ArrayImpl::select() const {
  if(m_size == 0) {
    selectError();
  }
  return m_elem[randInt() % m_size];
}

void *ArrayImpl::select() {
  if(m_size == 0) {
    selectError();
  }
  return m_elem[randInt() % m_size];
}

// -------------------------------------------------------------------------------

class ArrayIterator : public AbstractIterator {
private:
  DECLARECLASSNAME;
  ArrayImpl          &m_a;
  int                 m_next;
  int                 m_current;
  unsigned long       m_updateCount;
  void checkUpdateCount() const;
public:
  AbstractIterator *clone();
  ArrayIterator(ArrayImpl &a);
  bool hasNext() const;
  void *next();
  void remove();
};

DEFINECLASSNAME(ArrayIterator);

ArrayIterator::ArrayIterator(ArrayImpl &a) : m_a(a) {
  m_updateCount = m_a.m_updateCount;
  m_next        = 0;
  m_current     = -1;
}

AbstractIterator *ArrayIterator::clone() {
  return new ArrayIterator(*this);
}

bool ArrayIterator::hasNext() const {
  return m_next < m_a.size();
}

void *ArrayIterator::next() {
  if(m_next >= m_a.size()) {
    noNextElementError(s_className);
  }
  checkUpdateCount();
  m_current = m_next++;
  return m_a.getElement(m_current);
}

void ArrayIterator::remove() {
  if(m_current < 0) {
    noCurrentElementError(s_className);
  }
  checkUpdateCount();
  m_a.removeIndex(m_current,1);
  m_current     = -1;
  m_updateCount = m_a.m_updateCount;
}

void ArrayIterator::checkUpdateCount() const {
  if(m_updateCount != m_a.m_updateCount) {
    concurrentModificationError(s_className);
  }
}

AbstractIterator *ArrayImpl::getIterator() {
  return new ArrayIterator(*this);
}