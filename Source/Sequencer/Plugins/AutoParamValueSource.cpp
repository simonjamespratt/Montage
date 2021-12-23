#include "AutoParamValueSource.h"

AutoParamValueSource::AutoParamValueSource(te::AutomatableParameter::Ptr p)
: param(p)
{
    param->addListener(this);
}

AutoParamValueSource::~AutoParamValueSource()
{
    param->removeListener(this);
}

juce::var AutoParamValueSource::getValue() const
{
    return param->getCurrentValue();
}

void AutoParamValueSource::setValue(const juce::var &newValue)
{
    param->setParameter(static_cast<float>(newValue), juce::sendNotification);
}

void AutoParamValueSource::bindSliderToParameter(juce::Slider &s,
                                                 te::AutomatableParameter &p)
{
    const auto v = p.valueRange;
    const auto range =
        juce::NormalisableRange<double>(static_cast<double>(v.start),
                                        static_cast<double>(v.end),
                                        static_cast<double>(v.interval),
                                        static_cast<double>(v.skew),
                                        v.symmetricSkew);
    s.setNormalisableRange(range);

    // NB: use of new keyword here is ok as AutoParamValueSource inherits from
    // ValueSource which in turn inherits from ReferenceCountedObject. See
    // https://docs.juce.com/master/classValue_1_1ValueSource.html and
    // https://docs.juce.com/master/classReferenceCountedObject.html#details
    s.getValueObject().referTo(juce::Value(new AutoParamValueSource(p)));
}

// Private methods
void AutoParamValueSource::curveHasChanged(te::AutomatableParameter &)
{
    sendChangeMessage(false);
}

void AutoParamValueSource::currentValueChanged(te::AutomatableParameter &,
                                               float /*newValue*/)
{
    sendChangeMessage(false);
}
