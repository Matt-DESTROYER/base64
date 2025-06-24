# Base64

Simple Base64 encoding functionality.
Currently only encoding has been implemented.


## Documentation:
```c
char* base64_encode(char* input_buffer, size_t size);
```
Takes in an array of `char`s and outputs a new array with the values encoded in Base64.

## How to use:
Just grab the `base64.h` and `base64.c` files, include them where needed, and add them to the compile.
```c
// ...
#include "base64.h"
// ...
```

```bash
gcc main.c base64.c -o main 
```
