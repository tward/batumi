// Copyright 2015 Matthias Puech
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
// Processor. Orchestrates the four LFOs.

#include "drivers/adc.h"
#include "drivers/dac.h"

#include "processor.h"
#include "ui.h"

namespace batumi {

using namespace stmlib;

const int16_t kUnsyncPotThreshold = INT16_MAX / 20;
const int16_t kResetThresholdLow = 10000;
const int16_t kResetThresholdHigh = 20000;

void Processor::Init(Ui *ui, Adc *adc, Dac *dac) {
    ui_ = ui;
    adc_ = adc;
    dac_ = dac;
    
    for (uint8_t i = 0; i < kNumChannels; i++) {
        reset_trigger_armed_[i] = false;
        machines_[i].Init();
    }
}

void Processor::Process() {

    // do not run during the splash animation
    if (ui_->mode() == UI_MODE_SPLASH)
        return;

    // detect triggers on the reset input
    // TODO: this should be toggleable between:
    //  - first channel input clocks all machines
    //  - each machine uses its own reset input as a clock
    //  Need to enable this via the jumper/POTI polling
    int16_t reset = adc_->reset(0);

    for (int i = 0; i < kNumChannels; i++) {
        // FOR EACH MACHINE:
        // - CHECK IF CLOCK INPUT (RESET) RECEIVES A PULSE
        // - STEP THE MACHINE IF PULSE WAS RECIEVED
        // - SET THE SINE OUTPUT DAC VALUE TO THE MACHINE'S GATE STATUS
        // - SET THE ASGN OUTPUT DAC VALUE TO THE MACHINE'S NOTE VALUE

        // filter CV
        filtered_cv_[i] += (adc_->cv(i) - filtered_cv_[i]) >> 6;

        // detect triggers on the reset input
        // TODO: see notes above regarding toggling this behavior
        // int16_t reset = adc_->reset(i);

        if (reset < kResetThresholdLow)
            reset_trigger_armed_[i] = true;

        if (reset > kResetThresholdHigh && reset_trigger_armed_[i]) {
            reset_trigger_armed_[i] = false;
            machines_[i].Step();
        }
        
        dac_->set_sine(i, (machines_[i].GetGateValue()) ? 32767 : -32767);  // TODO: dial in high/low values
        dac_->set_asgn(i, machines_[i].GetNoteValue() * 100);                // TODO: dial in value range for DAC
    }
}
} // namespace batumi
