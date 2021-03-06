#include "pch.h"

#ifdef IS32BIT
#define ASM_OPTIMIZED
#endif

#if(SP_OPT_METHOD == SP_OPT_NONE)

#define expoBaseB2Baseb(n) (2*(n) + 1)

static int expoBaseb2BaseB(int n) {
  return (n>=0) ? (n/2):((n-1)/2);
}

// Assume *this = zero and x != zero
void BigReal::baseb(const BigReal &x) {
  for(Digit *p = x.m_first; p; p = p->next) {
    appendDigit(p->n / SQRT_BIGREALBASE);
    appendDigit(p->n % SQRT_BIGREALBASE);
  }
  m_expo     = expoBaseB2Baseb(x.m_expo);
  m_low      = 2 * x.m_low;
  m_negative = x.m_negative;
  trimZeroes();
}

// Assume *this = zero and x != zero and length > 0
void BigReal::baseb(const BigReal &x, int length) { // length in NUMBERDIGITS,
  m_expo = expoBaseB2Baseb(x.m_expo);
  int i = length;
  for(Digit *p = x.m_first; i-- && p; p = p->next) {
    appendDigit(p->n / SQRT_BIGREALBASE);
    appendDigit(p->n % SQRT_BIGREALBASE);
  }
  m_low      = m_expo + 3 - 2*(length-i);
  m_negative = x.m_negative;
  trimZeroes();
}

BigReal &BigReal::baseB(const BigReal &x) {
  if(x.isZero()) {
    return *this = x;
  }
  clearDigits();
//_tprintf(_T("converter baseb:")); x.dump(); _tprintf(_T("\n"));
  const Digit *p = x.m_first;

  unsigned long carry = 0;
  for(int i = x.m_expo; p; i--, p = p->next) {
    carry = carry * SQRT_BIGREALBASE + p->n;
    if(i % 2 == 0) {
      appendDigit(carry);
      carry = 0;
    }
  }
  if(carry) { // remember the last half Digit
    appendDigit(carry * SQRT_BIGREALBASE);
  }

  m_expo     = expoBaseb2BaseB(x.m_expo);
  m_low      = expoBaseb2BaseB(x.m_low);
  m_negative = x.m_negative;
  return *this;
//_tprintf(_T("baseB:")); dump(); _tprintf(_T("\n"));
}

#ifdef _DEBUG
BigReal &BigReal::shortProductNoZeroCheckDebug(const BigReal &x, const BigReal &y, int loopCount) { // return *this
#else
BigReal &BigReal::shortProductNoZeroCheck(     const BigReal &x, const BigReal &y, int loopCount) { // return *this
#endif
  // loopCount assumes we multiply NUMBERDIGITS. Here we multiply only SQRT_NUMBERDIGITS (half size)
  // in each iteration, so we have to do twice as many iterations

  const int xl = x.getLength();
  const int yl = y.getLength();

  DigitPool *pool = getDigitPool();
  BigReal xb(pool), yb(pool);

  if(loopCount >= xl*yl/2) {
    xb.baseb(x);
    yb.baseb(y);
  } else {
    xb.baseb(x, min(loopCount, xl));
    yb.baseb(y, min(loopCount, yl));
  }

  xb.assertIsValidBigReal();
  yb.assertIsValidBigReal();
  loopCount *= 2; // assume loopCount > 0
  loopCount--;

  BigReal total(pool);
  total.m_expo = xb.m_expo + yb.m_expo;
  total.m_low = total.m_expo + 1;

  for(Digit *xk = xb.m_first, *yk = yb.m_first;;) {

#ifndef ASM_OPTIMIZED
    unsigned __int64 tmp(0);
    for(Digit *xp = xk, *yp = yk; xp && yp; xp = xp->next, yp = yp->prev) {
//      _tprintf(_T("    multiply %2d * %2d = %d\n"),xp->n,yp->n,xp->n*yp->n);
      tmp += xp->n * yp->n;
    }
#else // ASM_OPTIMIZED
    unsigned __int64 tmp;
    __asm {
      xor         eax, eax                // eax = 0
      xor         ebx, ebx                // ebx = 0
      mov         ecx, dword ptr [xk]     // xp = xk
      mov         edx, dword ptr [yk]     // yp = yk
innerLoop:                                //
      cmp         ecx, 0                  // while(xp && yp) {
      je          exitLoop                //
      cmp         edx, 0                  //
      je          exitLoop                //
      mov         esi, dword ptr [ecx]    //   esi =  xp->n
      imul        esi, dword ptr [edx]    //   esi *= yp->n
      add         eax, esi                //   (ebx,eax) += esi
      adc         ebx, 0                  //
      mov         ecx, dword ptr [ecx+4]  //   xp = xp->next
      mov         edx, dword ptr [edx+8]  //   yp = yp->prev
      jmp         innerLoop               // }
exitLoop:                                 //
      mov         dword ptr [tmp]  , eax  // tmp = (ebx,eax)
      mov         dword ptr [tmp+4], ebx
    }
#endif // ASM_OPTIMIZED

    total.appendDigit(0);
    if(tmp) {
      total.addSubProduct(tmp);
    }
    total.m_low--;
    if(--loopCount < 0) break;
    if(yk->next) {
      yk = yk->next;
    } else if(!(xk = xk->next)) {
      break; // we're done
    }
  }
  total.trimZeroes().setSignByProductRule(x,y);
  return baseB(total).trimZeroes();
}

void BigReal::addSubProduct(unsigned __int64 n) {
  unsigned long carry = 0;
  for(Digit *tp = m_last; n || carry;) {
    if(tp) {
      carry += (unsigned long)(tp->n + n % SQRT_BIGREALBASE);
      tp->n = carry % SQRT_BIGREALBASE;
      tp = tp->prev;
    } else {
      carry += (unsigned long)n % SQRT_BIGREALBASE;
      insertDigit(carry);
      m_expo++;
    }
    carry /= SQRT_BIGREALBASE;
    n     /= SQRT_BIGREALBASE;
  }
// dont call trimZeroes() !!;
}

#define SPLIT_LENGTH 100

int BigReal::s_splitLength = SPLIT_LENGTH; // Value found by experiments with measureSplitFactor in testnumber.cpp

#endif // SP_OPT_METHOD == SP_OPT_NONE
