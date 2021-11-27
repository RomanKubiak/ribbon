#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S audioInput;           //xy=60,50
AudioPlaySdWav playSdWav1;     //xy=101,382
AudioMixer4 mixerAudioCardR;         //xy=192.00001525878906,639.9999847412109
AudioSynthWaveform waveModMultiply;      //xy=267.0000228881836,567.9999828338623
AudioAmplifier amp2;           //xy=283.0000114440918,193.0000057220459
AudioAmplifier amp3;           //xy=284.0000114440918,232.0000057220459
AudioMixer4 mixerAudioCardL;         //xy=333,78
AudioSynthWaveform waveform2;      //xy=336.0056686401367,288.0055980682373
AudioSynthWaveformDc dc1;            //xy=337.0966033935547,377.00566005706787
AudioAnalyzeNoteFrequency notefreq1;      //xy=405.0000114440918,192.0000057220459
AudioAnalyzeNoteFrequency notefreq2;      //xy=406.0000114440918,233.0000057220459
AudioEffectMultiply multiply;      //xy=411.0000915527344,494.00000858306885
AudioEffectEnvelope envelope1;      //xy=468.00569915771484,379.0056619644165
AudioMixer4 filterInputMixer;         //xy=481.91477966308594,296.00566482543945
AudioEffectFreeverb freeverb2;      //xy=568.000114440918,452.0000286102295
AudioEffectGranular granular1;      //xy=575.0000267028809,157.0000057220459
AudioMixer4 mixerR;         //xy=582.9999961853027,572.9999961853027
AudioAmplifier feedbackAmp;           //xy=609.3693141937256,221.0056562423706
AudioFilterStateVariable filter1;        //xy=640.0056495666504,307.0056095123291
AudioAmplifier ampL;           //xy=738.9999656677246,159.99998474121094
AudioAmplifier ampR;           //xy=760.9999542236328,570.0000305175781
AudioMixer4 outMixer;         //xy=916.8238677978516,291.00565910339355
AudioOutputI2S audioOutput;           //xy=1111.9999237060547,299.99999809265137
AudioConnection patchCord1(audioInput, 0, amp2, 0);
AudioConnection patchCord2(audioInput, 0, mixerAudioCardL, 0);
AudioConnection patchCord3(audioInput, 1, amp3, 0);
AudioConnection patchCord4(audioInput, 1, mixerAudioCardR, 0);
AudioConnection patchCord5(playSdWav1, 0, mixerAudioCardL, 1);
AudioConnection patchCord6(playSdWav1, 1, mixerAudioCardR, 1);
AudioConnection patchCord7(mixerAudioCardR, 0, multiply, 0);
AudioConnection patchCord8(mixerAudioCardR, 0, mixerR, 2);
AudioConnection patchCord9(waveModMultiply, 0, multiply, 1);
AudioConnection patchCord10(amp2, notefreq1);
AudioConnection patchCord11(amp3, notefreq2);
AudioConnection patchCord12(mixerAudioCardL, granular1);
AudioConnection patchCord13(waveform2, 0, filterInputMixer, 0);
AudioConnection patchCord14(dc1, envelope1);
AudioConnection patchCord15(multiply, 0, mixerR, 0);
AudioConnection patchCord16(multiply, freeverb2);
AudioConnection patchCord17(envelope1, 0, filter1, 1);
AudioConnection patchCord18(filterInputMixer, 0, filter1, 0);
AudioConnection patchCord19(freeverb2, 0, mixerR, 1);
AudioConnection patchCord20(granular1, ampL);
AudioConnection patchCord21(mixerR, ampR);
AudioConnection patchCord22(feedbackAmp, 0, filterInputMixer, 1);
AudioConnection patchCord23(filter1, 0, outMixer, 2);
AudioConnection patchCord24(filter1, 0, feedbackAmp, 0);
AudioConnection patchCord25(filter1, 1, outMixer, 3);
AudioConnection patchCord26(ampL, 0, outMixer, 0);
AudioConnection patchCord27(ampR, 0, outMixer, 1);
AudioConnection patchCord28(outMixer, 0, audioOutput, 0);
AudioConnection patchCord29(outMixer, 0, audioOutput, 1);
AudioControlSGTL5000 sgtl5000_1;     //xy=600.9999771118164,50.999996185302734
// GUItool: end automatically generated code

#include <WS2812Serial.h>
#include <Encoder.h>
#include <ResponsiveAnalogRead.h>
#include <MIDI.h>

#include "../lib/Adafruit-GFX-Library/Adafruit_GrayOLED.h"
#include "../lib/Adafruit-GFX-Library/Adafruit_GFX.h"
#include "../lib/Adafruit_SH110x/Adafruit_SH110X.h"
#include "../lib/Adafruit_MCP4725/Adafruit_MCP4725.h"
#include "Manager.h"
#include "SimpleSynth.h"

void playFile(const char *filename);

byte drawingMemory[NUM_LEDS * 3];         //  3 bytes per LED
DMAMEM byte displayMemory[NUM_LEDS * 12]; // 12 bytes per LED
WS2812Serial leds(NUM_LEDS, displayMemory, drawingMemory, LED_PIN, WS2812_GRB);

Encoder encoder(ENCODER_A, ENCODER_B);
Adafruit_SH1107 display = Adafruit_SH1107(128, 128, &Wire);
Adafruit_MCP4725 dac;

ribbon::Manager manager(display, encoder, leds);
ribbon::SimpleSynth simpleSynth(waveform2,
                                filter1,
                                envelope1,
                                outMixer,
                                feedbackAmp,
                                filterInputMixer,
                                dc1
                                );

void setup() {
    Serial.begin(115200);

    AudioMemory(40);
    display.begin(0x3C, true); // Address 0x3C default
    display.clearDisplay();
    display.setRotation(0);
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.print("I love rubber");
    display.display(); // actually display all of the above*/

    //dac.begin(0x60);

    leds.begin();
    leds.clear();
    leds.show();

    sgtl5000_1.enable();
    sgtl5000_1.volume(0.7);

    SPI.setMOSI(SDCARD_MOSI_PIN);
    SPI.setSCK(SDCARD_SCK_PIN);
    if (!(SD.begin(SDCARD_CS_PIN))) {
        // stop here, but print a message repetitively
        while (1) {
            Serial.println("Unable to access the SD card");
            delay(500);
        }
    }
    AudioNoInterrupts();
    mixerR.gain(0, 0.0);
    mixerR.gain(1, 0.0);
    mixerR.gain(2, 1.0);
    mixerR.gain(3, 0.0);

    mixerAudioCardL.gain(1, 1.0);
    mixerAudioCardR.gain(1, 1.0);

    waveModMultiply.begin(1.0, 440, WAVEFORM_SINE);

    amp2.gain(0.0f);
    amp3.gain(0.0f);

    outMixer.gain(0, 0.0);
    outMixer.gain(1, 0.0);
    outMixer.gain(2, 0.5);
    outMixer.gain(3, 0.0);

    dc1.amplitude(1.0);
    simpleSynth.begin();
    AudioInterrupts();

    leds.clear();
    leds.show();

    manager.begin();
}

elapsedMillis ledPiano;

void loop() {
    if (!playSdWav1.isPlaying()) {
        playFile("ORIGINAL.WAV");
    }
    manager.update();
}

void playFile(const char *filename) {
    printf("playing %s\r\n", filename);
    playSdWav1.play(filename);
    delay(25);
}

