#include "stdafx.h"
#include "CppUnitTest.h"
#include <Random.h>
#include <BitSet.h>
#include <ByteFile.h>
#include <TimeMeasure.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TestBitSet {

#include <UnitTestTraits.h>

  BitSet &genRandomSet(BitSet &dst, size_t capacity, intptr_t size) {
    dst.clear();
    dst.setCapacity(capacity);
    if (size == -1) {
      for (size_t i = capacity / 2; i--;) {
        dst.add(randSizet(capacity));
      }
    } else if ((size_t)size >= capacity) {
      dst.invert();
    } else if ((size_t)size > capacity / 2) {
      dst.invert();
      for (size_t t = capacity - size; t;) {
        const size_t e = randSizet(capacity);
        if (dst.contains(e)) {
          dst.remove(e);
          t--;
        }
      }
    } else {
      for (size_t t = size; t;) {
        const size_t e = randSizet(capacity);
        if (!dst.contains(e)) {
          dst.add(e);
          t--;
        }
      }
    }
    return dst;
  }

  BitSet genRandomSet(size_t capacity, intptr_t size = -1) {
    BitSet result(10);
    return genRandomSet(result, capacity, size);
  }

  MatrixIndex getRandomIndex(const MatrixDimension &dim) {
    return MatrixIndex(randSizet(dim.rowCount), randSizet(dim.columnCount));
  }

  BitMatrix &genRandomMatrix(BitMatrix &dst, const MatrixDimension &dim, intptr_t size) {
    dst.clear();
    dst.setDimension(dim);
    const size_t capacity = dim.getElementCount();
    if (size == -1) {
      for (size_t i = capacity / 2; i--;) {
        dst.set(randSizet(dim.rowCount), randSizet(dim.columnCount),true);
      }
    } else if ((size_t)size >= capacity) {
      dst.invert();
    } else if ((size_t)size > capacity / 2) {
      dst.invert();
      for (size_t t = capacity - size; t;) {
        const MatrixIndex i = getRandomIndex(dim);
        if (dst.get(i)) {
          dst.set(i, false);
          t--;
        }
      }
    } else {
      for (size_t t = size; t;) {
        const MatrixIndex i = getRandomIndex(dim);
        if (!dst.get(i)) {
          dst.set(i, true);
          t--;
        }
      }
    }
    return dst;
  }

  BitMatrix genRandomMatrix(const MatrixDimension &dim, intptr_t size = -1) {
    BitMatrix result(dim);
    return genRandomMatrix(result, dim, size);
  }

  class BitSetIteratorTimeTester : public MeasurableFunction {
  private:
    BitSet m_a;
  public:
    BitSetIteratorTimeTester();
    void f() {
      for (Iterator<size_t> it = m_a.getIterator(); it.hasNext(); it.next());
    }
  };

  BitSetIteratorTimeTester::BitSetIteratorTimeTester() : m_a(2000) {
    m_a = genRandomSet(10000, 5000);
  }

#ifdef __MEASURETIMES__
  class BitSetSizeTimeTester : public MeasurableFunction {
  private:
    const BitSet &m_a;
    size_t        m_size;
  public:
    BitSetSizeTimeTester(const BitSet &a) : m_a(a) {
    }
    ~BitSetSizeTimeTester() {
      OUTPUT(_T("Bitset.size():%zd"), m_size);
    }
    void f() {
      m_size = m_a.size();
    }
  };

  class BitSetOldSizeTimeTester : public MeasurableFunction {
  private:
    const BitSet &m_a;
    size_t        m_size;
  public:
    BitSetOldSizeTimeTester(const BitSet &a) : m_a(a) {}
    ~BitSetOldSizeTimeTester() {
      OUTPUT(_T("Bitset.oldSize():%zd"), m_size);
    }
    void f() {
      m_size = m_a.oldSize();
    }
  };

  class BitSetGetIndexTimeTester : public MeasurableFunction {
  private:
    const BitSet     &m_a;
    CompactSizetArray m_values;
    size_t m_result[100];
  public:
    BitSetGetIndexTimeTester(const BitSet &a) : m_a(a) {
      for (Iterator<size_t> it = ((BitSet&)m_a).getReverseIterator(); it.hasNext();) {
         m_values.add(it.next());
         if(m_values.size() >= ARRAYSIZE(m_result)) break;
      }
    }
    void f() {
      for(size_t i = 0; i < ARRAYSIZE(m_result); i++) {
        m_result[i] = m_a.getIndex(m_values[i]);
      }
    }
    const size_t *getResult() const {
      return m_result;
    }
  };

  class BitSetOldGetIndexTimeTester : public MeasurableFunction {
  private:
    const BitSet     &m_a;
    CompactSizetArray m_values;
    size_t m_result[100];
  public:
    BitSetOldGetIndexTimeTester(const BitSet &a) : m_a(a) {
      for (Iterator<size_t> it = ((BitSet&)m_a).getReverseIterator(); it.hasNext();) {
         m_values.add(it.next());
         if(m_values.size() >= ARRAYSIZE(m_result)) break;
      }
    }
    void f() {
      for(size_t i = 0; i < ARRAYSIZE(m_result); i++) {
        m_result[i] = m_a.oldGetIndex(m_values[i]);
      }
    }
    const size_t *getResult() const {
      return m_result;
    }
  };
#endif

	TEST_CLASS(TestBitSet) {
    public:

    TEST_METHOD(BitSetTestPrimitiveOperations) {
      BitSet a(33);
      verify(a.size() == 0);
      verify(a.isEmpty());
      a.invert();

      verify(!a.isEmpty());
      verify(a.size() == 33);
      a.remove(31);
      verify(a.size() == 32);
      a.add(31);
      verify(a.size() == 33);
      a.remove(10, 15);
      verify(a.size() == 27);
      a.remove(28, 32);
      verify(a.size() == 22);
      a.add(28, 31);
      verify(a.size() == 26);
      a.add(10, 32);
      verify(a.size() == 33);
      BitSet c(a);
      verify(c == a);
      BitSet d(1);
      d = a;
      verify(d == a);
      BitSet b(10);
      b.add(1, 5);
      a -= b;
      verify(a.size() == 28);
      a.remove(0);
      b.invert();
      a ^= b;
      verify(a.size() == 24);
      a ^= b;
      b += a;
      verify(b.size() == 28);
      a.clear();
      a.add(1);
      a.add(10);
      a.add(31);
      a.add(32);

      BitSet e(200);
      e.invert();
      e |= b;
      e *= a;
      verify(e.size() == 4);
      Iterator<size_t> it = a.getIterator();
      verify(it.next() == 1);
      verify(it.next() == 10);
      verify(it.next() == 31);
      verify(it.next() == 32);
      verify(!it.hasNext());
      verify(a.contains(1));
      verify(!a.contains(2));
      verify(a.contains(31));
      verify(a.contains(32));
      verify(!a.contains(33));

      verify(a.toString() == _T("(1,10,31,32)"));
    }

    TEST_METHOD(BitSetTestIterator)
    {
#define ANTALITERATIONER 20
      int s;
      try {
        for(size_t cap = 20; cap < 80; cap++) {
          for (s = 0; s < ANTALITERATIONER; s++) {
            BitSet a(genRandomSet(cap));
            BitSet copy(a);
            copy.clear();
            unsigned int count = 0;
            for (Iterator<size_t> it = a.getIterator(); it.hasNext();) {
              if (!copy.isEmpty()) {
                verify(copy.contains(copy.select()));
              } else {
                try {
                  size_t x = copy.select();
                  Assert::Fail(_T("BitSet.select should throw exception when called on empty set"));
                } catch (Exception e) {
                  //ok
                }
              }
              copy.add(it.next());
              count++;
            }
            verify(count == copy.size());
            verify(copy == a);
            copy.clear();
            count = 0;
            for (Iterator<size_t> it = a.getReverseIterator(); it.hasNext();) {
              copy.add(it.next());
              count++;
            }
            verify(count == copy.size());
            verify(copy == a);
            for (size_t start = 0; start < cap+2; start++) {
              for (size_t end = 0; end < cap; end++) {
                BitSet am(a), tmp(a.getCapacity());
                am.remove(start, end);
                for (Iterator<size_t> it = a.getIterator(start, end); it.hasNext();) {
                  tmp.add(it.next());
                }
                verify(a - am == tmp);

                tmp.clear();
                for (Iterator<size_t> it = a.getReverseIterator(end, start); it.hasNext();) {
                  tmp.add(it.next());
                }
                verify(a - am == tmp);
              }
            }
          }
        }
      } catch (Exception e) {
        Assert::Fail(format(_T("Exception:%s. s=%d"), e.what(), s).cstr());
      }
    }

    TEST_METHOD(BitSetMeasureIteratorTime) {
      randomize();
      BitSetIteratorTimeTester bstt;

      double msec = measureTime(bstt);

      OUTPUT(_T("BitSetIterator time:%.3le msec"), msec * 1000);
    }
#ifdef __NEVER__
    TEST_METHOD(BitSetTestSize) {
      for (int test = 0; test < 40; test++) {
        const BitSet s(genRandomSet(test + 300));
        const size_t oldSize = s.oldSize();
        const size_t size    = s.size();
        verify(oldSize == size);
      }
    }
#endif

#ifdef __MEASURETIMES__
    TEST_METHOD(BitSetMeasureSize) {
      randomize();
      for(int i = 0; i < 10; i++) {
        const size_t capacity = 10000000*(i+1);
        const BitSet m_a = genRandomSet(capacity, 3*capacity/4);
        BitSetSizeTimeTester bstt1(m_a);
        double msec = measureTime(bstt1);
        OUTPUT(_T("BitSetSize time(cap=%zd):%.3le msec"), capacity, msec * 1000);
        BitSetOldSizeTimeTester bstt2(m_a);
        msec = measureTime(bstt2);
        OUTPUT(_T("BitSet.oldSize time(cap=%zd):%.3le msec"), capacity, msec * 1000);
      }
    }
    TEST_METHOD(BitSetMeasureGetIndex) {
      randomize();
      for(int i = 0; i < 10; i++) {
        const size_t capacity = 10000000*(i+1);
        const BitSet m_a = genRandomSet(capacity, 3*capacity/4);
        BitSetGetIndexTimeTester bstt1(m_a);
        double msec = measureTime(bstt1);
        OUTPUT(_T("BitSet.getIndex() time(cap=%zd %.3le msec"), capacity, msec * 1000);
        BitSetOldGetIndexTimeTester bstt2(m_a);
        msec = measureTime(bstt2);
        OUTPUT(_T("BitSet.oldGetIndex() time(cap=%zd %.3le msec"), capacity, msec * 1000);
      }
    }
#endif
    static intptr_t getIntIndex(const BitSet &s, size_t e) {
      if (!s.contains(e)) {
        return -1;
      }
      size_t count = 0;
      for (Iterator<size_t> it = ((BitSet&)s).getIterator(); it.hasNext(); count++) {
        if (it.next() == e) {
          return count;
        }
      }
      return -1;
    }

    TEST_METHOD(BitSetGetIndex) {
      for (int i = 0; i < 40; i++) {
        BitSet s(genRandomSet(200));
        //    OUTPUT(_T("set:%s"), s.toString().cstr());
        for (Iterator<size_t> it = s.getIterator(); it.hasNext();) {
          const size_t e = it.next();
          const intptr_t index1 = getIntIndex(s, e);
          const intptr_t index2 = s.getIndex(e);
//            OUTPUT(_T("e:%3d. index1:%3d, index2:%3d"), e, index1, index2);
          verify(index1 == index2);
        }
      }
    }

    static size_t getCount1(const BitSet &s, size_t from, size_t to) {
      size_t count = 0;
      for (size_t i = from; i <= to; i++) {
        if (s.contains(i)) {
          count++;
        }
      }
      return count;
    }

    static size_t getCount2(const BitSet &s, size_t from, size_t to) {
      size_t count = 0;
      for(Iterator<size_t> it = ((BitSet&)s).getIterator(from, to); it.hasNext(); it.next()) {
        count++;
      }
      return count;
    }

    TEST_METHOD(BitSetGetCount) {
      for (int test = 0; test < 30; test++) {
        const BitSet s(genRandomSet(200));
        //    OUTPUT(_T("set:%s"), s.toString().cstr());
        for (size_t from = 0; from < 300; from++) {
          for (size_t to = from; to < 300; to++) {
            const size_t expected1 = getCount1(s, from, to);
            const size_t expected2 = getCount2(s, from, to);
            const size_t count     = s.getCount(from, to);
            /*
            if(count != expected) {
              OUTPUT(_T("(from,to):(%s,%s). expected:%s, got:%s")
                    , format1000(from).cstr()
                    , format1000(to).cstr()
                    , format1000(expected).cstr()
                    , format1000(count).cstr());
              continue;
            }
            */
            verify(count == expected1);
            verify(count == expected2);
          }
        }
      }
    }

    TEST_METHOD(TestFileBitSet) {
      const size_t capacity = 20000;
      const BitSet s(genRandomSet(capacity));
      const TCHAR *fileName = _T("c:\\temp\\testBitSet\\fileBitSet.dat");
      s.save(ByteOutputFile(fileName));
      FileBitSet fs(fileName, 0);
      for (size_t i = 0; i < capacity; i++) {
        verify(s.contains(i) == fs.contains(i));
      }
    }

    void testAllBitSetIndices(size_t capacity) {
      BitSet s(10);
      genRandomSet(s, capacity, capacity/3);

      double startTime = getThreadTime();

      BitSetIndex bi(s);
      double usedTime = (getThreadTime() - startTime) / 1000000;

      OUTPUT(_T("Capacity:%s size:%11s creation-time:%7.3lf"), format1000(capacity).cstr(), format1000(s.size()).cstr(), usedTime);

      startTime = getThreadTime();
      intptr_t expectedIndex = 0;
      for (Iterator<size_t> it = s.getIterator(); it.hasNext(); expectedIndex++) {
        const size_t e     = it.next();
        intptr_t     index = bi.getIndex(e);
//        if (expectedIndex % 50000 == 0) {
//          OUTPUT(_T("e:%s. index %s\r"), format1000(e).cstr(), format1000(index).cstr());
//        }
        verify(index == expectedIndex);
/*
        if (index != expectedIndex) {
          OUTPUT(_T("e:%s. Expected:%s, got %s")
                ,format1000(e).cstr()
                ,format1000(expectedIndex).cstr()
                ,format1000(index).cstr());
          pause();
          intptr_t index = bi.getIndex(e);
        }
*/
      }
      usedTime = (getThreadTime() - startTime) / 1000000;
      OUTPUT(_T("Iteration-time:%7.3lf"), usedTime);
    }

    TEST_METHOD(TestBitSetIndex) {
      randomize();
      testAllBitSetIndices(6600000);
    }

    void testAllFileBitSetIndices(size_t capacity) {
      BitSet s(10);
      genRandomSet(s, capacity, capacity / 3);

      double startTime = getThreadTime();
      const TCHAR *fileName = _T("c:\\temp\\testBitSet\\fileBitSetIndex.dat");

      BitSetIndex bi(s);
      bi.save(ByteOutputFile(fileName));

      FileBitSetIndex loaded(fileName, 0);

      double usedTime = (getThreadTime() - startTime) / 1000000;

      OUTPUT(_T("Capacity:%s size:%11s creation-time:%7.3lf"), format1000(capacity).cstr(), format1000(s.size()).cstr(), usedTime);

      startTime = getThreadTime();
      intptr_t expectedIndex = 0;
      for (Iterator<size_t> it = s.getIterator(); it.hasNext(); expectedIndex++) {
        const size_t e = it.next();
        //    if(expectedIndex % 1000 != 0) {
        //      continue;
        //    }
        intptr_t index = loaded.getIndex(e);
//        if (expectedIndex % 50000 == 0) {
//          OUTPUT(_T("e:%s. index %s\r"), format1000(e).cstr(), format1000(index).cstr());
//        }
        verify(index == expectedIndex);
/*
        if(index != expectedIndex) {
          OUTPUT(_T("e:%s. Expected:%s, got %s")
                , format1000(e).cstr()
                , format1000(expectedIndex).cstr()
                , format1000(index).cstr());
          intptr_t index = loaded.getIndex(e);
        }
*/
      }
      usedTime = (getThreadTime() - startTime) / 1000000;
      OUTPUT(_T("Iteration-time:%7.3lf"), usedTime);
    }

    TEST_METHOD(FileBitSetIndexTest) {
      /*
      {
      for(double capacity = 10; capacity < 650000000; capacity *= 1.4) {
      BitSet s(10);
      genRandomSet(s, (int)capacity, -1);
      BitSetIndex bi(s);
      }
      return;
      }
      */
      /*
      for(;;) {
      const unsigned int capacity = inputInt(_T("Enter capacity:"));
      testAllBitSetIndices(capacity);
      }
      */
      randomize();
      testAllFileBitSetIndices(6600000);
    }

    TEST_METHOD(BitSetIndexTimes) {
      const size_t capacity = 3000000;
      randomize();
      BitSet s(10);
      genRandomSet(s, capacity, capacity / 2);
      BitSetIndex bi(s);
      OUTPUT(_T("  Testing time for BitSetIndex.getIndex()"));
      double startTime = getThreadTime();
      for (size_t e = 0; e < capacity;) {
        bi.getIndex(e++);
//        if ((e & 0x3ffff) == 0) {
//          OUTPUT(_T("%.2lf%% Time/call:%.4lf msec"), 100.0*e / capacity, (getThreadTime() - startTime) / e);
//        }
      }
      double biTime = getThreadTime() - startTime;
      OUTPUT(_T("BitSetIndexTime:%.3lf sec"), biTime / 1000000);

      OUTPUT(_T("  Testing time for BitSet.getIndex()"));
      startTime = getThreadTime();
      for (size_t e = 0; e < capacity; e++) {
        const intptr_t index = s.getIndex(e);
      }
      double bitSetTime = getThreadTime() - startTime;
      OUTPUT(_T("BitSetTime     :%.3lf sec"), bitSetTime / 1000000);
    }

    TEST_METHOD(BitSetMatrix) {
      MatrixDimension dim;
      for (dim.rowCount = 1; dim.rowCount < 35; dim.rowCount++) {
        for (dim.columnCount = 1; dim.columnCount < 35; dim.columnCount++) {
          const BitMatrix m = genRandomMatrix(dim, -1);

          verify(m.getDimension() == dim);

          BitMatrix copy(m);
          verify(copy == m);

          copy.clear();
          verify(copy.isEmpty());

          copy = m;
          verify(copy == m);

          MatrixDimension dim1(dim.rowCount+1, dim.columnCount+1);
          copy.setDimension(dim1);
          verify(copy.getDimension() == dim1);
          verify(copy.size() == m.size());
          for (Iterator<MatrixIndex> it = ((BitMatrix&)m).getIterator(); it.hasNext();) {
            const MatrixIndex i = it.next();
            verify(copy.get(i));
          }

          size_t totalCount = 0;
          for(size_t r = 0; r < copy.getRowCount(); r++) {
            BitSet rb = copy.getRow(r);
            int rowCount = 0;
            for(Iterator<MatrixIndex> it = copy.getRowIterator(r); it.hasNext();) {
              const MatrixIndex mi = it.next();
              rowCount++;
              verify(copy.get(mi));
              verify(rb.contains(mi.c));
            }
            verify(rowCount == rb.size());
            totalCount += rowCount;
          }
          verify(totalCount == copy.size());

          totalCount = 0;
          for(size_t c = 0; c < copy.getColumnCount(); c++) {
            BitSet cb = copy.getColumn(c);
            int colCount = 0;
            for(Iterator<MatrixIndex> it = copy.getColumnIterator(c); it.hasNext();) {
              const MatrixIndex mi = it.next();
              colCount++;
              verify(copy.get(mi));
              verify(cb.contains(mi.r));
            }
            verify(colCount == cb.size());
            totalCount += colCount;
          }
          verify(totalCount == copy.size());
        }
      }
    }
  };
}
