#if !defined(AFX_WMPSTRINGCOLLECTION_H__FFD35B8E_C69B_4928_A444_2DC61381440E__INCLUDED_)
#define AFX_WMPSTRINGCOLLECTION_H__FFD35B8E_C69B_4928_A444_2DC61381440E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CWMPStringCollection wrapper class

class CWMPStringCollection : public COleDispatchDriver
{
public:
    CWMPStringCollection() {}       // Calls COleDispatchDriver default constructor
    CWMPStringCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CWMPStringCollection(const CWMPStringCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    long GetCount();
    CString Item(long lIndex);
};

// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WMPSTRINGCOLLECTION_H__FFD35B8E_C69B_4928_A444_2DC61381440E__INCLUDED_)
