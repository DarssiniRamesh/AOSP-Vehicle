#define LOG_TAG "VehicleServiceAidl"
#include <log/log_main.h>

#include <memory>

#include "../VehicleHalImpl.h"
#include "../hw-clients/Simulator.h"

/**
 * AIDL VHAL demo entry point.
 *
 * This file is intentionally written as "scaffolding":
 * - When compiled inside a full AOSP tree (where the AIDL VHAL headers exist), it registers an
 *   IVehicle AIDL service instance via the binder service manager.
 * - When compiled outside AOSP, it builds a stub main() that logs guidance and exits.
 *
 * This lets this repository demonstrate the HIDL→AIDL migration structure without requiring a full
 * Android build environment to exist in CI.
 */

#if __has_include(<android/binder_manager.h>) && __has_include(<android/binder_process.h>) && \
        __has_include(<aidl/android/hardware/automotive/vehicle/IVehicle.h>)

#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "VehicleAidlImpl.h"

namespace {

constexpr const char* kAidlInstance = "default";

std::string makeServiceName() {
    // Conventional fully-qualified AIDL service name.
    return std::string() + aidl::android::hardware::automotive::vehicle::IVehicle::descriptor +
           "/" + kAidlInstance;
}

} // namespace

int main(int /*argc*/, char const* /*argv*/[]) {
    ALOGI("Starting AIDL VHAL demo service (nlab).");

    // Reuse the existing "internal" implementation and simulator so the demo isolates IPC changes.
    auto store = std::make_unique<vhal_v2_0::VehiclePropertyStore>();

    auto hwClient = std::make_unique<vendor::nlab::vehicle::V1_0::impl::Simulator>();
    hwClient->start();

    auto coreHal = std::make_unique<vendor::nlab::vehicle::V1_0::impl::VehicleHalImpl>(
            store.get(), hwClient.get());
    coreHal->onCreate();

    auto binderService = ndk::SharedRefBase::make<
            vendor::nlab::vehicle::aidl_impl::VehicleAidlImpl>(coreHal.get(), hwClient.get());

    const std::string serviceName = makeServiceName();
    ALOGI("Registering AIDL service as '%s' ...", serviceName.c_str());

    const binder_status_t status = AServiceManager_addService(
            binderService->asBinder().get(), serviceName.c_str());
    if (status != STATUS_OK) {
        ALOGE("Failed to register AIDL service (status=%d)", status);
        return 1;
    }

    ALOGI("AIDL VHAL demo service ready; joining binder threadpool.");
    ABinderProcess_setThreadPoolMaxThreadCount(4);
    ABinderProcess_joinThreadPool();
    return 0;
}

#else

int main(int /*argc*/, char const* /*argv*/[]) {
    ALOGW("AIDL VHAL demo service scaffold is present, but AOSP AIDL/binder headers were not found.");
    ALOGW("This is expected when building outside a full Android tree.");
    ALOGW("To use this for a real device build, compile in AOSP with the automotive vehicle AIDL HAL "
          "available and ensure the init rc + VINTF + SELinux wiring is applied.");
    return 0;
}

#endif
