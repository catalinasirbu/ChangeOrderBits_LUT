// Github link:
// https://github.com/catalinasirbu/ChangeOrderBits_LUT.git

// Output:
//

// CPU used: Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz
// gcc (x86_64-posix-seh-rev2, Built by MinGW-W64 project) 12.2.0

#include <iostream>
#include <chrono>
#include <bitset>

#define VECTOR_SIZE (100*1000*1000UL)
uint32_t t[VECTOR_SIZE];

#define BITARR(i, y) man |= ((u >> i) & 1) << (y - (i / 2));
#define BITARR2(i) BITARR(i, 31);BITARR(i+1, 15);
#define BITARR4(i) BITARR2(i); BITARR2(i+2);
#define BITARR8(i) BITARR4(i); BITARR4(i+4);

static inline void swapu32(uint32_t& u) {
    uint32_t man = 0;
    BITARR8(0);
    BITARR8(8);
    BITARR8(16);
    BITARR8(24);
    u = man;
}

void init() {
    for (uint32_t i = 0; i < VECTOR_SIZE; i++)
        t[i] = rand();
}

using namespace std::chrono;
uint64_t getMilliseconds() {
    auto now = system_clock::now();
    auto millis = duration_cast<milliseconds>
            (now.time_since_epoch()).count();
    return millis;
}

uint8_t lut8[256];
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

    auto tstart = getMilliseconds();
    for (uint32_t i = 0; i < VECTOR_SIZE; i++)
        swapu32(t[i]);
    auto tstop = getMilliseconds();

    //t[0] = swapu32(t[0]);
    std::bitset<32> bs2(t[0]);
    std::cout << bs1 << std::endl;
    std::cout << bs2 << std::endl;

    std::cout << "Ran in " << (tstop - tstart) << " ms"  << std::endl;
    return 0;
}