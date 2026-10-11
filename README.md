# uthreads

A User-Level threads library, including semaphore and channel implementations.
For CS-527 Parallel Computer Architecture, CSD-UoC.

## Library  

### uthreads.h 

| Call | Description   |  
| :--- | :--- |
| uthread_init | |
| uthread_new | |
| uthread_self ||
| uthread_pause ||
| uthread_join  | |
| uthread_exit||

### usem.h 

| Call | Description   |  
| :--- | :--- |
| usem_init||
| usem_wait ||
| usem_signal ||

### uchan.h 

| Call | Description |
| uchan_init | |
| uchan_send | |
| uchan_recv | |

## Usage

```bash
    mkdir build; cd build 
    cmake ..
    make
```

### CMake Options

```bash
    -DARCH=<architecture>   # Default = "x86_64"
    -DBUILD_TESTS=<ON/OFF>
    -DDEBUG=<ON/OFF> 

```

### Supported Architectures

 - x86_64
 - x86_i386
 - arm64
 - arm32
 - risc-v 
 - mips
 - sparc 
 - powerpc


