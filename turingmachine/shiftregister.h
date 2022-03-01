#ifndef SHIFT_REGISTER_H_
#define SHIFT_REGISTER_H_

#include <inttypes.h>

class ShiftRegister {
  public:
    ShiftRegister() {}
    ~ShiftRegister() {}

    void Init();
    void Step();

    inline void SetProbability(uint8_t prb) { prb_ = prb; }
    inline void SetRange(uint8_t rng) { rng_ = rng; }
    inline void SetLength(uint8_t len) { len_ = len; }
    inline void SetDivision(uint8_t div) { div_ = div; }

    inline uint8_t GetNoteValue() { return cur_val_; }
    inline uint8_t GetGateValue() { return cur_pls_; }

  private:
    // control params
    int16_t prb_ = 600;
    uint32_t rng_ = 2;
    int32_t len_ = 8;
    uint32_t div_ = 1;

    // state
    uint32_t stp_cnt_ = 0;
    uint32_t cur_pls_ = 0;
    uint32_t cur_val_ = 0;

    // shift register
    uint32_t reg_ = 0;
};

#endif