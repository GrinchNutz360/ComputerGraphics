#pragma once
#include "CubeMap.h"

namespace neu {
	bool CubeMap::Load(const std::string& filenames) {
		m_target = GL_TEXTURE_CUBE_MAP;

		glGenTextures(1, &m_texture);
		glBindTexture(m_target, m_texture);

		GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		// extract cubemap from filename
		std::vector<std::string> filename_split;
		std::istringstream stream(filenames);
		std::string tempstring = "";

		while (stream >> tempstring) {
			filename_split.push_back(tempstring);
		}

		for (size_t i = 0; i < filename_split.size(); i++) {
			SDL_Surface* surface = IMG_Load(filename_split[i].c_str());
			if (!surface) {
				LOG_ERROR("Could not load image: {}", filenames);
				return false;
			}

			const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(surface->format);

			int channels = details->bytes_per_pixel;
			GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
			GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

			glTexImage2D(targets[i], 0, internalFormat, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);
			SDL_DestroySurface(surface);
		}

		glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		return true;
	}
}