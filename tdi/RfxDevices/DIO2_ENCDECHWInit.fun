public fun DIO2_ENCDECHWInit(in _nid, in _board_id, in _ext_clock, in _rec_event, in _synch_event)
{

	private _DIO2_CLOCK_SOURCE_INTERNAL	=	0x0;
	private _DIO2_CLOCK_SOURCE_TIMING_HIGHWAY =	0x3;
	private _DIO2_CLOCK_SOURCE_RISING_EDGE	=	0x0;
	private _DIO2_ER_START_EVENT_INT_DISABLE	= 0x0;
	private _DIO2_TC_NO_TRIGGER		=			0x00;
	private _DIO2_TC_IO_TRIGGER_RISING	=		0x01;
	private _DIO2_TC_IO_TRIGGER_FALLING	=		0x02;
	private _DIO2_TC_TIMING_EVENT		=		0x03;
	private _DIO2_TC_GATE_DISABLED	=			0x00;
	private _DIO2_TC_INTERRUPT_DISABLE	=		0x00;
	private _DIO2_TC_SOURCE_FRONT_REAR	=		0x01;
	private _DIO2_TC_CYCLIC			=			0x1;
	private _DIO2_EC_GENERAL_TRIGGER =			0x00;
	private _DIO2_EC_START_TRIGGER		=		0x01;
	private _DIO2_EC_STOP_TRIGGER		=		0x02;

	private _DIO2_IO_CONNECTION_SOURCE_REGISTER	=		0x00;
	private _DIO2_IO_CONNECTION_SOURCE_BUFFER	=		0x01;
	private _DIO2_IO_CONNECTION_SOURCE_PXI_TRIGGER=		0x02;
	private _DIO2_IO_CONNECTION_SOURCE_TIMING	=		0x03;

	private _DIO2_IO_CONNECTION_SIDE_FRONT		=		0x00;
	private _DIO2_IO_CONNECTION_SIDE_REAR		=		0x01;
	private _DIO2_IO_CONNECTION_TERMINATION_ON	=		0x01;
	private _DIO2_IO_CONNECTION_TERMINATION_OFF	=		0x00;
	private _DIO2_IO_INT_ENABLE	=	0x1;
	private _DIO2_IO_INT_DISABLE	=	0x0;

	private _DIO2_TH_ASYNCHRONOUS	= 0x00;
	private _DIO2_TH_SYNCHRONOUS	= 0x01;
	private _DIO2_TH_OUTPUT_DISABLE	= 0x00;
	private _DIO2_TH_OUTPUT_ENABLE	= 0x01;



	private _NO_EVENT = -1;

	if( size( _synch_event ) == 1 && _synch_event[0] == _NO_EVENT ) _synch_event = [];

/*
	write(*, 'DIO2HWInit', _board_id, _ext_clock, _rec_event, _synch_event );
*/
	write(*, 'DIO2_ENCDECHWInit');

/* Initialize Library if the first time */
    _first = 0;
    if_error(_DIO2_initialized, (DIO2->DIO2_InitLibrary(); public _DIO2_initialized = 1; _first = 1;));


/* Open device */
	_handle = 0L;
	_status = DIO2->DIO2_Open(val(long(_board_id)), ref(_handle));
	if(_status != 0)
	{
		DevLogErr(_nid, "Error opening DIO2 device, board ID = "// _board_id);
		return(0);
	}
/* Reset module */
/*******
       if(_first) 
		DIO2->DIO2_Reset(val(_handle));
*******/
	_status = DIO2->DIO2_Cmd_TimingChannelDisarm(val(_handle),val(byte(255)));


	/*_status = DIO2->DIO2_TH_SetTimingHighway(val(_handle), val(byte(1)), val(byte(1)));*/
/* Set clock functions */
	if(_ext_clock)
	{
/*
write(*, " =============== Set timing highway external ============= " );        

	    _status = DIO2->DIO2_TH_SetTimingHighway(val(_handle), val(byte(1)), val(byte(0)));
*/
	    _status = DIO2->DIO2_TH_SetTimingHighway(val(_handle), val(byte(_DIO2_TH_SYNCHRONOUS)), val(byte(_DIO2_TH_OUTPUT_DISABLE)));
		_clock_source = byte(_DIO2_CLOCK_SOURCE_TIMING_HIGHWAY);

		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error setting highway configuration in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting highway configuration in DIO2 device, board ID = "// _board_id);
			return(0);
		}

	}
	else
	{
/*
write(*, " =============== Set timing highway internal ============= " );        

This configuration has been used in DIO2 Decoder device and works correctly.
The Encoder Decoder configurazione is required to set  aynchronous highway flag for
event propagation
	    _status = DIO2->DIO2_TH_SetTimingHighway(val(_handle), val(byte(_DIO2_TH_SYNCHRONOUS)), val(byte(_DIO2_TH_OUTPUT_ENABLE)));

*/
	    _status = DIO2->DIO2_TH_SetTimingHighway(val(_handle), val(byte(_DIO2_TH_ASYNCHRONOUS)), val(byte(_DIO2_TH_OUTPUT_ENABLE)));

		_clock_source = byte(_DIO2_CLOCK_SOURCE_INTERNAL);
	}	

		
	for(_c = 1; _c <= 8; _c++)
		_status = DIO2->DIO2_CS_SetClockSource(val(_handle), val(_clock_source), val(byte(_c)), val(byte(_DIO2_CLOCK_SOURCE_RISING_EDGE)));
	if(_status != 0)
	{
		if(_nid != 0)
			DevLogErr(_nid, "Error setting clock source in DIO2 device, board ID = "// _board_id);
		else
			write(*, "Error setting clock source in DIO2 device, board ID = "// _board_id);
		return(0);
	}

/* Set recorder start event and arm recorder */
	if(_rec_event != 0)
	{
		_status = DIO2->DIO2_ER_SetStartEvent(val(_handle), val(byte(_rec_event)), val(byte(_DIO2_ER_START_EVENT_INT_DISABLE)));
		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error setting recorder start event in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting recorder start event in DIO2 device, board ID = "// _board_id);
			return(0);
		}

		_status = DIO2->DIO2_Cmd_FlushEventRecorder(val(_handle));
		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error flushing recorder in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting flushing recorder  in DIO2 device, board ID = "// _board_id);
			return(0);
		}

		_status = DIO2->DIO2_Cmd_ArmEventRecorder(val(_handle));
		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error arming recorder in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting arming recorder  in DIO2 device, board ID = "// _board_id);
			return(0);
		}
	}


/* Set synch event if defined */

	for(_i = 0; _i < size(_synch_event); _i++)
	{
		_status = DIO2->DIO2_EC_SetEventDecoder(val(_handle), val(byte(_i+1)), val(byte(_synch_event[_i])),
			val(byte(255)), val(byte(_DIO2_EC_START_TRIGGER)));

		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error setting recorder start event in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting recorder start event in DIO2 device, board ID = "// _board_id);
			return(0);
		}
	}
/* Initialize remaining event register */

    	
    for(_i = size(_synch_event) + 1; _i <= 16; _i++)
	{

		_status = DIO2->DIO2_EC_SetEventDecoder(val(_handle), val(_i), val(byte(0)),
				val(byte(0)), val(byte(_DIO2_EC_GENERAL_TRIGGER))); 
		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error setting event in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting event in DIO2 device, board ID = "// _board_id);
			return(0);
		}
	}
    

/* OLD VERSION *****************
	if(_synch_event != 0)
	{
		_status = DIO2->DIO2_EC_SetEventDecoder(val(_handle), val(byte(10)), val(byte(_synch_event)),
			val(byte(255)), val(byte(_DIO2_EC_START_TRIGGER)));

		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error setting recorder start event in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting recorder start event in DIO2 device, board ID = "// _board_id);
			return(0);
		}
	}		
***************************/

/* Configure Outputs: channel i: output, channel i + 1 corresponding trigger */
/*
	for(_c = 0; _c < 8; _c++)
	{

		_status = DIO2->DIO2_IO_SetIOConnectionOutput(val(_handle), val(byte(2 * _c + 1)), 
			val(byte(_DIO2_IO_CONNECTION_SIDE_FRONT)), val(byte(_DIO2_IO_CONNECTION_SOURCE_TIMING)),
			val(byte(_c + 1)), val(byte(_DIO2_IO_CONNECTION_TERMINATION_OFF)), 
			val(byte(_DIO2_IO_INT_DISABLE)));  

		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error setting output configuration in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting output configuration  in DIO2 device, board ID = "// _board_id);
			return(0);
		}
		_status = DIO2->DIO2_IO_SetIOConnectionInput(val(_handle), val(byte(2 * _c + 2)),
			val(byte(_DIO2_IO_CONNECTION_SIDE_FRONT)), val(byte(_DIO2_IO_CONNECTION_TERMINATION_OFF)));
		if(_status != 0)
		{
			if(_nid != 0)
				DevLogErr(_nid, "Error setting input configuration in DIO2 device, board ID = "// _board_id);
			else
				write(*, "Error setting input configuration  in DIO2 device, board ID = "// _board_id);
			return(0);
		}
	}

*/

/* Close device */
	DIO2->DIO2_Close(val(_handle));
/*
write(*, 'OK end initialized');
*/
    return(1);
}
		
