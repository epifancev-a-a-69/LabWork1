/*
Epifancev Artem
st142163@student.spbu.ru
LabWork1
*/

#include "bmp_image.h"
#include <fstream>
#include <cmath>
#include <algorithm>

bool BMP::Load(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file) return false;

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (header.type != 0x4D42 || header.bits != 24) return false;

    int width = Width();
    int height = Height();

    if (header.height < 0)
    {
        header.height = -header.height;
    }

    int row_size = ((width * 3 + 3) / 4) * 4;
    int padding = row_size - width * 3;

    data.resize(static_cast<size_t>(row_size) * static_cast<size_t>(height));
    file.seekg(header.offset);

    for (int y = 0; y < height; ++y)
    {
        file.read(reinterpret_cast<char*>(&data[static_cast<size_t>(y) * static_cast<size_t>(row_size)]),
                  static_cast<std::streamsize>(width * 3));
        if (padding > 0)
        {
            file.seekg(padding, std::ios::cur);
        }
    }

    return file.good();
}

bool BMP::Save(const std::string& filename)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file) return false;

    int width = Width();
    int height = Height();
    int row_size = ((width * 3 + 3) / 4) * 4;
    int padding = row_size - width * 3;

    BMPHeader save_header = header;
    save_header.image_size = static_cast<uint32_t>(row_size * height);
    save_header.size = save_header.offset + save_header.image_size;

    if (save_header.height < 0)
    {
        save_header.height = -save_header.height;
    }

    file.write(reinterpret_cast<const char*>(&save_header), sizeof(save_header));

    for (int y = 0; y < height; ++y)
    {
        file.write(reinterpret_cast<const char*>(&data[static_cast<size_t>(y) * static_cast<size_t>(row_size)]),
                   static_cast<std::streamsize>(width * 3));
        for (int i = 0; i < padding; ++i)
        {
            file.put(0);
        }
    }

    return file.good();
}

BMP BMP::RotateCW() const
{
    int w = Width(), h = Height();
    BMP result;
    result.header.width = h;
    result.header.height = w;

    int new_row_size = ((h * 3 + 3) / 4) * 4;
    result.data.resize(new_row_size * w);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            uint8_t r, g, b;
            GetPixel(x, y, r, g, b);
            result.SetPixel(h-1-y, x, r, g, b);
        }
    }
    return result;
}

BMP BMP::RotateCCW() const
{
    int w = Width(), h = Height();
    BMP result;
    result.header.width = h;
    result.header.height = w;

    int new_row_size = ((h * 3 + 3) / 4) * 4;
    result.data.resize(new_row_size * w);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            uint8_t r, g, b;
            GetPixel(x, y, r, g, b);
            result.SetPixel(y, w-1-x, r, g, b);
        }
    }
    return result;
}

BMP BMP::GaussianFilter() const
{
    const double kernel[3][3] =
    {
        {0.077847, 0.123317, 0.077847},
        {0.123317, 0.195346, 0.123317},
        {0.077847, 0.123317, 0.077847}
    };

    int w = Width(), h = Height();
    BMP result;

    result.header = header;
    result.header.width = w;
    result.header.height = h;

    int row_size = ((w * 3 + 3) / 4) * 4;
    result.data.resize(row_size * h);

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            double r = 0, g = 0, b = 0;

            for (int ky = -1; ky <= 1; ++ky)
            {
                for (int kx = -1; kx <= 1; ++kx)
                {
                    int px = x + kx;
                    int py = y + ky;

                    if (px < 0) px = -px;
                    else if (px >= w) px = 2*w - px - 2;

                    if (py < 0) py = -py;
                    else if (py >= h) py = 2*h - py - 2;

                    double weight = kernel[ky+1][kx+1];

                    uint8_t pr, pg, pb;
                    GetPixel(px, py, pr, pg, pb);

                    r += pr * weight;
                    g += pg * weight;
                    b += pb * weight;
                }
            }

            r = std::max(0.0, std::min(255.0, r));
            g = std::max(0.0, std::min(255.0, g));
            b = std::max(0.0, std::min(255.0, b));

            result.SetPixel(x, y,
                            static_cast<uint8_t>(std::round(r)),
                            static_cast<uint8_t>(std::round(g)),
                            static_cast<uint8_t>(std::round(b))
                           );
        }
    }
    return result;
}

void BMP::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b)
{
    int w = Width();
    int row_size = ((w * 3 + 3) / 4) * 4;
    size_t idx = static_cast<size_t>(y) * static_cast<size_t>(row_size) + static_cast<size_t>(x) * 3;

    if (idx + 2 < data.size())
    {
        data[idx] = b;
        data[idx+1] = g;
        data[idx+2] = r;
    }
}

void BMP::GetPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b) const
{
    int w = Width();
    int row_size = ((w * 3 + 3) / 4) * 4;
    size_t idx = static_cast<size_t>(y) * static_cast<size_t>(row_size) + static_cast<size_t>(x) * 3;

    if (idx + 2 < data.size())
    {
        b = data[idx];
        g = data[idx+1];
        r = data[idx+2];
    }
    else
    {
        r = g = b = 0;
    }
}
