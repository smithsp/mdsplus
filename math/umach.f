!	Ken Klare, LANL CTR-7	(c)1990
!***********************************************************************
!***************** Machine-Dependent Constants *************************
!-----------------------------------------------------------------------
	Integer Function IMACH(i)
	Integer	i	!input, item number
!+machine-dependent integer parameters
	Integer	mach(12)
	Data mach /
     1 32,		!bits per integer storage unit
     2 4,		!characters per integer
     3 2,		!integer base
     4 31,		!integer digits
     5 '7fffffff'x,	!largest integer
     6 2,		!single base
     7 24,		!single digits
     8 -127,		!smallest single exp
     9 127,		!largest single exp
     1 56,		!double digits
     1 -127,		!smallest double exp
     2 127/		!largest double exp
	If (i.gt.0.and.i.le.12) Then
		IMACH = mach(i)
	Else
		IMACH = 0
	Endif
	End
!-----------------------------------------------------------------------
	Real Function AMACH(i)
	Integer i	!input, item number
!+machine-dependent single parameters
	Real	xmach(8)
	Integer	mach(8)
	Equivalence (xmach,mach)
	Data mach/
     1 '00000080'x,	!smallest number
     2 'fffe7fff'x,	!largest number
     3 '00003480'x,	!base**(-number_of_digits)
     4 '00003500'x,	!base**(1-number_of_digits)
     5 0,		!log10(base)
     6 'ffff7fff'x,	!not-a-number
     7 'fffe7fff'x,	!positive infinity
     8 'fffeffff'x/	!negative infinity
	If (i.gt.0.and.i.le.8) Then
		If (i.eq.5.and.mach(5).eq.0) xmach(5) = alog10(2.0e0)
		AMACH = xmach(i)
	Else
		AMACH = 0
	Endif
	End
!-----------------------------------------------------------------------
	Real*8 Function DMACH(i)
	Integer i	!input, item number
!+machine-dependent double parameters
	Real*8	xmach(8)
	Integer	mach(2,8)
	Equivalence (xmach,mach)
	Data mach/
     1 '00000080'x,0,		!smallest number
     2 'ffff7fff'x,'fffeffff'x,	!largest number
     3 '00002480'x,0,		!base**(-number_of_digits)
     4 '00002500'x,0,		!base**(1-number_of_digits)
     5 0,0,				!log10(base)
     6 'ffff7fff'x,'ffffffff'x,	!not-a-number or bigger than infinity
     7 'ffff7fff'x,'fffeffff'x,	!positive infinity
     8 'ffffffff'x,'fffeffff'x/	!negative infinity
	If (i.gt.0.and.i.le.8) Then
		If (i.eq.5.and.mach(1,5).eq.0) xmach(5) = dlog10(2.0d0)
		DMACH = xmach(i)
	Else
		DMACH = 0
	Endif
	End
!-----------------------------------------------------------------------
	Subroutine UMACH(n,nunit)
	Integer	n	!1=input 2=output +=get -=set
	Integer	nunit	!the IO number
!+machine-dependent IO parameters
	Integer in,out
	Data	in,out/-4,-1/
	If (n.eq.1) Then
		nunit = in
	Elseif (n.eq.2) Then
		nunit = out
	Elseif (n.eq.-1) Then
		in = nunit
	Elseif (n.eq.-2) Then
		out = nunit
	Endif
	End
!-----------------------------------------------------------------------
	Subroutine ERSET(iersvr,ipack,isact)
	Integer	iersvr,ipack,isact
!+Dummy error set routine
	End
