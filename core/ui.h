// Copyright 2015 Matthias Puech.
//
// Author: Matthias Puech (matthias.puech@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// User Interface

#ifndef BATUMI_UI_H_
#define BATUMI_UI_H_

#include "stmlib/stmlib.h"

#include "stmlib/ui/event_queue.h"

#include "drivers/adc.h"
#include "drivers/leds.h"
#include "drivers/switches.h"

namespace batumi {

const uint8_t kFinePotDivider = 8;

enum UiMode {
    UI_MODE_SPLASH,
    UI_MODE_NORMAL/*,
    UI_MODE_ZOOM,*/
};

class Ui {
  public:
    Ui() {}
    ~Ui() {}

    void Init(Adc *adc);
    void Poll();
    void DoEvents();
    void FlushEvents();

    inline uint16_t coarse(uint8_t channel) {
        return pot_coarse_value_[channel];
    }

    inline int16_t fine(uint8_t channel) {
        return pot_probability_value_[channel] - 32768;
    }

    inline uint16_t phase(uint8_t channel) { return pot_div_value_[channel]; }

    inline uint16_t level(uint8_t channel) { return pot_range_value_[channel]; }

    inline uint16_t atten(uint8_t channel) { return pot_length_value_[channel]; }

    inline UiMode mode() const { return mode_; }
    
    inline uint8_t shape() const {
        return (switches_.pressed(2) << 1) | switches_.pressed(1);
    }
    inline bool sync_mode() const { return switches_.pressed(0); }

  private:
    void OnSwitchPressed(const stmlib::Event &e);
    void OnSwitchReleased(const stmlib::Event &e);
    void OnPotChanged(const stmlib::Event &e);

    uint16_t pot_value_[4];
    uint16_t pot_filtered_value_[4];
    uint16_t pot_coarse_value_[4];
    uint8_t focus_channel_ = 0;
    uint32_t press_time_[kNumSwitches];
    bool detect_very_long_press_[kNumSwitches];
    bool catchup_state_[4];

    int32_t animation_counter_;

    stmlib::EventQueue<32> queue_;

    Leds leds_;
    Switches switches_;
    Adc *adc_;
    UiMode mode_;

    // This originally added up to 42 in the stock firmware
    // FeatureMode feat_mode_;             // +4 bytes?
    // WaveBank bank_;                     // +4 bytes?
    uint16_t pot_probability_value_[4]; // +8 bytes
    uint16_t pot_range_value_[4];       // +8 bytes
    uint16_t pot_length_value_[4];      // +8 bytes
    uint16_t pot_div_value_[4];         // +8 bytes
    uint8_t padding[10];                // +10 bytes (was +2 originally)

    enum SettingsSize {
        SETTINGS_SIZE = /*sizeof(feat_mode_) + sizeof(bank_) +*/
                        sizeof(pot_probability_value_) + sizeof(pot_div_value_) +
                        sizeof(pot_range_value_) + sizeof(pot_length_value_) +
                        sizeof(padding)
    };

    uint16_t version_token_;

    DISALLOW_COPY_AND_ASSIGN(Ui);
};

} // namespace batumi

#endif // BATUMI_UI_H_
