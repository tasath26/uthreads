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
│   ├── wrap                        # Wrapper functions for preemption
│   └── sig                         # Signal Handling for Preemption 
│       ├── handler.c
│       └── handler.h
│       
│       
└── tests                           # Testfiles (ctest)
```


## Library  

### Threads - uthreads.h 

| Call | Description   |  
| :--- | :--- |
| uthread_init | Initialized thread library |
| uthread_new | Creates thread and returns the TID |
| uthread_self | Returns the current thread's TID |
| uthread_pause | Yields execution to another thread |
| uthread_join  | Waits for a thread to finish |
| uthread_exit| Terminates calling thread |

### Semaphores - usem.h 

| Call | Description   |  
| :--- | :--- |
| usem_init | Initializes the semaphore object |
| usem_wait | Aquires semaphore, blocks if needed |
| usem_signal | Releases semaphore |

### Channels - uchan.h 

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
    
    export PKG_CONFIG_PATH="$PWD${PKG_CONFIG_PATH:+:$PKG_CONFIG_PATH}"
    gcc program -o main $(pkg-config --cflags --libs uthreads)

```

### CMake Options

```bash
    -DARCH=<architecture>                           # Default = "x86_64"
    -UTHREAD_STACK_SIZE=< size in KB >              # Default = 64 | Max = 8192 (8MB)
    -DUTHREAD_PRMT_INTERVAL_US=<time in microsec.>  # Default = 5000 (5ms)
    -DBUILD_TESTS=<ON/OFF>
    -DDEBUG=<ON/OFF> 

```

### Supported Architectures


 - **x86_64**      #Tested natively  
 - **x86_i386**    #Tested natively
 - **arm64**       #Tested natively on macOS
 - **arm32**       #Not Tested 
 - **risc-v**      #Tested using QEMU

## Testing 

Run the given examples and more custom tests:

```bash
    cd build/tests 
    ctest

```

# Preemption 

Preemption works using a signal handler that starts a timer and switches context every interval using a round-robin policy.
We have some regions that are protected (such as the creation of a thread or exiting) that we don't 
allow switches to occur. 

Also i added wrapper functions for the most common arbitrary calls that a user might do such as 
i/o or memory management like malloc, so that threads cannot switch until those important
operations have finished.

___ 

This assignment took 4-5 days to complete.

Athanasiadis Anastasios - csdp1440 - 2026
