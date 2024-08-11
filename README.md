# ZrØ - Core Library

**libzro** is a crossplateform boilerplate library for sanity and peaceful mind C programming ✌️

> [!warning]
> It is still in early stage development, so it comes without any guarantee to work properly in a production context. Thank you for your understanding. 

**libzro** is the core fundation of a bigger toolchains project called "ZrØ". It's apersonal project primarely for my hobby and professional usecases but also for educational purposes. You can find the sources **libzro** is inspired of below. 

## What libzro provides

### Legend

🔴 - Not implemented or partialy implemented

🟠 - Implemented but not tested

🟢 - Implemented and tested

### Features

- **zro_alloc.h**: contextual generic allocator mechanisme. 🟢
- **zro_arena.h**: arena allocator, an alternative to malloc for safer and easier memory lifetimes managment. 🟢
- **zro_dynarr.h**: dynamic array data structure (aka Vector or ArrayList). 🟢
- **zro_string.h**: new string API for easier unicode/UTF-8 compatible strings manimulation. 🟠
- **zro_buff.h**: Stream bytes buffer data structures for easier data flow
  managment. 🔴
- **zro_type.h**: collection of useful type for libzro to work properly. 🔴
