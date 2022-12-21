# Umalloc

	To keep track of the metadata I used a struct that contains a size_t size which keeps track of how big the chunk is, an int free which keeps track of whether the memory chunk is free or in use with a 0 or 1. It also has two pointers one for the next chunk and one that points to the previous one. This is to make memory combining on free faster since it can easily check if either neighbor is free and then combine with them. The timings for the tests were found using clock_t before and after and then subtracting them and dividing by CLOCKS_PER_SEC to find the number of seconds that elapsed on the ilab machines. 
	The umalloc function finds the first available free chunk that is a big enough size, then creates metadata and returns a pointer to the available chunk. It checks for errors if malloc is called for 0 bytes, or if there is no available space for malloc, or if there is enough space available, but no chunk large enough. 
The ufree function sets the memory chunk to free and checks its neighboring chunks to see if they are free and combines with them if so. It prints an error if the pointer passed is NULL, or if the memory is uninitialized, or  if the pointer doesn’t point to the start of a memory chunk which also prevents redundant freeing of the same pointer. 

Tests:
The Consistency test returned the same address for both malloc calls to an int which was written to. It took 0.000107 seconds. 
The Maximization test worked and gave a max size of 524288 bytes. It took 0.000055 seconds.
The Basic Coalescence test could malloc half size and quarter size, free both, then malloc full size. It took 0.000004 seconds.
The Saturation test did 9k 1024 byte malloc calls then did 1 byte malloc calls. It also stored the pointers in an array, so they could be later freed by test #6. T took 0.025950 seconds.
The time overhead test took 0.00003 seconds to do one malloc call. 
Intermediate Coalescence went through the array and freed every pointer. Then it did a malloc on maximum size and that fit. It then freed that leaving nothing in use in the memory. It took 0.000011 seconds. 
