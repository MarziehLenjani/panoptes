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

#include <cuda_runtime_api.h>
#include <gtest/gtest.h>

TEST(EventQuery, QueryNoRecord) {
    cudaError_t ret;
    cudaEvent_t event;

    ret = cudaEventCreate(&event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventQuery(event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventDestroy(event);
    EXPECT_EQ(cudaSuccess, ret);
}

TEST(EventQuery, SimpleQuery) {
    cudaError_t ret;
    cudaEvent_t event;
    cudaStream_t stream;

    ret = cudaStreamCreate(&stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventCreate(&event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventRecord(event, stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventQuery(event);
    EXPECT_TRUE(cudaSuccess == ret || cudaErrorNotReady == ret);

    ret = cudaStreamSynchronize(stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventQuery(event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventDestroy(event);
    EXPECT_EQ(cudaSuccess, ret);

    ret = cudaStreamDestroy(stream);
    ASSERT_EQ(cudaSuccess, ret);
}

TEST(EventQuery, SameStream) {
    cudaError_t ret;
    cudaEvent_t event;
    cudaStream_t stream;

    ret = cudaStreamCreate(&stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventCreate(&event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventRecord(event, stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventRecord(event, stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventQuery(event);
    EXPECT_TRUE(cudaSuccess == ret || cudaErrorNotReady == ret);

    ret = cudaStreamSynchronize(stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventQuery(event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventDestroy(event);
    EXPECT_EQ(cudaSuccess, ret);

    ret = cudaStreamDestroy(stream);
    ASSERT_EQ(cudaSuccess, ret);
}

TEST(EventQuery, TwoStreams) {
    cudaError_t ret;
    cudaEvent_t event;
    cudaStream_t stream[2];

    ret = cudaStreamCreate(&stream[0]);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaStreamCreate(&stream[1]);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventCreate(&event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventRecord(event, stream[0]);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventRecord(event, stream[1]);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaStreamSynchronize(stream[0]);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaStreamSynchronize(stream[1]);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventQuery(event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventDestroy(event);
    EXPECT_EQ(cudaSuccess, ret);

    ret = cudaStreamDestroy(stream[0]);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaStreamDestroy(stream[1]);
    ASSERT_EQ(cudaSuccess, ret);
}

TEST(EventQuery, QueryAfterStreamDestroy) {
    cudaError_t ret;
    cudaEvent_t event;
    cudaStream_t stream;

    ret = cudaStreamCreate(&stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventCreate(&event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventRecord(event, stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaStreamSynchronize(stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaStreamDestroy(stream);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventQuery(event);
    ASSERT_EQ(cudaSuccess, ret);

    ret = cudaEventDestroy(event);
    EXPECT_EQ(cudaSuccess, ret);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
