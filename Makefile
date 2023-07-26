# Project Name
TARGET = vleerhond

USE_FATFS = 0

# Sources
CPP_SOURCES = \
	daisy_midi_io.cpp \
	polypaly.cpp \
	transient_detector.cpp \
	vleerhond.cpp \
	\
	vleerhond_lib/core/note_struct.cpp \
	vleerhond_lib/core/time_struct.cpp \
	\
	vleerhond_lib/harmony/harmony_struct.cpp \
	vleerhond_lib/harmony/chords.cpp \
	vleerhond_lib/harmony/chord_type.cpp \
	vleerhond_lib/harmony/scale.cpp \
	\
	vleerhond_lib/instruments/instrument_base.cpp \
	vleerhond_lib/instruments/instrument_rack.cpp \
	vleerhond_lib/instruments/minitaur/minitaur.cpp \
	vleerhond_lib/instruments/minitaur/moog_bass.cpp \
	vleerhond_lib/instruments/minitaur/moog_long_bass.cpp \
	vleerhond_lib/instruments/monopoly/monopoly.cpp \
	vleerhond_lib/instruments/tonal_instruments/acid_bass.cpp \
	vleerhond_lib/instruments/tonal_instruments/bass.cpp \
	vleerhond_lib/instruments/tonal_instruments/long_bass.cpp \
	vleerhond_lib/instruments/tonal_instruments/mono.cpp \
	vleerhond_lib/instruments/tonal_instruments/poly.cpp \
	\
	vleerhond_lib/midi/dummy_midi_channel.cpp \
	vleerhond_lib/midi/midi_channel.cpp \
	\
	vleerhond_lib/patterns/ab.cpp \
	vleerhond_lib/patterns/arp.cpp \
	vleerhond_lib/patterns/binary_pattern.cpp \
	vleerhond_lib/patterns/chord_pattern.cpp \
	vleerhond_lib/patterns/cv_patterns.cpp \
	vleerhond_lib/patterns/gate_patterns.cpp \
	vleerhond_lib/patterns/interval_pattern.cpp \
	vleerhond_lib/patterns/modulators.cpp \
	vleerhond_lib/patterns/pattern_utils.cpp \
	vleerhond_lib/patterns/sample_and_hold.cpp \
	\
	vleerhond_lib/utils/rand.cpp \
	vleerhond_lib/utils/utils.cpp

# Library Locations
LIBDAISY_DIR = ../libDaisy/
DAISYSP_DIR = ../DaisySP/

C_INCLUDES += \
	-I. \
	-Ivleerhond_lib \
	-Ivleerhond_lib/core \
	-Ivleerhond_lib/patterns \
	-Ivleerhond_lib/utils

# Core location, and generic Makefile.
SYSTEM_FILES_DIR = $(LIBDAISY_DIR)/core
include $(SYSTEM_FILES_DIR)/Makefile
