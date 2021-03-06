/*
; NAME:		DYCOM_RGA_M
; PURPOSE:	Returns the pressure of a given mass where 0 < mass <= 60
; CATEGORY:	DYCOM_RGA
; CALLING SEQUENCE:	_p = DYCOM_RGA_M(_mass, OPTIONAL _pressure_node)
;	
*/
FUN	PUBLIC	DYCOM_RGA_M(_mass, OPTIONAL _pressure_node) {
	if (present(_pressure_node)) {
	        return(MAKE_SIGNAL(DATA(DYCOM_RGA_P(_pressure_node)[*,_mass]),*,DYCOM_RGA_TIME(_pressure_node),_mass));
	}
	else {
	        return(MAKE_SIGNAL(DATA(DYCOM_RGA_P()[*,_mass]),*,DYCOM_RGA_TIME(),_mass));
	}
}
               