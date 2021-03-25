#pragma once
#include "ErrorTypes.h"

#include <algorithm>
#include <iterator>
#include <juce_data_structures/juce_data_structures.h>

template<typename ObjectType>
class ObjectList : public juce::ValueTree::Listener {
  public:
    ObjectList(const juce::ValueTree &vt) : state(vt)
    {
        state.addListener(this);
    }

    ObjectList(const ObjectList &objList) : state(objList.state)
    {
        state.addListener(this);
    }

    ~ObjectList()
    {
        state.removeListener(this);
    }

    std::vector<ObjectType> getObjects() const
    {
        std::vector<ObjectType> objects;

        for(const auto &child : state) {
            if(isSuitableType(child)) {
                objects.emplace_back(makeObject(child));
            }
        }

        return objects;
    }

    int getIndex(const ObjectType &object) const
    {
        auto objects = getObjects();

        auto it = std::find_if(objects.begin(),
                               objects.end(),
                               [&object](const ObjectType &o) {
                                   return object.getId() == o.getId();
                               });

        if(it == objects.end()) {
            return -1;
        }

        return std::distance(objects.begin(), it);
    }

    ObjectType addObject(const ObjectType &object)
    {
        auto objectState = getObjectState(object);

        if(objectState.isAChildOf(state)) {
            throw ObjectAlreadyExists();
        }

        validateObjectAddition(object);

        state.appendChild(objectState, nullptr);

        return object;
    }

    ObjectType removeObject(const ObjectType &object)
    {
        auto objectState = getObjectState(object);

        if(!objectState.isAChildOf(state)) {
            throw ObjectNotFound();
        }

        validateObjectRemoval(object);

        state.removeChild(objectState, nullptr);

        return object;
    }

    std::function<void(ObjectType)> onObjectAdded;
    std::function<void(ObjectType)> onObjectRemoved;
    std::function<void(ObjectType object, juce::Identifier property)>
        onObjectUpdated;

    void valueTreeChildAdded(juce::ValueTree &parent,
                             juce::ValueTree &child) override
    {
        if(parent == state && isSuitableType(child)) {
            if(onObjectAdded) {
                onObjectAdded(makeObject(child));
            }
        }
    }

    void valueTreeChildRemoved(juce::ValueTree &parent,
                               juce::ValueTree &child,
                               int) override
    {
        if(parent == state && isSuitableType(child)) {
            if(onObjectRemoved) {
                onObjectRemoved(makeObject(child));
            }
        }
    }

    void valueTreePropertyChanged(juce::ValueTree &vt,
                                  const juce::Identifier &property) override
    {
        if(vt.getParent() == state && isSuitableType(vt)) {
            if(onObjectUpdated) {
                onObjectUpdated(makeObject(vt), property);
            }
        }
    }

  protected:
    juce::ValueTree state;

  private:
    virtual bool isSuitableType(const juce::ValueTree &) const = 0;
    virtual ObjectType makeObject(const juce::ValueTree &) const = 0;
    virtual juce::ValueTree getObjectState(const ObjectType &) const = 0;
    virtual void validateObjectRemoval(const ObjectType &)
    {}
    virtual void validateObjectAddition(const ObjectType &)
    {}
};
