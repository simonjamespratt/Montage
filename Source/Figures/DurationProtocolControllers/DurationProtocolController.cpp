#include "DurationProtocolController.h"

#include "GeometricProtocolController.h"
#include "MultiplesProtocolController.h"
#include "PrescribedProtocolController.h"

#include <stdexcept>

std::unique_ptr<DurationProtocolController>
DurationProtocolController::create(DurationProtocolType type,
                                   DurationProtocolParams &params)
{
    params.activeType = type;

    switch(type) {
    case DurationProtocolType::geometric:
        return std::make_unique<GeometricProtocolController>(params);
        break;
    case DurationProtocolType::multiples:
        return std::make_unique<MultiplesProtocolController>(params);
        break;
    case DurationProtocolType::prescribed:
        return std::make_unique<PrescribedProtocolController>(params);
        break;

    default:
        throw std::invalid_argument("Type not recognised");
    }
}

std::unique_ptr<aleatoric::DurationProtocol>
DurationProtocolController::createProtocol(DurationProtocolParams &params)
{
    switch(params.activeType) {
    case DurationProtocolType::geometric: {
        auto &geoParams = params.geometric;
        return aleatoric::DurationProtocol::createGeometric(
            aleatoric::Range(geoParams.rangeStart, geoParams.rangeEnd),
            geoParams.collectionSize);
    } break;
    case DurationProtocolType::multiples: {
        auto &multiParams = params.multiples;
        if(multiParams.strategy == MultiplierStrategy::range) {
            return aleatoric::DurationProtocol::createMultiples(
                multiParams.baseIncrement,
                aleatoric::Range(multiParams.rangeStart, multiParams.rangeEnd),
                multiParams.deviationFactor);
        } else if(multiParams.strategy == MultiplierStrategy::hand) {
            return aleatoric::DurationProtocol::createMultiples(
                multiParams.baseIncrement,
                multiParams.multipliers,
                multiParams.deviationFactor);
        } else {
            throw std::invalid_argument("Multiplier strategy for protocol type "
                                        "Multiples not recognised");
        }

    } break;
    case DurationProtocolType::prescribed: {
        return aleatoric::DurationProtocol::createPrescribed(
            params.prescribed.durations);
    } break;
    default:
        throw std::invalid_argument("Protocol type not recognised");
    }
}
