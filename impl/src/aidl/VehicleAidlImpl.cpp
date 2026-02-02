#define LOG_TAG "VehicleAidlImpl"
#include <log/log_main.h>

#include "VehicleAidlImpl.h"

#if __has_include(<aidl/android/hardware/automotive/vehicle/IVehicle.h>) && \
        __has_include(<android/binder_interface_utils.h>)

#include "VehicleTypeConvert.h"

namespace vendor::nlab::vehicle::aidl_impl {

using ::aidl::android::hardware::automotive::vehicle::StatusCode;
using ::aidl::android::hardware::automotive::vehicle::VehiclePropConfig;
using ::aidl::android::hardware::automotive::vehicle::VehiclePropValue;

VehicleAidlImpl::VehicleAidlImpl(vendor::nlab::vehicle::V1_0::impl::VehicleHalImpl* core,
                                 vendor::nlab::vehicle::V1_0::impl::VehicleHalClient* hwClient)
    : mCore(core), mHwClient(hwClient) {}

ndk::ScopedAStatus VehicleAidlImpl::getAllPropConfigs(std::vector<VehiclePropConfig>* _aidl_return) {
    if (_aidl_return == nullptr) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_NULL_POINTER);
    }
    if (mCore == nullptr) {
        return ndk::ScopedAStatus::fromServiceSpecificError(
                static_cast<int32_t>(StatusCode::INTERNAL_ERROR));
    }

    const auto hidlConfigs = mCore->listProperties();
    _aidl_return->clear();
    _aidl_return->reserve(hidlConfigs.size());
    for (const auto& c : hidlConfigs) {
        _aidl_return->push_back(vendor::nlab::vehicle::aidl_impl::toAidl(c));
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VehicleAidlImpl::get(const VehiclePropValue& in_requestedPropValue,
                                       VehiclePropValue* _aidl_return) {
    if (_aidl_return == nullptr) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_NULL_POINTER);
    }
    if (mCore == nullptr) {
        return ndk::ScopedAStatus::fromServiceSpecificError(
                static_cast<int32_t>(StatusCode::INTERNAL_ERROR));
    }

    vhal_v2_0::StatusCode hidlStatus = vhal_v2_0::StatusCode::OK;
    const auto hidlReq = vendor::nlab::vehicle::aidl_impl::toHidl(in_requestedPropValue);

    auto hidlVal = mCore->get(hidlReq, &hidlStatus);
    if (hidlVal == nullptr) {
        return ndk::ScopedAStatus::fromServiceSpecificError(
                static_cast<int32_t>(vendor::nlab::vehicle::aidl_impl::toAidl(hidlStatus)));
    }

    *_aidl_return = vendor::nlab::vehicle::aidl_impl::toAidl(*hidlVal);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VehicleAidlImpl::set(const VehiclePropValue& in_value) {
    if (mCore == nullptr) {
        return ndk::ScopedAStatus::fromServiceSpecificError(
                static_cast<int32_t>(StatusCode::INTERNAL_ERROR));
    }

    const auto hidlVal = vendor::nlab::vehicle::aidl_impl::toHidl(in_value);
    const auto status = mCore->set(hidlVal);
    if (status != vhal_v2_0::StatusCode::OK) {
        return ndk::ScopedAStatus::fromServiceSpecificError(
                static_cast<int32_t>(vendor::nlab::vehicle::aidl_impl::toAidl(status)));
    }
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VehicleAidlImpl::subscribe(
        const std::shared_ptr<aidl::android::hardware::automotive::vehicle::IVehicleCallback>& /*in_callback*/,
        const std::vector<aidl::android::hardware::automotive::vehicle::SubscribeOptions>& /*in_options*/) {
    // TODO(migration-demo): Implement callback fanout and reuse simulator + internal eventing.
    // For the scaffolding demo, we return OK to show service structure; production code must store
    // callbacks and emit onPropertyEvent/onPropertySetError.
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus VehicleAidlImpl::unsubscribe(
        const std::shared_ptr<aidl::android::hardware::automotive::vehicle::IVehicleCallback>& /*in_callback*/,
        int32_t /*in_propId*/) {
    // TODO(migration-demo): Implement callback removal and stop sample-rate timers as needed.
    return ndk::ScopedAStatus::ok();
}

} // namespace vendor::nlab::vehicle::aidl_impl

#endif
