/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SYSTEM_NATIVEPOWER_DAEMON_POWER_MANAGER_H_
#define SYSTEM_NATIVEPOWER_DAEMON_POWER_MANAGER_H_

#include <memory>

#include <base/macros.h>
#include <nativepower/BnPowerManager.h>

#include "wake_lock_manager.h"

namespace android {

class PowerManager : public BnPowerManager {
 public:
  PowerManager();
  ~PowerManager() override;

  // Must be called before Init().
  void set_wake_lock_manager_for_testing(
      std::unique_ptr<WakeLockManagerInterface> manager) {
    wake_lock_manager_ = std::move(manager);
  }

  // Initializes the object, returning true on success.
  bool Init();

  // BnPowerManager:
  status_t acquireWakeLock(int flags,
                           const sp<IBinder>& lock,
                           const String16& tag,
                           const String16& packageName,
                           bool isOneWay=false) override;
  status_t acquireWakeLockWithUid(int flags,
                                  const sp<IBinder>& lock,
                                  const String16& tag,
                                  const String16& packageName,
                                  int uid,
                                  bool isOneWay=false) override;
  status_t releaseWakeLock(const sp<IBinder>& lock,
                           int flags,
                           bool isOneWay=false) override;
  status_t updateWakeLockUids(const sp<IBinder>& lock,
                              int len,
                              const int* uids,
                              bool isOneWay=false) override;
  status_t powerHint(int hintId, int data) override;

 private:
  // Helper method for acquireWakeLock*(). Returns true on success.
  bool AddWakeLockRequest(const sp<IBinder>& lock,
                          const String16& tag,
                          const String16& packageName,
                          int uid);

  std::unique_ptr<WakeLockManagerInterface> wake_lock_manager_;

  DISALLOW_COPY_AND_ASSIGN(PowerManager);
};

}  // namespace android

#endif  // SYSTEM_NATIVEPOWER_DAEMON_POWER_MANAGER_H_