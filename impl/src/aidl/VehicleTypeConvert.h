#pragma once

#include "../VehicleHalImpl.h"

#if __has_include(<aidl/android/hardware/automotive/vehicle/IVehicle.h>)

#include <aidl/android/hardware/automotive/vehicle/StatusCode.h>
#include <aidl/android/hardware/automotive/vehicle/VehiclePropConfig.h>
#include <aidl/android/hardware/automotive/vehicle/VehiclePropValue.h>

namespace vendor::nlab::vehicle::aidl_impl {

/**
 * Convert internal StatusCode to AIDL StatusCode.
 */
aidl::android::hardware::automotive::vehicle::StatusCode toAidl(vhal_v2_0::StatusCode status);

/**
 * Convert internal VehiclePropConfig to AIDL VehiclePropConfig.
 */
aidl::android::hardware::automotive::vehicle::VehiclePropConfig toAidl(
        const vhal_v2_0::VehiclePropConfig& c);

/**
 * Convert internal VehiclePropValue to AIDL VehiclePropValue.
 */
aidl::android::hardware::automotive::vehicle::VehiclePropValue toAidl(
        const vhal_v2_0::VehiclePropValue& v);

/**
 * Convert AIDL VehiclePropValue to internal VehiclePropValue.
 */
vhal_v2_0::VehiclePropValue toHidl(
        const aidl::android::hardware::automotive::vehicle::VehiclePropValue& v);

} // namespace vendor::nlab::vehicle::aidl_impl

#endif
