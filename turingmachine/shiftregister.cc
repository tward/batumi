#include "shiftregister.h"
#include "stmlib/utils/random.h"

void ShiftRegister::Init() {
    // Note: this assumes random has been properly seeded prior to calling
    // TODO: write this method in a way that allows for easily calling at will, at runtime, in the event the UI adds a reset/randomize feature

    // It's OK to leave the control params at whatever value they're currently at...
    //  They get defaulted appropriately in the class definition
    //  IF this is called later on in runtime, that will let them retain their settings (which should match the UI at that moment)

    // Reset state params
    stp_cnt_ = 0;
    cur_pls_ = 0;
    cur_val_ = 0;

    // Fill the register with a randomized WORD value
    reg_ = stmlib::Random::GetWord();
    // reg_ = rand();
}

void ShiftRegister::Step() {
    stp_cnt_++;

    // No need to do anything if the step doesn't fall on the set division
    // if (stp_cnt_ % div_)
        // return;

    // Capture the LSB(s)
    uint32_t lsb = reg_ & static_cast<uint32_t>(1);
    uint32_t lsb_ex = (len_ < 32) ? (reg_ & (static_cast<uint32_t>(1) << len_)) >> len_ : 0;

    // Shift the register
    reg_ = reg_ >> 1;

    // Put LSB back in MSB
    // number = (number & ~(1UL << n)) | (x << n);
    // TODO: need to figure out why this doesn't work with (len_ - 1)!
    reg_ = (reg_ & ~(static_cast<uint32_t>(1) << (8 - 1))) | (lsb << (8 - 1));
    if (len_ < 32)
        reg_ = (reg_ & ~(static_cast<uint32_t>(1) << 31)) | (lsb_ex << 31);

    // Roll the dice to flip LSB
    if (stmlib::Random::GetSample() < prb_) {
        reg_ ^= static_cast<uint32_t>(1);
    }

    // Compute new/current NOTE value
    cur_val_ = reg_ & static_cast<uint32_t>(0xFF);

    // Set GATE status
    cur_pls_ = lsb;
}