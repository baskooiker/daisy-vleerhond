#include "patterns/cv_patterns.h"

// #include <algorithm>
// #include <iostream>

#include "utils/rand.h"

namespace Vleerhond {
void CvPattern::randomize(const uint8_t maximum, const uint8_t minimum) {
    for (size_t i = 0; i < STEPS_IN_BAR; i++) {
        this->pattern[i] = Rand::randui8(minimum, maximum);
    }
}

void CvPattern::set(const uint8_t i, const uint8_t value) {
    this->pattern[i] = value;
}

void CvPattern::setAll(const uint8_t value) {
    for (size_t i = 0; i < STEPS_IN_BAR; i++) this->pattern[i] = value;
}

uint8_t CvPattern::value(const uint8_t step) const {
    return this->pattern[step % STEPS_IN_BAR];
}

std::string CvPattern::toString(const int length) {
    // std::stringstream ss;
    // for (int16_t i = 0; i < length; i++) {
    //     ss << std::to_string(this->value(i)) << " ";
    // }
    // return ss.str();
    return "";
}

///////////////////////////////////////////////////////////////////

CvPattern16::CvPattern16() {
    this->length = STEPS_IN_BAR;
    this->time_division = TimeDivision::Sixteenth;
}

void CvPattern16::randomize(const uint8_t maximum, const uint8_t minimum) {
    this->pattern.randomize(maximum, minimum);
}

uint8_t CvPattern16::value(const TimeStruct& time) const {
    return this->value(time.getCount(this->time_division));
}

uint8_t CvPattern16::value(const uint8_t step) const {
    return this->pattern.value(step % MAX(MIN(this->length, 16), 1));
}

void CvPattern16::setAll(const uint8_t value) { this->pattern.setAll(value); }

void CvPattern16::set(const uint8_t index, const uint8_t value) {
    this->pattern.set(index, value);
}

//////////////////////////////////////////////////////////////////////////////////

CvPatternAB::CvPatternAB() {
    length = 16;
    time_division = TimeDivision::Sixteenth;
}

uint8_t CvPatternAB::value(const TimeStruct& time) const {
    uint8_t pat_length = MIN(this->length, 16);
    uint32_t count = time.getCount(this->time_division) %
                     (this->length <= 16 ? pat_length * 4 : 64);
    return this->value(count);
}

uint8_t CvPatternAB::value(const int index) const {
    uint8_t pat_length = MIN(this->length, 16);
    return this->patterns[this->abPattern.value(index / pat_length)].value(
        index % pat_length);
}

void CvPatternAB::setAll(const uint8_t value) {
    for (size_t i = 0; i < 3; i++) this->patterns[i].setAll(value);
}

void CvPatternAB::randomize(const uint8_t max, const uint8_t min) {
    for (size_t i = 0; i < 3; i++) {
        this->patterns[i].randomize(max, min);
    }
    this->abPattern.randomize();
}

std::string CvPatternAB::toString() {
    // std::stringstream ss;
    // for (size_t i = 0; i < 4; i++) {
    //     ss << this->patterns[this->abPattern.value(i)].toString(this->length)
    //        << "\n";
    // }
    // return ss.str();
    return "";
}

void CvPatternAB::set(const uint8_t index, const uint8_t value)
{
    uint8_t pat_length = MIN(this->length, 16);
    this->patterns[this->abPattern.value(index / pat_length)].set(index % pat_length, value);
}

void CvPatternAB::rollOut()
{
    CvPattern new_patterns[4];
    for (int i = 0; i < 4; i++)
    {
        new_patterns[i] = patterns[abPattern.value(i)];
    }
    for (int i = 0; i < 4; i++)
    {
        patterns[i] = new_patterns[i];
    }
    abPattern.setABCD();
}

//////////////////////////////////////////////////////

MorphingCvPattern::MorphingCvPattern()
{
    pattern.randomize();
    pattern.rollOut();
    randomizeTarget();
}
uint8_t MorphingCvPattern::value(const TimeStruct& time) const
{
    return pattern.value(time);
}
void MorphingCvPattern::randomizeTarget()
{
    target.randomize();
}
void MorphingCvPattern::morph()
{
    if (Rand::randf(1.0) < this->morph_probability)
    {
        std::vector<int> indices(64);
        for (int i = 0; i < 64; i++)
        {
            indices.push_back(i);
        }
        Vleerhond::Rand::shuffle(indices);
        for (const auto& i : indices)
        {
            auto target_value = this->target.value(i);
            auto pattern_value = this->pattern.value(i);
            if (target_value != pattern_value)
            {
                pattern.set(i, target_value);
                return;
            }
        }
        // If no single item was updated, pattern and target are equal.
        target.randomize();
        pattern.rollOut(); 
    }
}

std::string MorphingCvPattern::toString()
{
    return pattern.toString() + target.toString() + "\n";
}

bool MorphingCvPattern::isMorphed()
{
    for (int i = 0; i < 4 * pattern.length; i++)
    {
        if (target.value(i) != pattern.value(i))
        {
            return false;
        }
    }
    return true;
}

    const CvPatternAB MorphingCvPattern::getPattern()
    {
        return pattern;
    }
    const CvPatternAB MorphingCvPattern::getTarget()
    {
        return target;
    }
    void MorphingCvPattern::setLength(uint8_t length)
    {
        pattern.length = length;
        target.length = length;
    }
}  // namespace Vleerhond
