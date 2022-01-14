#pragma once


int decodePNG(std::vector<unsigned char>& out_image, int & image_width, int & image_height, const unsigned char* in_png, size_t in_size, bool convert_to_rgba32 = true);
void loadFile(std::vector<unsigned char>& buffer, const char * filename);
std::vector<unsigned char> loadPNG(const char * filename, int & image_width, int & image_height, bool convert_to_rgba32 = true);