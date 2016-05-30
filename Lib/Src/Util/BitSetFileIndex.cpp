#include "pch.h"
#include <BitSet.h>
#include <CountedByteStream.h>

void BitSetIndex::save(ByteOutputStream &s) const {
  s.putByte(m_shift);                                             // save 1 byte:m_shift
  const unsigned int setCapacity = m_bitSet.getCapacity();
  s.putBytes((BYTE*)&setCapacity, sizeof(setCapacity));           // save 4 bytes:setCapacity
  m_rangeTable.save(s);                                           // save rangetable
  m_bitSet.save(s);                                               // save m_bitset
}

BitSetFileIndex::BitSetFileIndex(const String &fileName, unsigned __int64 startOffset) 
: m_startOffset(startOffset)
, m_f(fileName)
 {
  m_f.seek(startOffset);
  CountFileOffset        byteCounter;
  CountedByteInputStream in(byteCounter, m_f);

  m_shift = in.getByte();                                         // load 1 byte:m_shift
  unsigned int setCapacity;
  in.getBytesForced((BYTE*)&setCapacity, sizeof(setCapacity));    // load 4 bytes:setCapacity
  m_rangeTable.load(in);                                          // load rangeTable
  m_loadedIntervals   = new BitSet(m_rangeTable.size()+1);
  m_bitSet            = new BitSet(setCapacity);                  // allocate bitset. dont read it
  m_bitsStartOffset   = startOffset + byteCounter.getByteOffset() + sizeof(unsigned long); // bits begin after bitet.capacity
}

BitSetFileIndex::~BitSetFileIndex() {
  delete m_bitSet;
  delete m_loadedIntervals;
}

#define BYTESINATOM sizeof(m_bitSet->m_p[0])

int BitSetFileIndex::getIndex(unsigned long i) const {
  if(i >= m_bitSet->getCapacity()) {
    return -1;
  }
  const long rangeIndex = m_rangeTable.binarySearchLE(i, uintHashCmp);
  
  const unsigned int  bitInterval = (rangeIndex < 0) ? 0 : (rangeIndex+1);
  const unsigned long startBit    =  bitInterval ? m_rangeTable[rangeIndex] : 0;
  if(!m_loadedIntervals->contains(bitInterval)) {
    const unsigned long lastBit     =  (bitInterval < (unsigned int)m_rangeTable.size()) ? m_rangeTable[bitInterval] : (m_bitSet->getCapacity()-1);
    const unsigned long startAtom   = m_bitSet->getAtomIndex(startBit);
    const unsigned long atomsToRead = m_bitSet->getAtomIndex(lastBit) - startAtom + 1;
    m_f.seek(m_bitsStartOffset + startAtom * BYTESINATOM);
    m_f.getBytesForced((BYTE*)(m_bitSet->m_p + startAtom), BYTESINATOM * atomsToRead);
    m_loadedIntervals->add(bitInterval);
  }

  if(!m_bitSet->contains(i)) {
    return -1;
  }

  return (bitInterval << m_shift) + m_bitSet->getCount(startBit, i) - 1;
}