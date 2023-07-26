#pragma once

#include <map>
#include <vector>
#include <string>

// #include "ofxMidiIn.h"
// #include "ofxMidiOut.h"

struct AbstractMidiOut {
    void sendMidiByte(uint8_t b);
};

struct AbstractMidiIn {

};

struct AbstractMidiListener {

};

namespace Vleerhond {

class MidiOut {
   public:
    AbstractMidiOut port;
    int time_multiplier = 1;

    MidiOut(int time_multiplier = 1);
};

class MidiIO {
   private:
    static std::vector<AbstractMidiIn> in_ports;
    static std::map<std::string, MidiOut> out_ports;

    static std::vector<MidiOut> getOutPorts(const std::string& port_name);

   public:
    // static bool setMainInput(const std::string& port_name, AbstractMidiListener* listener);
    static void setOutput(AbstractMidiOut& out);

    static bool portsOpen();
    static void closeAll();

    static void sendTimeClock();
    static void sendStart();
    static void sendContinue();
    static void sendStop();

    static void sendNoteOn(const uint8_t pitch, const uint8_t velocity, const uint8_t channel);
    static void sendNoteOff(const uint8_t pitch, const uint8_t channel);
    static void sendCc(const uint8_t cc, const uint8_t value, const uint8_t channel);
    static void sendBytes(std::vector<uint8_t>& bytes);
    static void sendProgramChange(const uint8_t channel, const uint8_t program);
};
}  // namespace Vleerhond
