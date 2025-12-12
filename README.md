# C++ Software Renderer

A high-performance C++ software renderer built on top of **OpenGL**, with support for fully programmable **vertex and fragment shaders**, and multiple lighting models. The project uses **SDL2** for windowing/input and **glm** for math operations.

---

## 📘 Introduction

This project is a modular and extensible software renderer designed for learning, experimentation, and visualization of graphics-pipeline concepts.  
Although it uses OpenGL as the drawing backend, all key stages of the rendering pipeline—transformation, lighting, rasterization logic, and shading—are implemented in **software**.

The goal of this renderer is to provide a transparent, hackable graphics pipeline suitable for studying rendering techniques or building custom visual experiments.

---

## ✨ Features

### 🔹 Core Rendering
- Custom **software rasterizer**
- Programmable **vertex shaders**
- Programmable **fragment shaders**
- Perspective-correct interpolation
- Back-face culling and depth buffering

### 🔹 Lighting Models
- **Gouraud Shading** (per-vertex lighting)
- **Phong Shading** (per-pixel lighting)
- Support for directional, point, and ambient light sources

### 🔹 Framework
- **SDL2** window management & input
- **glm** for vector/matrix operations
- Cross-platform C++17 codebase

---

## 🛠️ Installation

### **Prerequisites**
Make sure you have the following installed:

- Visual Studio 2022 (or higher)
- C++17-compatible compiler  
- **CMake** (3.10+ recommended)  
- **SDL2**  https://libsdl.org
- **OpenGL** development libraries  
- **glm**  https://github.com/g-truc/glm

### **Build Instructions**

This is a full engine with multiple technologies added. If you run it as is you will see this:
<img width="1264" height="1066" alt="Screenshot 2025-12-09 112749-5" src="https://github.com/user-attachments/assets/cf7fb171-7e07-401e-a8e2-7dae05116508" />




```bash
git clone https://github.com/GrinchNutz360/ComputerGraphics.git
cd ComputerGraphics
mkdir build && cd build
cmake ..
cmake --build .
