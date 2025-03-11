# libzro

A header-only C library that provides essential data structures, including:

- **Generic memory management**
- **Arena allocator**
- **Dynamic array**
- **Hashmap** (TODO)

## Getting Started

### Download

```sh
wget https://raw.githubusercontent.com/julienrull/libzro/refs/heads/main/zro.h
```

OR

```sh
curl https://raw.githubusercontent.com/julienrull/libzro/refs/heads/main/zro.h -o libzro.h
```

### Setup

Add this in one translation unit (One source file):

```c
#define ZRO_IMPLEMENTATION
#include "zro.h"
```

In other files, when you need to use the library, juste include "zro.h"
