# libzrØ

A cross-platform foundation header file, offering a unified set of data structures, memory management solutions, and other core components to accelerate project development and reduce complexity. 

> [!warning]
> Please note that libzro is an early-stage project, which means it has not yet undergone extensive testing and therefore should be used with caution in production environments.

**Features**

### Memory Management
#### Custom Allocators

* **Standard Allocator**: Compatible with existing code, featuring a wrapper around the standard `malloc`
function.
* **Arena Allocator**: A safer and more controlled memory management solution.

### Data Structures
#### Essential Collection

* **Dynamic Array**: A Vector or ArrayList-like data structure for efficient storage and manipulation.
* **Hash Table**: A Dictionary-like data structure for fast lookups and insertions.

### String Handling
#### New String API

* **Unicode/UTF-8 Friendly**: Efficiently manipulate and build Unicode/UTF-8 compatible strings.
