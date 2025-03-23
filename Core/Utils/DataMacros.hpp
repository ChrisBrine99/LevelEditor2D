#ifndef DATA_MACROS_HPP
#define DATA_MACROS_HPP

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A simple define that stores how many sprites I expect the engine to have loaded simultaneously on average. The true value can be	//
//	below or above this whenever required for a performance hit when deallocating or allocating additional space, respectively.			//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define SPRDATA_RESERVE_AMOUNT			10ui64

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define ID_TSHEET_TEST					0x0000ui16

// ------------------------------------------------------------------------------------------------------------------------------------ //
//	A simple default value that represents an invalid ID within the various data structures in the game. As a result, they must all be	//
//	16-bit values ranging from 0-65534 AKA no more than 65535 unique pieces of data can exist for any one group.						//
// ------------------------------------------------------------------------------------------------------------------------------------ //

#define ID_INVALID						0xFFFFui16

#endif