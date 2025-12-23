/*
Epifancev Artem
st142163@student.spbu.ru
LabWork1
*/

#include <gtest/gtest.h>
#include "bmp_image.h"
#include <fstream>
#include <filesystem>
#include <cmath>

TEST(BMPTest, LoadValidFile) {
    BMP image;
    EXPECT_TRUE(image.Load("sample.bmp"));
    
    EXPECT_GT(image.Width(), 0);
    EXPECT_GT(image.Height(), 0);
    EXPECT_GT(image.Memory(), 0);
}

TEST(BMPTest, SaveFile) {
    BMP image;
    
    ASSERT_TRUE(image.Load("sample.bmp"));
    EXPECT_TRUE(image.Save("test_output.bmp"));
    
    EXPECT_TRUE(std::filesystem::exists("test_output.bmp"));
    EXPECT_GT(std::filesystem::file_size("test_output.bmp"), 0);
}

TEST(BMPTest, RotateDimensions) {
    BMP image;
    ASSERT_TRUE(image.Load("sample.bmp"));
    
    BMP rotated_cw = image.RotateCW();
    EXPECT_EQ(rotated_cw.Width(), image.Height()) ;
    EXPECT_EQ(rotated_cw.Height(), image.Width());
    
    BMP rotated_ccw = image.RotateCCW();
    EXPECT_EQ(rotated_ccw.Width(), image.Height());
    EXPECT_EQ(rotated_ccw.Height(), image.Width());
}

TEST(BMPTest, GAUSSIANFilter) {
    BMP image;
    ASSERT_TRUE(image.Load("sample.bmp"));
    
    BMP filtered = image.GaussianFilter();
    
    bool changed = false;
    int checkPoints = 0;
    int maxChecks = 100;
    
    for (int i = 0; i < maxChecks && !changed; i++) {
        int x = rand() % image.Width();
        int y = rand() % image.Height();
        
        uint8_t r1, g1, b1, r2, g2, b2;
        image.GetPixel(x, y, r1, g1, b1);
        filtered.GetPixel(x, y, r2, g2, b2);
        
        if (r1 != r2 || g1 != g2 || b1 != b2) {
            changed = true;
        }
        checkPoints++;
    }
    
    EXPECT_TRUE(changed);
    EXPECT_EQ(image.Width(), filtered.Width());
    EXPECT_EQ(image.Height(), filtered.Height());
}

int test(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
