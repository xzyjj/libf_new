/* C type statement */

/*
* sizeof base:
*   char      == 1
*   short     == 2
*   int       == 4
*   long      == 8 (machine 32bit) : long      == 4
*   long long == 8 (machine 32bit) : long long == 8
*   void      == ? (depends on the compiler)
*
* sizeof pointer:
*   char *+      == 8 (machine 32bit) : char *+      == 4
*   short *+     == 8 (machine 32bit) : short *+     == 4
*   int *+       == 8 (machine 32bit) : int *+       == 4
*   long *+      == 8 (machine 32bit) : long *+      == 4
*   long long *+ == 8 (machine 32bit) : long long *+ == 4
*   void *+      == 8 (machine 32bit) : void *+      == 4
*
* NOTE: 'void' pointer, accept any type pointer
*
* NOTE: Pointer are essentially memory address, dereference
*       to obtain the pointer object (memory)
*
* NOTE: Pointer address offset, (int *)p + 1 == (char *)p + 4
*/

char  _char_1;
short _shor_1;
int   _int_1;
long  _long_1;

signed char    _char_2;
unsigned char  _char_3;
signed short   _short_2;
unsigned short _short_3;
signed int     _int_2;
unsigned int   _int_3;
signed long    _long_2;
unsigned long  _long_3;

signed   _int_4;
unsigned _int_5;

signed long int   _long_4;
unsigned long int _long_5;

signed long long   _long_long_1;
unsigned long long _long_long_2;

signed long long int   _long_long_3;
unsigned long long int _long_long_4;

/*
* NOTE: The order of 'char', 'short', 'int', 'long', 'signed',
*       and 'unsigned' is irrelevant
*/
long unsigned int long _long_long_5;

/* NOTE: The type and name are consistent and repeatable */
long int long unsigned _long_long_5;

char *  _ptr_char_1;
short * _ptr_shor_1;
int *   _ptr_int_1;
long *  _ptr_long_1;

signed char *    _ptr_char_2;
unsigned char *  _ptr_char_3;
signed short *   _ptr_short_2;
unsigned short * _ptr_short_3;
signed int *     _ptr_int_2;
unsigned int *   _ptr_int_3;
signed long *    _ptr_long_2;
unsigned long *  _ptr_long_3;

signed *   _ptr_int_4;
unsigned * _ptr_int_5;

signed long int *   _ptr_long_4;
unsigned long int * _ptr_long_5;

signed long long *   _ptr_long_long_1;
unsigned long long * _ptr_long_long_2;

signed long long int *   _ptr_long_long_3;
unsigned long long int * _ptr_long_long_4;

long unsigned int long * _ptr_long_long_5;

long int long unsigned * _ptr_long_long_5;

/* NOTE: Multiple pointer, define dereference limits */
int   ** _ptr_int_6;
int  *** _ptr_int_7;
int **** _ptr_int_8;

int main(void) {
	(void)_char_1;
	(void)_shor_1;
	(void)_int_1;
	(void)_long_1;
	(void)_char_2;
	(void)_char_3;
	(void)_short_2;
	(void)_short_3;
	(void)_int_2;
	(void)_int_3;
	(void)_long_2;
	(void)_long_3;
	(void)_int_4;
	(void)_int_5;
	(void)_long_4;
	(void)_long_5;
	(void)_long_long_1;
	(void)_long_long_2;
	(void)_long_long_3;
	(void)_long_long_4;
	(void)_long_long_5;
	(void)_long_long_5;
	(void)_ptr_char_1;
	(void)_ptr_shor_1;
	(void)_ptr_int_1;
	(void)_ptr_long_1;
	(void)_ptr_char_2;
	(void)_ptr_char_3;
	(void)_ptr_short_2;
	(void)_ptr_short_3;
	(void)_ptr_int_2;
	(void)_ptr_int_3;
	(void)_ptr_long_2;
	(void)_ptr_long_3;
	(void)_ptr_int_4;
	(void)_ptr_int_5;
	(void)_ptr_long_4;
	(void)_ptr_long_5;
	(void)_ptr_long_long_1;
	(void)_ptr_long_long_2;
	(void)_ptr_long_long_3;
	(void)_ptr_long_long_4;
	(void)_ptr_long_long_5;
	(void)_ptr_long_long_5;
	(void)_ptr_int_6;
	(void)_ptr_int_7;
	(void)_ptr_int_8;

	return 0;
}
