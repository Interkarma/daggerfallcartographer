/****************************************************************************\
*
* Filename:		TException.h
* Purpose:		Definition of TException class
* Version:		1.0
* Author:		Gavin Clayton
*
* Last Updated:	30/07/2000
*
* Copyright 2000. Gavin Clayton. All Rights Reserved.
*
\****************************************************************************/


#if !defined(AFX_TEXCEPTION_H__4D5D6EFC_00BB_4403_B63E_3F22EED777EB__INCLUDED_)
#define AFX_TEXCEPTION_H__4D5D6EFC_00BB_4403_B63E_3F22EED777EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class TException  
{
public:		// construction / destruction
	TException();
	TException( const TException& exceptionSrc );
	TException( unsigned long dwErrorID, const char* pszErrorString );
	virtual ~TException();

public:		// functions
	bool ConstructException( unsigned long dwErrorID, const char* pszErrorString );
	virtual void Delete();

public:		// data
	unsigned long	m_dwErrorID;
	char*	m_pszErrorString;
};


#endif // !defined(AFX_TEXCEPTION_H__4D5D6EFC_00BB_4403_B63E_3F22EED777EB__INCLUDED_)
