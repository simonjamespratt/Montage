#include <iostream>
#include <juce_core/juce_core.h>
#include <juce_data_structures/juce_data_structures.h>
#include <string>

int main(int argc, char *argv[])
{
    static juce::Identifier myNodeType("MyNode"); // pre-create an Identifier
    juce::ValueTree myNode(
        myNodeType); // This is a valid node, of type "MyNode"
    static juce::Identifier propertyName("name");
    myNode.setProperty(propertyName, 123, nullptr);
    int name(myNode.getProperty(propertyName));
    // Your code goes here!
    std::cout << "Hello world!\n" << name;
    juce::ignoreUnused(argc, argv);

    return 0;
}
