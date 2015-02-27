/*	
 *
 *	Pierwsze zadanie konkursu "Algorytmion 2015"
 *	
 *	Dodatkowe parametry kompilacji: -o2
 *
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef __GNUC__
#warning "This program has been compiled ONLY by GNU/C."
#endif

#ifndef __linux
#warning "OS not supported"

#ifdef __WIN32__
#define getc_unlocked getc
#endif

#endif


const char *FILENAME_UNENCRYPTED	= "tekst.txt";
const char *FILENAME_ENCRYPTED		= "szyfr.txt";
const char *FILENAME_OUTPUT		= "odszyfrowane.txt";


const char fMODE_READ[2]	= "r+";
const char fMODE_WRITE[1]	= "w";

// Tables definitions
signed char *unencryptedTextArray;
unsigned int *encryptedTextArray;

// Functions are inline for cleanliness of code
inline short convert( signed char cVar );		// This one will decrease signed char by 0x30
inline unsigned int power( short p, short num );// Exponentiation
inline void tEncrypt( unsigned short fileSize );// Encrytption funciton
inline void tDecrypt( unsigned short fileSize );// Decryption function

main()
{
	unsigned short hVar = 0;

	while(1)		// Menu loop.
	{
		printf( "Do you want to encrypt[1] or decrypt[2] file?\n[0] Exit " );
		hVar = getchar();
		if( hVar == '1' )
		{
			hVar = 1;
			break;
		}
		else if( hVar == '2' )
		{
			hVar = 2;
			break;
		}
		else if( hVar == '0' )
			return 0;

		hVar ^= hVar;
	}			// End of menu loop

	FILE *fHandle;	// Our files handle

	unsigned short fSIZE;

	if( 1 == hVar )
	{
		if( (fHandle = fopen( FILENAME_UNENCRYPTED, fMODE_READ )) == NULL )	// Trying to open file
		{
			perror( "Error: fopen()" );
			return 1;
		}
		printf( "File \"%s\" opened.\n", FILENAME_UNENCRYPTED );
	}
	else if( 2 == hVar )
	{
		if( (fHandle = fopen( FILENAME_ENCRYPTED, fMODE_READ )) == NULL )	// Trying to open file
		{
			perror( "Error: fopen()" );
			return 1;
		}
		printf( "File \"%s\" opened.\n", FILENAME_ENCRYPTED );
	}

	fseek( fHandle, 0L, SEEK_END );		// Find EOF

	fSIZE = ftell( fHandle );		// Return file size

	// Allocating memory for tables
	if( !( unencryptedTextArray = malloc( sizeof(*unencryptedTextArray) * ( fSIZE + 1 ) ) ) )
	{
		perror( "Error: malloc() - *unencryptedTextArray" );
		return 2;
	}

	if( !( encryptedTextArray = malloc( sizeof(*encryptedTextArray) * ( fSIZE + 1 ) ) ) )
	{
		perror( "Error: malloc() - *encryptedTextArray" );
		return 2;
	}	// End of alloc

	fseek( fHandle, 0L, SEEK_SET );		// Return to the begining of file

	switch( hVar )	// Main switch
	{
		case 0x1:	// START OF CASE 1
			{
				while( ( *unencryptedTextArray = getc_unlocked(fHandle) ) != EOF )	// File reading loops
				{
					printf( "%c", *unencryptedTextArray );			// To be sure
					unencryptedTextArray++;
				}

				fclose( fHandle );	// Close file

				tEncrypt( fSIZE );	// Encrypt table of chars

				if( (fHandle = fopen( FILENAME_ENCRYPTED, fMODE_WRITE ) ) == NULL )	// Trying to open file
				{
					perror( "Error: fopen()" );
					return 1;
				}

				encryptedTextArray -= fSIZE-1;

				for( fSIZE; fSIZE > 0; --fSIZE )			// Writing file loop
				{
					if( *encryptedTextArray == 0 )
						break;
					if( 1 == fSIZE )
						fprintf( fHandle, "%d", *encryptedTextArray );
					else
						fprintf( fHandle, "%d ", *encryptedTextArray );

					encryptedTextArray++;
				}											// End of writing file loop

				fclose(fHandle);	// We're done
			}break;	// END OF CASE 1
		case 0x2:	// START OF CASE 2
			{
				fSIZE ^= fSIZE;
				hVar ^= hVar;

				{ // START OF LIFETIME for countVar
					unsigned short countVar;
// This loop have to read word until 0x20 ( ASCII space 32(10) ) appear
					while( ( *unencryptedTextArray = getc_unlocked(fHandle) ) != EOF )
					{
						hVar++;
// If char 0x20 appeared, translate signed char* table into a number 
						if( *unencryptedTextArray == 0x20 || *unencryptedTextArray == EOF )
						{
							countVar = 0;

							while( countVar < hVar-1 )
							{
								unencryptedTextArray--;

								*encryptedTextArray += ( convert(*unencryptedTextArray) * power(countVar, 10) );

								countVar++;
							}

							hVar ^= hVar;

							encryptedTextArray++;

							fSIZE++;
						} // END OF IF
						unencryptedTextArray++;
					} // END OF LOOP
				} // END OF LIFETIME

				fclose( fHandle );	// Close file

				tDecrypt( fSIZE ); // Decrypt table od numbers

				if( (fHandle = fopen( FILENAME_OUTPUT, fMODE_WRITE )) == NULL )	// Trying to open a file
				{
					perror( "Error: fopen()" );
					return 1;
				}

				unencryptedTextArray -= fSIZE;

				for( fSIZE; fSIZE > 0; fSIZE-- )		// Writing our result into a file
				{
					printf( "%c ", *unencryptedTextArray );

					fprintf( fHandle, "%c", *unencryptedTextArray );

					unencryptedTextArray++;
				}

				fclose(fHandle);
			}break;
	}
	//zapis do pliku

	return 0;	// Well done!
}

inline short convert( signed char cVar )
{
	return ( (short)cVar - 0x30 );
}

inline unsigned int power( register short j, register short num )
{
	if( j == 0 )
		return 1;
	unsigned int hVar = num;

	while( j != 1 )
	{
		hVar *= num;
		j--;
	}
	return hVar;
}

inline void tEncrypt( unsigned short fileSize )
{
	register unsigned short numSys = 2;
	register unsigned int loop = 0;		// Loop counter

	unsigned char helpCharVar;

	unencryptedTextArray -= fileSize+1;

	while( --fileSize > 0 )				// Main encryption loop
	{
		unencryptedTextArray++;
		
		printf( "%c %d\t= ", *unencryptedTextArray, *unencryptedTextArray );

		*encryptedTextArray ^= *encryptedTextArray;

		helpCharVar = *unencryptedTextArray;

		while(1)						// Math things, conversion between decimal and other numeric systems
		{
			if( *unencryptedTextArray < numSys )
			{
				*encryptedTextArray += ( *unencryptedTextArray * power( loop, 10 ) );
				break;
			}
			else
			{
				*encryptedTextArray += ( ( *unencryptedTextArray % numSys ) * power( loop, 10 ) );
				*unencryptedTextArray /= numSys;
			}
			loop++;
		}

		numSys = ( helpCharVar & 7 ) + 2;

		loop ^= loop;

		encryptedTextArray++;
	}
}

inline void tDecrypt( unsigned short fileSize )
{
	register unsigned short numSys = 2;
	register unsigned int loop = 0;		// Loop counter

	encryptedTextArray -= fileSize;

	while( fileSize-- > 0 )	// Main decryption loop
	{
		*unencryptedTextArray ^= *unencryptedTextArray;

		while(1)			// Math things again
		{
			if( *encryptedTextArray < numSys )
			{
				*unencryptedTextArray += ( *encryptedTextArray * power( loop, numSys ) );
				break;
			}
			else
			{
				*unencryptedTextArray += ( ( *encryptedTextArray % 10 ) * power( loop, numSys ) );
				*encryptedTextArray /= 10;
			}
			loop++;
		}
		
		numSys = ( *unencryptedTextArray % 8 ) + 2;

		loop ^= loop;

		unencryptedTextArray++;

		encryptedTextArray++;
	}
}
