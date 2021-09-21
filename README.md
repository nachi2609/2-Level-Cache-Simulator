# 2-Level-Cache-Simulator

## Important Instructions about execution of the simulator 

 

### A two-level (L1 and L2) cache simulator is simulated. The cache simulator takes several parameters describing the cache (block size, associativity, etc) along with a memory access trace file for an input program. 

#### Simulator Output: 

For each cache access, the simulator outputs whether the access caused one of the five cache outcomes in the L1 and L2 caches: read hit,  write hit, read miss, write miss or if it was not accessed. Each event is coded with a number, as shown below. \ 

  0: No Access  \
  1: Read Hit  \
  2: Read Miss  \
  3: Write Hit  \
  4: Write Miss  

For example, if a write access misses in the L1 cache but hits in the L2 cache, the simulator would output: 4 3 \

Here, the first number corresponds to the L1 cache event and the second to the L2 cache event. \

## Cache Design 

   ● Read Miss: on a read miss, the cache will issue a read request for the data from the lower level cache. Once the data is received it is placed in an empty way if such a way exists. If not, the data in one of the ways is cleared to store the new data. The ways of the cache are numbered from {0,1,2..W-1} for a W-way cache. If an empty way exists, data is placed in lowest numbered empty way. Deletion is performed based on a round-robin policy - Each way has a counter that is initialized to 0, incrementing up to W-1 and back to zero. The current value of the counter indicates the way from which data is to be cleared/deleted. The counter is incremented by 1 after a deletion. 

   ● Write Hit: both the L1 and L2 caches are write-back caches. 

   ● Write Miss: both the L1 and L2 caches are write no-allocate caches. On a write miss, the write request is forwarded to the lower level of the cache. 

  ● Non-Inclusive: the L1 and L2 caches are non-inclusive. 

#### Configuration File (config.txt): 

The parameters of the L1 and L2 caches are specified in a configuration file as follows. 

  ● Block size: Block size for the cache in bytes.  

  ● Associativity: The associativity of the cache. A value of "1" implies a direct-mapped cache, while a "0" value implies fully-associative.  

  ● Cache size: The total size of the cache data array in kilobytes. 

#### A 16KB direct-mapped L1 cache with 4 byte blocks, and a 32KB 8-way set associative L2 cache with 2 byte blocks would be represented as follows: 

### Sample configuration file for L1 and L2 Cache: L1: 4 1 16 L2: 2 8 32 

The provided code reads the config file and initializes variables for the L1 and L2 cache parameters. 

#### Trace File (trace.txt): 

The simulator uses a trace file to compute the cache performance. It contains all the data memory accesses simulated. Each line in the trace file specifies an operation and a memory address.
