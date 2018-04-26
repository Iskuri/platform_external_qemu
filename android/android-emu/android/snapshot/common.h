// Copyright 2017 The Android Open Source Project
//
// This software is licensed under the terms of the GNU General Public
// License version 2, as published by the Free Software Foundation, and
// may be copied, distributed, and modified under those terms.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

#pragma once

#include "android/base/StringView.h"
#include "android/emulation/control/vm_operations.h"
#include "android/snapshot/interface.h"

#include <memory>
#include <stdint.h>

namespace android {
namespace snapshot {

class ITextureSaver;
class TextureSaver;
class ITextureLoader;
class TextureLoader;
using ITextureSaverPtr = std::shared_ptr<ITextureSaver>;
using ITextureLoaderPtr = std::shared_ptr<ITextureLoader>;
using ITextureLoaderWPtr = std::weak_ptr<ITextureLoader>;

using RamBlock = ::SnapshotRamBlock;

enum class IndexFlags {
    Empty = 0,
    CompressedPages = 0x01,
};

enum class OperationStatus {
    NotStarted = SNAPSHOT_STATUS_NOT_STARTED,
    Ok = SNAPSHOT_STATUS_OK,
    Error = SNAPSHOT_STATUS_ERROR,
    Canceled = SNAPSHOT_STATUS_CANCELED,
};

enum class FailureReason {
    Empty = 0,

    CorruptedData,
    NoSnapshotPb,
    BadSnapshotPb,
    IncompatibleVersion,
    NoRamFile,
    NoTexturesFile,
    SnapshotsNotSupported,
    Canceled,

    UnrecoverableErrorLimit = 10000,

    NoSnapshotInImage,
    ConfigMismatchHostHypervisor,
    ConfigMismatchHostGpu,
    ConfigMismatchRenderer,
    ConfigMismatchFeatures,
    ConfigMismatchAvd,
    SystemImageChanged,

    ValidationErrorLimit = 20000,

    InternalError,
    EmulationEngineFailed,
    RamFailed,
    TexturesFailed,
    AdbOffline,
    OutOfDiskSpace,
    Tombstone,

    InProgressLimit = 30000,
};

FailureReason errnoToFailure(int error);
const char* failureReasonToString(FailureReason failure,
                                  SnapshotOperation op);

template <class Operation>
bool isComplete(const Operation& op) {
    return op.status() != OperationStatus::NotStarted;
}

bool isBufferZeroed(const void* ptr, int32_t size);

constexpr int32_t kDefaultPageSize = 4096;

constexpr int32_t kCancelTimeoutMs = 15000;

// Size in bytes of largest in-flight RAM region for decommitting.
constexpr uint64_t kDecommitChunkSize = 4096 * 4096; // 16 MB
constexpr base::StringView kDefaultBootSnapshot = "default_boot";
constexpr base::StringView kRamFileName = "ram.bin";
constexpr base::StringView kTexturesFileName = "textures.bin";

void resetSnapshotLiveness();
bool isSnapshotAlive();

}  // namespace snapshot
}  // namespace android
