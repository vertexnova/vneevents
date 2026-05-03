/* ---------------------------------------------------------------------
 * Copyright (c) 2026 Ajeet Singh Yadav. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License")
 *
 * Forces key symbols into the vneevents shared library on Windows (MSVC).
 *
 * Most event types are implemented inline in headers; without at least one .cpp
 * in the DLL that sees those types with dllexport (VNEEVENTS_BUILDING_DLL),
 * vtables and outlined copies are never emitted into the DLL. Consumers then
 * link with dllimport and fail with LNK2019 on Event::type(), destructors, etc.
 * ----------------------------------------------------------------------
 */

#include "vertexnova/events/events.h"
