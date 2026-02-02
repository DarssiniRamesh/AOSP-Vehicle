#define LOG_TAG "VehicleTypeConvert"
#include <log/log_main.h>

#include "VehicleTypeConvert.h"

#if __has_include(<aidl/android/hardware/automotive/vehicle/IVehicle.h>)

namespace vendor::nlab::vehicle::aidl_impl {

using ::aidl::android::hardware::automotive::vehicle::StatusCode;
using ::aidl::android::hardware::automotive::vehicle::VehiclePropConfig;
using ::aidl::android::hardware::automotive::vehicle::VehiclePropValue;

StatusCode toAidl(vhal_v2_0::StatusCode status) {
    // NOTE: Not all codes map 1:1 across releases; keep a conservative mapping.
    switch (status) {
        case vhal_v2_0::StatusCode::OK:
            return StatusCode::OK;
        case vhal_v2_0::StatusCode::INVALID_ARG:
            return StatusCode::INVALID_ARG;
        case vhal_v2_0::StatusCode::NOT_AVAILABLE:
            return StatusCode::NOT_AVAILABLE;
        case vhal_v2_0::StatusCode::ACCESS_DENIED:
            return StatusCode::ACCESS_DENIED;
        case vhal_v2_0::StatusCode::INTERNAL_ERROR:
            return StatusCode::INTERNAL_ERROR;
        default:
            return StatusCode::INTERNAL_ERROR;
    }
}

VehiclePropConfig toAidl(const vhal_v2_0::VehiclePropConfig& c) {
    VehiclePropConfig out{};

    // Common fields that exist across most versions.
    out.prop = c.prop;
    out.access = static_cast<int32_t>(c.access);
    out.changeMode = static_cast<int32_t>(c.changeMode);

    // Sampling rates.
    out.minSampleRate = c.minSampleRate;
    out.maxSampleRate = c.maxSampleRate;

    // Arrays: best-effort.
    out.configArray = c.configArray;
    out.configString = c.configString;

    // Area configs are more complex in AIDL; leaving as TODO since the parcelable differs by
    // Android release. The demo focuses on service scaffolding and build wiring.
    return out;
}

VehiclePropValue toAidl(const vhal_v2_0::VehiclePropValue& v) {
    VehiclePropValue out{};

    out.prop = v.prop;
    out.areaId = v.areaId;
    out.timestamp = v.timestamp;
    out.status = static_cast<int32_t>(v.status);

    // Raw value mapping (best-effort). Layout differs across versions; this is demo scaffolding.
    out.value.int32Values = v.value.int32Values;
    out.value.int64Values = v.value.int64Values;
    out.value.floatValues = v.value.floatValues;
    out.value.stringValue = v.value.stringValue;
    out.value.bytes = v.value.bytes;

    return out;
}

vhal_v2_0::VehiclePropValue toHidl(const VehiclePropValue& v) {
    vhal_v2_0::VehiclePropValue out{};

    out.prop = v.prop;
    out.areaId = v.areaId;
    out.timestamp = v.timestamp;
    out.status = static_cast<vhal_v2_0::VehiclePropertyStatus>(v.status);

    out.value.int32Values = v.value.int32Values;
    out.value.int64Values = v.value.int64Values;
    out.value.floatValues = v.value.floatValues;
    out.value.stringValue = v.value.stringValue;
    out.value.bytes = v.value.bytes;

    return out;
}

} // namespace vendor::nlab::vehicle::aidl_impl

#endif
