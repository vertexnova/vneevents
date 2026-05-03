# VneEvents — release-please and publish artifacts

## Flow

1. **release-please** (push to `main`) updates `CHANGELOG.md`, `VERSION`, and `.release-please-manifest.json`, and opens or updates the release PR.
2. After merge, a **tag** is created.
3. The **publish** job runs when `release_created == 'true'`, builds `cmake --install` trees per platform, and uploads `vneevents-v<version>-<platform>-<detail>.tar.gz`.

Configure **publish** with **`VNEEVENTS_LIB_TYPE`** (`shared` for desktop/web/android matrix paths, **`static`** for **iOS**). The nested **VneLogging** submodule uses the same library kind via cache (`VNE_LOGGING_LIB_TYPE` forced from the parent before `add_subdirectory`).

## Install layout

Tarballs include **`include/vertexnova/events/`** (with **`export.h`**), **`vertexnova/common/`** (bundled header-only **vnecommon**), **`lib/`** (`libvneevents` / `.dll` / `.dylib` / `.so`), **`LICENSE`**, **`CHANGELOG.md`**, and **`lib/cmake/VneEvents/`** (`VneEventsTargets.cmake`, **`FindVneEvents.cmake`**). **VneLogging** is linked at build time when embedded; its headers are **not** installed by VneEvents (`VNE_LOGGING_SKIP_INSTALL`) — use **`find_package(VneLogging)`** (or your dependency layout) for **`vertexnova/logging/`** includes.

## Artifact naming

Use **[`scripts/ci_artifact_detail.sh`](../../scripts/ci_artifact_detail.sh)** in workflows (see **`release-please.yml`**). For **Android**, set **`ANDROID_PLATFORM`** (e.g. `android-24`) and **`ANDROID_NDK_ROOT`** when invoking the script.

## iOS / Xcode probes

Use **`env -u MD_APPLE_SDK_ROOT -u SDKROOT -u IOS_SDK_VERSION`** with **`xcodebuild -version`** or rely on **`ci_artifact_detail.sh`** to avoid exit **134** / SIGABRT on some runners.
