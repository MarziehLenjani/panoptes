/**
 * Panoptes - A Binary Translation Framework for CUDA
 * (c) 2011-2013 Chris Kennelly <chris@ckennelly.com>
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

extern "C" __global__ void k_pmevent() {
    prof_trigger(0);
}

/**
 * Per the PTX documentation, PTX ISA 3.0 was released with CUDA runtime
 * version 4.1.  As pmevent.mask appeared with this ISA, we restrict
 * compilation of this portion of the test to runtime versions 4.1 and newer.
 */
#if CUDART_VERSION >= 4010 /* 4.1 */
extern "C" __global__ void k_pmmask() {
    /* This must be an immediate */
    asm volatile("pmevent.mask 3;");
}
#endif

TEST(kPMEVENT, ExplicitStream) {
    cudaError_t ret;
    cudaStream_t stream;

    ret = cudaStreamCreate(&stream);
    ASSERT_EQ(cudaSuccess, ret);

    k_pmevent<<<256, 16, 0, stream>>>();

    ret = cudaStreamSynchronize(stream);
    EXPECT_EQ(cudaSuccess, ret);

    ret = cudaStreamDestroy(stream);
    ASSERT_EQ(cudaSuccess, ret);
}

#if CUDART_VERSION >= 4010 /* 4.1 */
TEST(kPMEVENT, Mask) {
    cudaError_t ret;
    cudaStream_t stream;

    ret = cudaStreamCreate(&stream);
    ASSERT_EQ(cudaSuccess, ret);

    k_pmmask<<<256, 16, 0, stream>>>();

    ret = cudaStreamSynchronize(stream);
    EXPECT_EQ(cudaSuccess, ret);

    ret = cudaStreamDestroy(stream);
    ASSERT_EQ(cudaSuccess, ret);
}
#endif

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
