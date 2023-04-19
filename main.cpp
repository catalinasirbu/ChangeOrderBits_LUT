// Github link:
// https://github.com/catalinasirbu/ChangeOrderBits_LUT.git

// Output:
// Ran in 2207852 us = 2.2 seconds

// CPU used: Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz
// gcc (x86_64-posix-seh-rev2, Built by MinGW-W64 project) 12.2.0

#include <iostream>
#include <chrono>
#include <bitset>

#define VECTOR_SIZE (100*1000*1000UL)
alignas(64) uint32_t t[VECTOR_SIZE];

uint8_t lut8[256];

#define BITARR(i, y) man |= lut8[(u >> i) & 0xFF] << (y - (i / 2) * 8);
#define BITARR4(i) BITARR(i, 31); BITARR(i + 2, 23); BITARR(i + 1, 15); BITARR(i + 3, 7);

static inline void swapu32(uint32_t& u) {
    uint32_t man = 0;
    BITARR4(0);
    BITARR4(4);
    BITARR4(8);
    BITARR4(12);
    u = man;
}

void init() {
    for (uint32_t i = 0; i < VECTOR_SIZE; i++)
        t[i] = rand();
}

using namespace std::chrono;
uint64_t getMicroseconds() {
    auto now = high_resolution_clock::now();
    auto micros = duration_cast<microseconds>
            (now.time_since_epoch()).count();
    return micros;
}

void init_lut8() {
    for (int i = 0; i < 256; i++) {
        lut8[i] = 0;
        lut8[i] |= ((i & 0x01) >> 0) << 7; // bit0
        lut8[i] |= ((i & 0x04) >> 2) << 6; // bit2
        lut8[i] |= ((i & 0x10) >> 4) << 5; // bit4
        lut8[i] |= ((i & 0x40) >> 6) << 4; // bit6

        lut8[i] |= ((i & 0x02) >> 1) << 3; // bit1
        lut8[i] |= ((i & 0x08) >> 3) << 2; // bit3
        lut8[i] |= ((i & 0x20) >> 5) << 1; // bit5
        lut8[i] |= ((i & 0x80) >> 7) << 0; // bit7
    }
}

int main() {
    std::cout << "Started " << std::endl;
    init();
    std::cout << "Init done " << std::endl;

    t[0] = 3;
    std::bitset<32> bs1(t[0]);

    init_lut8();

    auto tstart = getMicroseconds();
    for (uint32_t i = 0; i < VECTOR_SIZE; i+=4) {
        swapu32(t[i]);
        swapu32(t[i+1]);
        swapu32(t[i+2]);
        swapu32(t[i+3]);
    }
    auto tstop = getMicroseconds();

    std::bitset<32> bs2(t[0]);
    std::cout << bs1 << std::endl;
    std::cout << bs2 << std::endl;

    std::cout << "Ran in " << (tstop - tstart) << " us"  << std::endl;
    return 0;
}