#include "shiftregister.h"
#include "stmlib/utils/random.h"

void ShiftRegister::Init() {
    // Note: this assumes random has been properly seeded prior to calling

    // It's OK to leave the control params at whatever value they're currently at...
    //  They get defaulted appropriately in the class definition
    //  IF this is called later on in runtime, that will let them retain their settings (which should match the UI at that moment)

    // Reset state params
    stp_cnt_ = 0;
    cur_pls_ = 0;
    cur_val_ = 0;

    // Fill the register with a randomized WORD value
    reg_ = stmlib::Random::GetWord();

    SetLength(8);
}

void ShiftRegister::Step() {
    stp_cnt_++;

    // No need to do anything if the step doesn't fall on the set division
    if (stp_cnt_ % div_)
        return;

    // rotate the whole register by n=32/len steps
    // NOTE: the shift_step_ value can potentially be 32, which would qualify as UB when using as a shift amount
    //          ...so far in testing it is working fine on STM32 and Apple M1
    reg_ = (reg_ >> shift_step_) | (reg_ << (-shift_step_ & static_cast<uint32_t>(31)));

    // Compute new/current NOTE value
    cur_val_ = reg_ & static_cast<uint32_t>(0xFF);

    // Set GATE status
    // TODO: should this be based on a mask, like flip_mask?
    cur_pls_ = reg_ & static_cast<uint32_t>(1);

    // mutate the register based on probability
    // TODO: change the flip_mask based on sequence length(?)
    if (stmlib::Random::GetSample() < prb_) {
        uint32_t flip_mask = 0xFF;
        reg_ ^= flip_mask;
    }
}