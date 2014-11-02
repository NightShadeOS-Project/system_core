/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include "NativeBridgeTest.h"

#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

namespace android {

// Tests that the bridge initialization creates the code_cache if it doesn't
// exists.
TEST_F(NativeBridgeTest, CodeCacheCreate) {
    // Make sure that code_cache does not exists
    struct stat st;
    ASSERT_EQ(-1, stat(kCodeCache, &st));
    ASSERT_EQ(ENOENT, errno);

    // Init
    ASSERT_TRUE(LoadNativeBridge(kNativeBridgeLibrary, nullptr));
    ASSERT_TRUE(PreInitializeNativeBridge(".", "isa"));
    ASSERT_TRUE(InitializeNativeBridge(nullptr, nullptr));
    ASSERT_TRUE(NativeBridgeAvailable());
    ASSERT_FALSE(NativeBridgeError());

    // Check that code_cache was created
    ASSERT_EQ(0, stat(kCodeCache, &st));
    ASSERT_TRUE(S_ISDIR(st.st_mode));

    // Clean up
    UnloadNativeBridge();
    ASSERT_EQ(0, rmdir(kCodeCache));

    ASSERT_FALSE(NativeBridgeError());
}

}  // namespace android
