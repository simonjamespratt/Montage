// NB: This only includes conversions for generic types. Conversions for very
// specialised types (e.g. enums) are defined at the point of use.

#pragma once

#include <juce_data_structures/juce_data_structures.h>

template<>
struct juce::VariantConverter<juce::Array<int>> {
    static juce::Array<int> fromVar(const juce::var &v)
    {
        juce::Array<int> arr;
        for(auto &i : *v.getArray()) {
            arr.add(i);
        }

        return arr;
    };

    // NB: when this converter is used for a juce::CachedValue, note that the
    // value of the juce::CachedValue (and the underlying value tree) can only
    // be set using ASSIGNMENT. Methods of juce::Array such as ::add() do not
    // change the underlying value of the juce::ValueTree
    static juce::var toVar(const juce::Array<int> &t)
    {
        juce::var arr;
        for(auto &i : t) {
            arr.append(i);
        }

        return arr;
    };
};

template<>
struct juce::VariantConverter<std::vector<int>> {
    static std::vector<int> fromVar(const juce::var &v)
    {
        std::vector<int> arr;
        for(auto &i : *v.getArray()) {
            arr.emplace_back(i);
        }

        return arr;
    };

    // NB: when this converter is used for a juce::CachedValue, note that the
    // value of the juce::CachedValue (and the underlying value tree) can only
    // be set using ASSIGNMENT. Methods of the vector do not change the
    // underlying value of the juce::ValueTree
    static juce::var toVar(const std::vector<int> &t)
    {
        juce::var arr;
        for(auto &i : t) {
            arr.append(i);
        }

        return arr;
    };
};

template<>
struct juce::VariantConverter<std::vector<double>> {
    static std::vector<double> fromVar(const juce::var &v)
    {
        std::vector<double> arr;
        for(auto &i : *v.getArray()) {
            arr.emplace_back(i);
        }

        return arr;
    };

    // NB: when this converter is used for a juce::CachedValue, note that the
    // value of the juce::CachedValue (and the underlying value tree) can only
    // be set using ASSIGNMENT. Methods of the vector do not change the
    // underlying value of the juce::ValueTree
    static juce::var toVar(const std::vector<double> &t)
    {
        juce::var arr;
        for(auto &i : t) {
            arr.append(i);
        }

        return arr;
    };
};
