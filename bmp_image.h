/*
Epifancev Artem
st142163@student.spbu.ru
LabWork1
*/

#pragma once
#include "image.h"
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BMPHeader
{
    uint16_t type = 0x4D42;
    uint32_t size = 0;
    uint16_t reserved1 = 0;
    uint16_t reserved2 = 0;
    uint32_t offset = 54;
    uint32_t info_size = 40;
    int32_t width = 0;
    int32_t height = 0;
    uint16_t planes = 1;
    uint16_t bits = 24;
    uint32_t compression = 0;
    uint32_t image_size = 0;
    int32_t x_ppm = 0;
    int32_t y_ppm = 0;
    uint32_t colors = 0;
    uint32_t important = 0;
};
#pragma pack(pop)

class BMP : public Image
{
    BMPHeader header;
    std::vector<uint8_t> data;

public:
    bool Load(const std::string& filename) override;
    bool Save(const std::string& filename) override;

    void GetPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b) const;

    std::size_t Width() const override
    {
        return std::abs(header.width);
    }
    std::size_t Height() const override
    {
        return std::abs(header.height);
    }
    std::size_t Memory() const override
    {
        return data.size() + sizeof(header);
    }

    BMP RotateCW() const;
    BMP RotateCCW() const;
    BMP GaussianFilter() const;

private:
    void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
};
