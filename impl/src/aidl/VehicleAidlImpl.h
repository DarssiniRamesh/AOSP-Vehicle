#pragma once

#include <memory>

#include "../VehicleHalClient.h"
#include "../VehicleHalImpl.h"

/**
 * AIDL binder implementation scaffolding for the Vehicle HAL.
 *
 * IMPORTANT:
 * - The exact IVehicle method surface varies across Android releases.
 * - This file is intentionally guarded by __has_include checks so that the repo can build outside a
 *   full AOSP tree while still providing the "real" implementation shape for AOSP integration.
 */

#if __has_include(<aidl/android/hardware/automotive/vehicle/IVehicle.h>) && \
        __has_include(<android/binder_interface_utils.h>)

#include <aidl/android/hardware/automotive/vehicle/IVehicle.h>
#include <android/binder_interface_utils.h>

namespace vendor::nlab::vehicle::aidl_impl {

/**
 * Implements the AIDL IVehicle service by delegating to the existing internal V2_0-style core.
 *
 * This preserves simulator + property store behavior while migrating only the IPC boundary.
 */
class VehicleAidlImpl final : public aidl::android::hardware::automotive::vehicle::BnVehicle {
public:
    VehicleAidlImpl(vendor::nlab::vehicle::V1_0::impl::VehicleHalImpl* core,
                    vendor::nlab::vehicle::V1_0::impl::VehicleHalClient* hwClient);
    ~VehicleAidlImpl() override = default;

    // NOTE: We intentionally keep implementations minimal and documented. In a real migration,
    // these methods would convert types and call into core logic.
    ndk::ScopedAStatus getAllPropConfigs(
            std::vector<aidl::android::hardware::automotive::vehicle::VehiclePropConfig>* _aidl_return)
            override;

    ndk::ScopedAStatus get(
            const aidl::android::hardware::automotive::vehicle::VehiclePropValue& in_requestedPropValue,
            aidl::android::hardware::automotive::vehicle::VehiclePropValue* _aidl_return) override;

    ndk::ScopedAStatus set(
            const aidl::android::hardware::automotive::vehicle::VehiclePropValue& in_value) override;

    ndk::ScopedAStatus subscribe(
            const std::shared_ptr<aidl::android::hardware::automotive::vehicle::IVehicleCallback>& in_callback,
            const std::vector<aidl::android::hardware::automotive::vehicle::SubscribeOptions>& in_options)
            override;

    ndk::ScopedAStatus unsubscribe(
            const std::shared_ptr<aidl::android::hardware::automotive::vehicle::IVehicleCallback>& in_callback,
            int32_t in_propId) override;

private:
    vendor::nlab::vehicle::V1_0::impl::VehicleHalImpl* mCore;
    vendor::nlab::vehicle::V1_0::impl::VehicleHalClient* mHwClient;
};

} // namespace vendor::nlab::vehicle::aidl_impl

#endif
