
#define word_size_bit 8
#define windowsize_bit 12
#define length_bit 3
#define windowsize (1 << windowsize_bit)
#define max_length (1 << length_bit) + 1
#define c_bits (1 + windowsize_bit + length_bit)
#define length_mask (1 << length_bit) - 1
#define windowsize_mask ( (1 << windowsize_bit) - 1 ) << length_bit
#define c_bits_mask 1 << (c_bits - 1)

#include<vector>
#include<math.h>
#include<assert.h>
#include<iostream>
#include<algorithm>