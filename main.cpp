#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "gui.h"
#include "setting.h"

// Je mets à jour avec Grok - Test mise à jour

// ====================== STRUCTURE UTILISATEUR ======================
struct UserData {
    GUI* gui;
    Setting* setting;
    bool generateClicked = false;
    std::vector<std::vector<int>> currentMap;
};

// ====================== GÉNÉRATION DE LA CARTE ======================
void generateMap(int mapSize, std::vector<std::vector<int>>& grid) {
    grid.assign(mapSize, std::vector<int>(mapSize, 0));

    for (int i = 0; i < mapSize; ++i) {
        for (int j = 0; j < mapSize; ++j) {
            grid[i][j] = (rand() % 2);  // 0 ou 1
        }
    }
}

void exportMapToFile(const std::vector<std::vector<int>>& grid, const std::string& filename) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        int mapSize = grid.size();
        outFile << mapSize << "\n";
        for (const auto& row : grid) {
            for (size_t j = 0; j < row.size(); ++j) {
                outFile << row[j];
                if (j < row.size() - 1) outFile << " ";
            }
            outFile << "\n";
        }
        outFile.close();
        std::cout << "✅ Carte exportée dans " << filename << std::endl;
    } else {
        std::cerr << "❌ Erreur : Impossible d'ouvrir " << filename << std::endl;
    }
}

// ====================== CALLBACK SOURIS ======================
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        UserData* userData = (UserData*)glfwGetWindowUserPointer(window);
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Inversion Y (OpenGL commence en bas à gauche)
        ypos = 800.0 - ypos;

        std::cout << "Clic à (" << xpos << ", " << ypos << ")\n";

        // Zone du bouton GENERATE (à adapter selon ta GUI)
        if (xpos >= 300.0 && xpos <= 500.0 && ypos >= 50.0 && ypos <= 100.0) {
            userData->generateClicked = true;
            std::cout << "🚀 Bouton GENERATE cliqué !\n";
        }
    }
}

// ====================== MAIN ======================
int main() {
    srand(static_cast<unsigned>(time(nullptr)));  // Initialisation aléatoire

    if (!glfwInit()) {
        std::cerr << "Erreur : Impossible d'initialiser GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "civiSim - Map Editor", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erreur : Impossible de créer la fenêtre\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erreur : Impossible d'initialiser GLAD\n";
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 800);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    GUI gui;
    gui.init();

    Setting setting;

    UserData userData;
    userData.gui = &gui;
    userData.setting = &setting;

    glfwSetWindowUserPointer(window, &userData);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        setting.update();
        gui.render();

        if (userData.generateClicked) {
            std::cout << "Génération de la carte...\n";
            int mapSize = userData.setting->getMapSize();

            generateMap(mapSize, userData.currentMap);
            exportMapToFile(userData.currentMap, "map.txt");

            userData.generateClicked = false;
        }

        // TODO: Ajouter ici le rendu visuel de la carte (grille de carrés)

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
