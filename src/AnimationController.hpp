#ifndef ANIMATIONCONTROLLER_HPP
#define ANIMATIONCONTROLLER_HPP

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <thread>
#include <chrono>
#include <iostream>
#include <sstream>
#include <cmath>
#include "Characters.hpp"

using namespace ftxui;

class AnimationController {
public:
    void run() {
        Pacman pac;
        Ghost ghost;

        double ghost_pos = 40;  // posición inicial del fantasma
        double pac_pos = 0;     // posición inicial del Pac-Man
        const int total_frames = 80;
        const int vertical_offset = -10; // baja la animación en pantalla

        for (int frame = 0; frame < total_frames; ++frame) {
            auto screen = Screen::Create(Dimension::Full(), Dimension::Full());

            // --- Movimiento corregido ---
            pac_pos += 1.0; // Pac-Man avanza hacia la derecha

            double distancia = ghost_pos - pac_pos;

            if (distancia > 25.0) {
                ghost.scared = false;
                ghost_pos -= 0.4; // se acerca lentamente
            } else if (distancia > 5.0) {
                ghost.scared = true;
                ghost_pos += 0.8; // huye hacia la derecha
            } else {
                ghost.scared = true;
                ghost_pos = pac_pos + 5.0; // evita superposición
            }

            // Frames ASCII
            auto pac_frame = pac.getFrame();
            auto ghost_frame = ghost.getFrame();
            int num_lines = std::max(pac_frame.size(), ghost_frame.size());

            // --- Colores ---
            auto pacColor = Color::RGB(255, 255, 0); // Amarillo brillante

            // Fantasma tiembla entre azul y blanco si está asustado
            Color ghostColor;
            if (ghost.scared) {
                if (frame % 2 == 0)
                    ghostColor = Color::RGB(65, 105, 225); // Azul
                else
                    ghostColor = Color::RGB(255, 255, 255); // Blanco
            } else {
                ghostColor = Color::RGB(255, 0, 0); // Rojo normal
            }

            Elements scene_lines;

            // Espaciado vertical
            for (int i = 0; i < vertical_offset; ++i)
                scene_lines.push_back(text(" "));

            // Dibujar línea por línea
            for (int i = 0; i < num_lines; ++i) {
                std::string pac_line = (i < pac_frame.size()) ? pac_frame[i] : "";
                std::string ghost_line = (i < ghost_frame.size()) ? ghost_frame[i] : "";

                int pac_x = static_cast<int>(std::round(pac_pos));
                int ghost_x = static_cast<int>(std::round(ghost_pos));

                // Evitar que se encimen
                if (std::abs(pac_x - ghost_x) < 2)
                    ghost_x = pac_x + 2;

                // --- Pintar Pac-Man y fantasma ---
                Elements colored_parts;
                if (pac_x <= ghost_x) {
                    colored_parts.push_back(text(std::string(pac_x, ' ') + pac_line) | color(pacColor));
                    int gap = ghost_x - (pac_x + (int)pac_line.size());
                    if (gap > 0)
                        colored_parts.push_back(text(std::string(gap, ' ')));
                    colored_parts.push_back(text(ghost_line) | color(ghostColor));
                } else {
                    colored_parts.push_back(text(std::string(ghost_x, ' ') + ghost_line) | color(ghostColor));
                    int gap = pac_x - (ghost_x + (int)ghost_line.size());
                    if (gap > 0)
                        colored_parts.push_back(text(std::string(gap, ' ')));
                    colored_parts.push_back(text(pac_line) | color(pacColor));
                }

                scene_lines.push_back(hbox(colored_parts));
            }

            // Escena completa
            auto scene = vbox({
                vbox(scene_lines),
                separator(),
                text("") | center
            }) | bgcolor(Color::Black);

            Render(screen, scene);
            screen.Print();

            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }

        std::cout << "\n fin de animacion \n";
    }
};

#endif
