/**
 * Panoptes - A Binary Translation Framework for CUDA
 * (c) 2011-2012 Chris Kennelly <chris@ckennelly.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cuda.h>
#include <gtest/gtest.h>
#include <stdint.h>

texture<int32_t, 1, cudaReadModeElementType> tex_src;

TEST(BindTextureToArray, NullArguments) {
    ::testing::FLAGS_gtest_death_test_style = "threadsafe";

    cudaError_t ret;
    const struct textureReference * texref;
    cudaArray * array;

    const uint32_t ints = 1u << 16;

    struct cudaChannelFormatDesc desc;
    desc.f = cudaChannelFormatKindSigned;
    desc.x = 32;
    desc.y = desc.z = desc.w = 0;

    ret = cudaMallocArray(&array, &desc, ints, 0, 0);
    ASSERT_EQ(cudaSuccess, ret);

    int version;
    ret = cudaRuntimeGetVersion(&version);
    ASSERT_EQ(cudaSuccess, ret);

    #if CUDA_VERSION >= 5000
    if (version < 5000 /* 5.0 */) {
    #endif
        ret = cudaGetTextureReference(&texref, "tex_src");
    #if CUDA_VERSION >= 5000
    } else {
        ret = cudaGetTextureReference(&texref, &tex_src);
    }
    #endif
    ASSERT_EQ(cudaSuccess, ret);

    EXPECT_EXIT(
        cudaBindTextureToArray(texref, array, NULL),
        ::testing::KilledBySignal(SIGSEGV), "");

    EXPECT_EXIT(
        cudaBindTextureToArray(texref, NULL,  NULL),
        ::testing::KilledBySignal(SIGSEGV), "");

    /* We never bound anything successfully, so no need for
       cudaUnbindTexture. */

    ret = cudaFreeArray(array);
    ASSERT_EQ(cudaSuccess, ret);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
