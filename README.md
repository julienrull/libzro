# libzrØ

**libzro** is a crossplateform boilerplate header file that implement essential data structures, allocators and more. 

> [!warning]
> It is still in early stage development, so it comes without any guarantee to work properly in a production context.

**libzro** is a personal set of libs for my own usecases first, but also for educational purposes.

## Features

- **zro_alc.h**: contextual generic allocator mechanisme with custom allocators. 
  - std (malloc) allocator wrapper
  - arena allocator, an alternative to std malloc for safer and easier memory lifetimes managment. 

- **zro_ds.h**: Collection of essential data structure. (These are not implemented by C language out of the box compare to more modern programming languages) 
  - dynamic array data structure (aka Vector or ArrayList).
  - hash table (dictionary like data structure) 

- **zro_str.h**: new string API for easier unicode/UTF-8 compatible strings manimulation and string building. 

- **zro_t.h**: collection of useful type for libzro to work properly. 
