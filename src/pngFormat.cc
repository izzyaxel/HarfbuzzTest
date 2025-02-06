#include "pngFormat.hh"

#include <png.h>
#include <cstring>

PNG::PNG(const u32 width, const u32 height, const char colorFormat, const char bitDepth, std::vector<u8>&& imageData) :
  width(width), height(height), colorFormat(colorFormat), bitDepth(bitDepth), imageData(imageData)
{}

void pngErrorCallback(png_structp, const png_const_charp error)
{
  
}

PNG decodePNG(const std::string& filePath)
{
  FILE* input = fopen(filePath.data(), "rb");
  if(!input)
  {
    return {0, 0, 0, 0, {}};
  }

  png_structp pngPtr;
  png_infop infoPtr;
  png_byte header[8];
  size_t foo = fread(header, 8, 1, input);

  if(png_sig_cmp(header, 0, 8) != 0)
  {
    return {0, 0, 0, 0, {}};
  }

  pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, pngErrorCallback, nullptr);
  if(pngPtr == nullptr)
  {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return {0, 0, 0, 0, {}};
  }

  png_set_error_fn(pngPtr, png_get_error_ptr(pngPtr), [](png_structp, png_const_charp)
  {}, [](png_structp, png_const_charp)
  {});
  infoPtr = png_create_info_struct(pngPtr);

  if(!infoPtr)
  {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return {0, 0, 0, 0, {}};
  }

  if(setjmp(png_jmpbuf(pngPtr)))
  {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return {0, 0, 0, 0, {}};
  }

  png_init_io(pngPtr, input);
  png_set_sig_bytes(pngPtr, 8);
  png_read_info(pngPtr, infoPtr);
  const u32 width = png_get_image_width(pngPtr, infoPtr);
  const u32 height = png_get_image_height(pngPtr, infoPtr);
  const char colorType = (char)png_get_color_type(pngPtr, infoPtr);
  const char bitDepth = (char)png_get_bit_depth(pngPtr, infoPtr);

  if(colorType & PNG_COLOR_MASK_PALETTE)
  {
    return {0, 0, 0, 0, {}};
  }

  png_set_interlace_handling(pngPtr);
  png_read_update_info(pngPtr, infoPtr);

  if(setjmp(png_jmpbuf(pngPtr)))
  {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return {0, 0, 0, 0, {}};
  }
  std::vector<u8> imageData;
  imageData.resize(height * png_get_rowbytes(pngPtr, infoPtr));
  u8** fauxData = new u8* [height];
  for(size_t i = 0; i < height; i++) fauxData[i] = imageData.data() + i * png_get_rowbytes(pngPtr, infoPtr);
  png_read_image(pngPtr, fauxData);
  png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
  delete[] fauxData;
  fclose(input);
  return PNG{width, height, colorType, bitDepth, std::move(imageData)};
}

struct PNGReadStruct
{
  PNGReadStruct(const u8* data, size_t bufferLocation) : data(data), bufferLocation(bufferLocation)
  {}

  u8 const* data = nullptr;
  size_t bufferLocation = 0;
};

void pngReadFn(const png_structp read, const png_bytep data, const png_size_t length)
{
  PNGReadStruct* buffer = reinterpret_cast<PNGReadStruct*>(png_get_io_ptr(read));
  memcpy(data, buffer->data + buffer->bufferLocation, length);
  buffer->bufferLocation += length;
}

PNG decodePNG(const std::vector<u8>& file)
{
  PNGReadStruct pngrs{file.data(), 0};
  png_structp pngPtr;
  png_infop infoPtr;
  png_byte header[8];
  memcpy(header, pngrs.data, 8);

  if(png_sig_cmp(header, 0, 8) != 0)
  {
    return {0, 0, 0, 0, {}};
  }

  pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, pngErrorCallback, nullptr);
  if(pngPtr == nullptr)
  {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return {0, 0, 0, 0, {}};
  }

  png_set_error_fn(pngPtr, png_get_error_ptr(pngPtr), [](png_structp, png_const_charp)
  {}, [](png_structp, png_const_charp)
  {});
  infoPtr = png_create_info_struct(pngPtr);

  if(!infoPtr)
  {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return {0, 0, 0, 0, {}};
  }

  if(setjmp(png_jmpbuf(pngPtr)))
  {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return {0, 0, 0, 0, {}};
  }

  png_set_read_fn(pngPtr, &pngrs, pngReadFn);
  png_set_sig_bytes(pngPtr, 0);
  png_read_info(pngPtr, infoPtr);
  u32 width = png_get_image_width(pngPtr, infoPtr);
  u32 height = png_get_image_height(pngPtr, infoPtr);
  char colorType = png_get_color_type(pngPtr, infoPtr);
  char bitDepth = png_get_bit_depth(pngPtr, infoPtr);

  if(colorType & PNG_COLOR_MASK_PALETTE)
  {
    return {0, 0, 0, 0, {}};
  }

  png_set_interlace_handling(pngPtr);
  png_read_update_info(pngPtr, infoPtr);

  if(setjmp(png_jmpbuf(pngPtr)))
  {
    png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
    return {0, 0, 0, 0, {}};
  }
  std::vector<u8> imageData;
  imageData.resize(height * png_get_rowbytes(pngPtr, infoPtr));
  u8** fauxData = new u8* [height];
  for(size_t i = 0; i < height; i++) fauxData[i] = imageData.data() + i * png_get_rowbytes(pngPtr, infoPtr);
  png_read_image(pngPtr, fauxData);
  png_destroy_read_struct(&pngPtr, &infoPtr, nullptr);
  delete[] fauxData;
  return PNG{width, height, colorType, bitDepth, std::move(imageData)};
}
