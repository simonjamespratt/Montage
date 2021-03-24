#include "DurationProtocolController.h"

#include "GeometricProtocolController.h"
#include "MultiplesProtocolController.h"
#include "PrescribedProtocolController.h"

#include <stdexcept>

std::unique_ptr<DurationProtocolController> DurationProtocolController::create(
    Type type,
    DurationProtocolParams &params,
    std::shared_ptr<aleatoric::DurationsProducer> producer)
{
    switch(type) {
    case Type::geometric:
        params.activeType = DurationProtocolController::Type::geometric;
        return std::make_unique<GeometricProtocolController>(params, producer);
        break;
    case Type::multiples:
        params.activeType = DurationProtocolController::Type::multiples;
        return std::make_unique<MultiplesProtocolController>(params, producer);
        break;
    case Type::prescribed:
        params.activeType = DurationProtocolController::Type::prescribed;
        return std::make_unique<PrescribedProtocolController>(params, producer);
        break;

    default:
        throw std::invalid_argument("Type not recognised");
    }
}
