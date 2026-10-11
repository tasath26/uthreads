# uthreads

A User-Level Preemptive threads library, including semaphore and channel implementations.
For CS-527 Parallel Computer Architecture, CSD-UoC.

## Structure 

```text
.
├── api                             # Public API 
│   ├── uchan.h
│   ├── usem.h
│   └── uthreads.h
├── CMakeLists.txt
├── README.md
├── src
│   ├── arch
│   │   ├── arch.h
│   │   └── <architecture>
│   │       ├── context.c           # Arch-specific context init  
│   │       ├── start.s             # Thread entry assembly code  
│   │       ├── switch.s            # Switch assembly code 
│   │       └── uthread_<arch>.h    # Context structs
│   ├── CMakeLists.txt
│   ├── common
│   │   ├── common.h            
│   │   └── structures              # Data Structures 
│   │       ├── queue.c          
│   │       ├── queue.h
│   │       ├── table.c
│   │       └── table.h
│   ├── impl                        # API Implementations 
│   │   ├── chan.c
│   │   ├── sem.c
│   │   └── threads.c
│   └── sig                         # Signal Handling for Preemption 
│       ├── handler.c
│       └── handler.h
└── tests
    ├── CMakeLists.txt
    ├── hello.c
    └── stress_test.c
```


## Library  

### uthreads.h 

| Call | Description   |  
| :--- | :--- |
| uthread_init | Initialized thread library |
| uthread_new | Creates thread and returns the TID |
| uthread_self | Returns the current thread's TID |
| uthread_pause | Yields execution to another thread |
| uthread_join  | Waits for a thread to finish |
| uthread_exit| Terminates calling thread |

### usem.h 

| Call | Description   |  
| :--- | :--- |
| usem_init | Initializes the semaphore object |
| usem_wait | Aquires semaphore, blocks if needed |
| usem_signal | Releases semaphore |

### uchan.h 

| Call | Description |
| :--- | :--- |
| uchan_init | Initializes the channel object |
| uchan_send | Sends data through the channel |
| uchan_recv | Recieves data from the channel |

## Usage

```bash
    mkdir build; cd build 
    cmake <options> ..
    make
```

### CMake Options

```bash
    -DARCH=<architecture>                # Default = "x86_64"
    -UTHREAD_STACK_SIZE=< size in KB >   # Default = 64
    -DBUILD_TESTS=<ON/OFF>
    -DDEBUG=<ON/OFF> 

```

### Supported Architectures

 - **x86_64**
 - **x86_i386**
 - **arm64**
 - **arm32**
 - **risc-v** 
 - **mips**
 - **sparc** 
 - **powerpc**


## Testing 


```bash


```

___ 

Athanasiadis Anastasios - csdp1440 - 2026
