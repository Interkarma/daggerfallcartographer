/*
* TRandom.h
* Encapsulates the "Mersenne Twister" algorithm by Makoto Matsumoto and Takuji Nishimura.
* This code can be distributed under the Artistic license.
* See below for more details on obtaining the standard version.
*/

/* Copyright (C) 1997, 1999 Makoto Matsumoto and Takuji Nishimura. */
/* Any feedback is very welcome. For any question, comments,       */
/* see http://www.math.keio.ac.jp/matumoto/emt.html or email       */
/* matumoto@math.keio.ac.jp                                        */

/* REFERENCE                                                       */
/* M. Matsumoto and T. Nishimura,                                  */
/* "Mersenne Twister: A 623-Dimensionally Equidistributed Uniform  */
/* Pseudo-Random Number Generator",                                */
/* ACM Transactions on Modeling and Computer Simulation,           */
/* Vol. 8, No. 1, January 1998, pp 3--30.                          */

#if !defined(AFX_TRANDOM_H__1DE3DAB8_C9EB_4A18_8628_525617ADECBA__INCLUDED_)
#define AFX_TRANDOM_H__1DE3DAB8_C9EB_4A18_8628_525617ADECBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class TRandom  
{
public:		// construction / destruction
	TRandom();
	TRandom( unsigned long seed );
	virtual ~TRandom();

public:		// methods
	void			Seed( unsigned long seed );
	unsigned long	Rnd();

private:	// data
	int mti;
	unsigned long *mt;
};


#endif // !defined(AFX_TRANDOM_H__1DE3DAB8_C9EB_4A18_8628_525617ADECBA__INCLUDED_)
