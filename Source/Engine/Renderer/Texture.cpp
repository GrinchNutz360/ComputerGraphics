#include "Texture.h"
#include "Renderer.h"

namespace neu {
    /// <summary>
    /// Destructor ensures proper cleanup of SDL resources.
    /// Destroys the SDL texture if it exists, freeing GPU memory.
    /// </summary>
    Texture::~Texture() {
        // If texture exists, destroy texture to free GPU resources
        if (m_texture) glDeleteTextures(1, &m_texture);
    }

    /// <summary>
    /// Loads an image file from disk and creates an SDL texture for rendering.
    /// The loading process involves two steps:
    /// 1. Load the image into a CPU-side surface using SDL_image
    /// 2. Create a GPU-side texture from the surface for efficient rendering
    /// The surface is freed after texture creation to conserve memory.
    /// </summary>
    /// <param name="filename">Path to the image file to load</param>
    /// <param name="renderer">Reference to the Renderer that provides the SDL_Renderer context</param>
    /// <returns>True if the texture was successfully loaded and created; otherwise, false</returns>
    bool Texture::Load(const std::string& filename) {
        // Load image onto a CPU-side surface
        // SDL_image supports various formats: PNG, JPG, BMP, GIF, etc.
        SDL_Surface* surface = IMG_Load(filename.c_str());
        if (!surface) {
            LOG_ERROR("Could not load image: {}", filename);
            return false;
        }

        SDL_FlipSurface(surface, SDL_FLIP_VERTICAL);

        const SDL_PixelFormatDetails* details = SDL_GetPixelFormatDetails(surface->format);

        int channels = details->bytes_per_pixel;
        GLenum internalFormat = (channels == 4) ? GL_RGBA8 : GL_RGB8;
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;

        m_size.x = surface->w;
        m_size.y = surface->h;

        glGenTextures(1, &m_texture);
        glBindTexture(m_target, m_texture);

        glTexImage2D(m_target, 0, internalFormat, surface->w, surface->h, 0, format, GL_UNSIGNED_BYTE, surface->pixels);

        // Texture parameters
        glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);

        SDL_DestroySurface(surface);

        return true;
    }

    void Texture::Bind() {
        glBindTexture(m_target, m_texture);

        glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, (filter == Filter::Nearest) ? GL_NEAREST : GL_LINEAR);
        glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, (filter == Filter::Nearest) ? GL_NEAREST : GL_LINEAR);
                
        switch (wrap) {
        case neu::Texture::Wrap::Repeat:
            glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_REPEAT);
            break;
        case neu::Texture::Wrap::RepeatMirror:
            glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
            glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
            break;
        case neu::Texture::Wrap::ClampEdge:
            glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            break;
        default:
            break;
        }
    }

    /*void Texture::UpdateGui() {
        ImGui::Text("Name: %s", file::GetFilename(name).c_str());
        ImGui::Text("Size: %dx%d", m_size.x, m_size.y);
        Editor::ShowTexture(*this, 128, 128);*/

        //ImGui::Text("Filter:");
        //ImGui::SameLine();
        //const char* filters[] = { "Nearest", "Linear" };
        //ImGui::Combo(" ", (int*)&filter, filters, 2);

        //ImGui::Text("Wrap:  ");
        //ImGui::SameLine();
        //const char* wraps[] = { "Repeat", "Mirrored Repeat", "Clamp Edge"};
        //ImGui::Combo(" ", (int*)&wrap, wraps, 3);
    //}

}