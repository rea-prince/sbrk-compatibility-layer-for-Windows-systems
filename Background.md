# Potential uses
I initially created this project to learn how to create my own `malloc()` and `free()` functions in C. However, this project could be applied for other purposes such as:
1. Porting Unix software to Windows
2. Creating cross-platform projects
3. General experimentation
4. Studying memory from resources that use `sbrk()` (my situation)

# How the header works
For those curious, here lies an explanation for how the function works. I included some references and additional information that I found useful while creating it.
### `windows.h`
First, we use functions from the `windows.h` header to call `VirtualAlloc` and return a memory address of type `void`.
- Void is a generic type for memory addresses in C.

### Function type
Defining `sbrk` in a header without static would produce a global symbol in every `.c` file, causing linking errors. `static inline` ensures that each translation unit gets its own copy. (recall that static functions are only visible to other functions within the same `.c` file).

### Variables
We use `char*` variables in this program too keep track of the heap since we're performing pointer arithmetic.
- `heap_end` is a variable that remembers where the heap currently ends. Every time `sbrk` is called, this pointer moves forward by `increment`.
- `heap_start` marks the beginning of the reserved memory block obtained from windows via `VirtualAlloc`.
- `heap_size` stores the total size of the reserved memory block in bytes. Used to make sure we don't go past the end of the heap.

### First call parameter
When the function is called for the first time, `heap_start` will be null. In this case, we reserve by default 1 megabyte of memory (which you can change according to your needs) with `VirtualAlloc`.
- `MEM_RESERVE | MEM_COMMIT` reserves memory (reserves virtual address space) and commits physical memory (makes it usable). `PAGE_READWRITE` allows reading/writing in memory.
In case the allocation fails, we return `(void*)-1` or `0xFFFFFFFFFFFFFFFF` just as `sbrk()` would.
- `0xFFFFFFFFFFFFFFFF` just means error as it's an impossible address (CPUs don't use the full 64 bits for addresses).

After priming the memory, we set the `heap_end` equal to the `heap_start`.

### Checking for memory
To check if we still have space to increment to, we only need to check if the current end of the heap + the increment is greater than the start of the heap + the reserved heap size. If it is, then we return `(void*)-1`. This ensures that we do not return memory outside the bounds of the reserved block, since the OS will not enforce it in this simulated heap.

### Active function calls
Once our heap is initialized, we only need to increment the end of the heap on each call, and return the address of the end of the previous call.

## References
https://www.youtube.com/watch?v=N2y6csonII4
https://www.youtube.com/watch?v=ocOFKb6V6XE
https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc