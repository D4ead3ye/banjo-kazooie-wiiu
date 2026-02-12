#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <libultraship/libultraship.h>
#include <ship/resource/Resource.h>

extern "C" {
#include "include/model.h"
}

namespace Factories {

class Model : public Ship::Resource<BKModelBin> {
  public:
    using Resource::Resource;

    Model() : Resource(std::shared_ptr<Ship::ResourceInitData>()) {}

    BKModelBin* GetPointer();
    size_t GetPointerSize();

    std::unique_ptr<uint8_t[]> mModelData;
    size_t mModelDataSize = 0;
};
}
