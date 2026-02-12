#include "Model.h"
#include <cstring>
#include "spdlog/spdlog.h"

namespace Factories {

BKModelBin* Model::GetPointer() {
    return reinterpret_cast<BKModelBin*>(mModelData.get());
}

size_t Model::GetPointerSize() {
    return mModelDataSize;
}

}
