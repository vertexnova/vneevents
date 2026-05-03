#pragma once
/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 * ----------------------------------------------------------------------
 */

/**
 * @file export.h
 * @brief `VNEEVENTS_API` — DLL export/import for shared `vneevents` builds.
 *
 * On Windows: define `VNEEVENTS_BUILDING_DLL` when compiling the library;
 * define `VNEEVENTS_DLL` when linking the DLL from an application.
 */

#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
#if defined(VNEEVENTS_BUILDING_DLL)
#define VNEEVENTS_API __declspec(dllexport)
#elif defined(VNEEVENTS_DLL)
#define VNEEVENTS_API __declspec(dllimport)
#else
#define VNEEVENTS_API
#endif
#else
#if defined(VNEEVENTS_BUILDING_DLL) && (defined(__GNUC__) || defined(__clang__))
#define VNEEVENTS_API __attribute__((visibility("default")))
#else
#define VNEEVENTS_API
#endif
#endif
