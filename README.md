dem
===

RGSS Data Exchange Model

undef LGPL license

We use this model to exchange data between different languages, mostly on RGSS(1-3), without the knowledges in:
  1) how to perform a call to modules in other languages,
  2) how to react on a call from modules in other languages.

We define dem_node like this:
  1) 32bit signed integer : class/type specification
  2) 32bit integer(not always signed or unsigned): data/pointer to data
 altogether 64bits = 8 bytes = length of a QWord(long long/__int64)

Conventions:
  *The class number of Integer = 1
  *The class number of 32bit Float (aka IEEE single precision) = 2
  *The class number of Array < 0 
  *Other class number < 65536 is a predefined class tag,
  *Class number >= 65536 is a real address in 32bit. 
     Either a pointer to a struct to certain class, or a function.
     At this time, the lower part of the QWord is the object address or some user-defined data.


for example:
  3      : integer     =>  0x00000001000000003 (Integer  class, data = 3)
  5.5    : float(32bit) => 0x0000000240b000000 (Float(32bit) class, data = 5.5f)
  [1,2,3]: Array in length 3 => 0xfffffffd12345678 ([-3, 0x12345678])
                              -3 means Array length is 3, 0x12345678 is the address of the array:
                from 0x12345678, you may read like a int[]:
                (int []){1, 1, 1, 2, 1, 3};

  "abc" : String "abc" may like 0x8765432112345678:  
      *) 0x87654321 is the address to a String class struct(like in ruby),
      *) 0x12345678 is the address to "abc".



  
