#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

#include <inttypes.h>

class ShiftRegister {
  public:
    ShiftRegister() {}
    ~ShiftRegister() {}

    void Init();
    void Step();

    inline void SetProbability(int16_t prb) { prb_ = prb; }
    inline void SetRange(uint32_t rng) { rng_ = rng; }
    inline void SetLength(uint32_t len) { shift_step_ = 32/len; }
    inline void SetDivision(uint8_t div) { div_ = div; }

    inline uint8_t GetNoteValue() { return cur_val_; }
    inline uint8_t GetGateValue() { return cur_pls_; }

  private:
    // control params
    int16_t prb_ = 6000;
    uint32_t rng_ = 2;
    uint32_t shift_step_ = 0;
    uint32_t div_ = 1;

    // state
    uint32_t stp_cnt_ = 0;
    uint32_t cur_pls_ = 0;
    uint32_t cur_val_ = 0;

    // shift register
    uint32_t reg_ = 0;
};

#endif