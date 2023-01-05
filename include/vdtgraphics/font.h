/// Copyright (c) Vito Domenico Tagliente
#pragma once

#include <filesystem>
#include <string>

namespace graphics
{
	struct Font final
	{	
		Font();
		Font(const std::shared_ptr<unsigned char>& data, const std::filesystem::path& path, size_t size);
		Font(const Font& other);
		~Font();
		
		static Font load(const std::filesystem::path& filename);

		Font& operator= (const Font& other);
		bool operator== (const Font& other) const;
		bool operator!= (const Font& other) const;

		std::filesystem::path path;
		std::shared_ptr<unsigned char> data;
		size_t size;
	};
}