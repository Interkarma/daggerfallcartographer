/****************************************************************************\
*
* Filename:		TGuid.h
* Purpose:		Definition of TGuid class
* Version:		1.0
* Author:		Gavin Clayton
*
* Last Updated:	16/08/00
*
* Copyright 1999-2000. Gavin Clayton. All Rights Reserved.
*
\****************************************************************************/


#if !defined(AFX_TGUID_H__469D6A22_A176_11D2_B1ED_00E0290A2804__INCLUDED_)
#define AFX_TGUID_H__469D6A22_A176_11D2_B1ED_00E0290A2804__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class TGuid
{
public:		// construction / destruction
	TGuid();
	TGuid( const LPGUID lpGUID );
	TGuid( const TGuid& refGuid );
	virtual ~TGuid();

public:		// methods
	HRESULT CreateGuid();

	void GetGuid( LPGUID lpGUIDOut );

	const TGuid& operator = ( const LPGUID lpGUID );
	const TGuid& operator = ( const TGuid& refGuid );

	bool operator == ( const LPGUID lpGUID );
	bool operator == ( const TGuid& refGuid );
	bool operator != ( const LPGUID lpGUID );
	bool operator != ( const TGuid& refGuid);

	void Guid2Str( TString& strOut );
	void Guid2RegStr( TString& strOut );

protected:	// methods
	void Assign( unsigned long Data1, unsigned short Data2, unsigned short Data3, unsigned char *pData4 );
	bool Compare( unsigned long Data1, unsigned short Data2, unsigned short Data3, unsigned char *pData4 );

	void Hex2Str( unsigned char byte, TString& strOut );
	void Hex2Str( unsigned short ushort, TString& strOut );
	void Hex2Str( unsigned long ulong, TString& strOut );

public:		// data
	GUID	m_GUID;
};


#endif // !defined(AFX_TGUID_H__469D6A22_A176_11D2_B1ED_00E0290A2804__INCLUDED_)
