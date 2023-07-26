#include "midi/midi_io.h"

#include "core/defs.h"

namespace Vleerhond {
std::vector<ofxMidiIn> MidiIO::in_ports;
std::map<std::string, MidiOut> MidiIO::out_ports;

bool nameMatches(const std::string& port_name, const std::string& target_name) {
    return port_name.find(target_name) != std::string::npos;
}

std::vector<MidiOut> MidiIO::getOutPorts(const std::string& port_name) {
    std::vector<MidiOut> ports;
    std::transform(
        out_ports.begin(), out_ports.end(), std::back_inserter(ports),
        [](const std::map<std::string, MidiOut>::value_type& val) {
            return val.second;
        });
    if (port_name.length() > 0) {
        ports = {out_ports[port_name]};
    }
    return ports;
}

bool MidiIO::setMainInput(
    const std::string& target_port_name, ofxMidiListener* listener) {
    //        in_ports.push_back(ofxMidiIn("ofxMidiInClient",
    //        MIDI_API_DEFAULT));
    in_ports.push_back(ofxMidiIn("ofxMidiInClient", MIDI_API_JACK));

    ofxMidiIn& in = in_ports.back();

    // ofLogNotice("MIDI", "finding port name: %s", target_port_name.c_str());
    std::vector<std::string> port_names = in.getInPortList();
    // for (const std::string& name : port_names) {
    //     ofLogNotice("MIDI", "In port: %s", name.c_str());
    // }

    for (size_t i = 0; i < port_names.size(); i++) {
        if (nameMatches(port_names[i], target_port_name)) {
            if (in.openPort(i)) {
                // don't ignore sysex, timing, & active sense messages,
                // these are ignored by default
                in.ignoreTypes(false, false, false);
                in.setVerbose(true);
                in.addListener(listener);
                // ofLogNotice(
                //     "", "input succesfully initialized: %s",
                //     in.getInPortName(i).c_str());
                return true;
            }
        }
    }
    // ofLogNotice("input not initialized!");
    return false;
}

bool MidiIO::setOutput(const std::string& target_port_name) {
    out_ports.insert(
        std::make_pair(target_port_name, MidiOut(1)));

    MidiOut& pair = out_ports[target_port_name];
    ofxMidiOut& out = pair.port;

    for (const std::string name : out.getOutPortList()) {
        ofLogNotice("MIDI", "Out port: %s", name.c_str());
    }

    for (int16_t i = 0; i < out.getNumOutPorts(); i++) {
        if (nameMatches(out.getOutPortName(i), target_port_name)) {
            if (out.openPort(i)) {
                return true;
            }
        }
    }
    return false;
}

bool MidiIO::portAvailable(const std::string& target_port_name) {
    ofxMidiIn in("ofxMidiInClient", MIDI_API_JACK);

    bool return_value = false;
    for (const std::string& name : in.getInPortList()) {
        ofLogNotice("MIDI", "InPort: %s", name.c_str());
        if (nameMatches(name, target_port_name)) {
            return_value = true;
        }
    }
    return return_value;
}

bool MidiIO::portsOpen() {
    for (auto& in : in_ports) {
        if (!in.isOpen()) {
            return false;
        }
    }
    for (auto& out : out_ports) {
        if (!out.second.port.isOpen()) {
            return false;
        }
    }

    return true;
}

void MidiIO::closeAll() {
    for (auto& in : in_ports) {
        in.closePort();
    }
    for (auto& out : out_ports) {
        out.second.port.closePort();
    }
}

void MidiIO::sendTimeClock() {
    for (auto& out : out_ports) {
        for (int16_t i = 0; i < out.second.time_multiplier; i++) {
            out.second.port.sendMidiByte(0xF8);
        }
    }
}

void MidiIO::sendStart() {
    for (auto& out : out_ports) {
        out.second.port.sendMidiByte(0xFA);
    }
}

void MidiIO::sendContinue() {
    for (auto& out : out_ports) {
        out.second.port.sendMidiByte(0xFB);
    }
}

void MidiIO::sendStop() {
    for (auto& out : out_ports) {
        out.second.port.sendMidiByte(0xFC);
    }
}

void MidiIO::sendNoteOn(
    const uint8_t pitch, const uint8_t velocity, const uint8_t channel,
    const std::string& port_name) {
    //        ofLogNotice("MIDI", "sendNoteOn(pitch: %d, velocity: %d, channel:
    //        %d)", pitch, velocity, channel);
    for (auto& out : getOutPorts(port_name)) {
        out.port.sendNoteOn(channel, pitch, velocity);
    }
}

void MidiIO::sendNoteOff(
    const uint8_t pitch, const uint8_t channel, const std::string& port_name) {
    for (auto& out : getOutPorts(port_name)) {
        out.port.sendNoteOff(channel, pitch);
    }
}

void MidiIO::sendCc(
    uint8_t cc, uint8_t value, uint8_t channel, const std::string& port_name) {
    for (auto& out : getOutPorts(port_name)) {
        out.port.sendControlChange(channel, cc, value);
    }
}

void MidiIO::sendBytes(
    std::vector<uint8_t>& bytes, const std::string& port_name) {
    for (auto& out : getOutPorts(port_name)) {
        out.port.sendMidiBytes(bytes);
    }
}

void MidiIO::sendProgramChange(
    const uint8_t channel, const uint8_t program,
    const std::string& port_name) {
    for (auto& out : getOutPorts(port_name)) {
        out.port.sendProgramChange(channel, program);
    }
}

inline MidiOut::MidiOut(int time_multiplier)
    //        : port("ofxMidiOut Client", MIDI_API_DEFAULT)
    : port("ofxMidiOut Client", MIDI_API_JACK),
      time_multiplier(time_multiplier) {}

}  // namespace Vleerhond
