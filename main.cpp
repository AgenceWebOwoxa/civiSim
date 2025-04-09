#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include "gui.h"
#include "setting.h"

struct UserData {
    GUI* gui;
    Setting* setting;
    bool generateClicked;
};

void generateMap(int mapSize, const std::string& filename) {
    std::vector<std::vector<int>> grid(mapSize, std::vector<int>(mapSize, 0));

    for (int i = 0; i < mapSize; ++i) {
        for (int j = 0; j < mapSize; ++j) {
            grid[i][j] = (rand() % 2);
        }
    }

    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        outFile << mapSize << "\n";
        for (int i = 0; i < mapSize; ++i) {
            for (int j = 0; j < mapSize; ++j) {
                outFile << grid[i][j];
                if (j < mapSize - 1) outFile << " ";
            }
            outFile << "\n";
        }
        outFile.close();
        std::cout << "Carte exportée dans " << filename << std::endl;
    } else {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << filename << " pour écriture" << std::endl;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    UserData* userData = (UserData*)glfwGetWindowUserPointer(window);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        // Inverser Y pour correspondre à l'axe de drawRectangle
        ypos = 800.0 - ypos;

        std::cout << "Clic détecté à (" << xpos << ", " << ypos << ")" << std::endl;

        // Ajuster la plage Y pour correspondre à la position réelle du bouton
        if (xpos >= 375.0 && xpos <= 300.0 && ypos >= 375.0 && ypos <= 300.0) {
            userData->generateClicked = true;
            std::cout << "Bouton GENERATE cliqué !" << std::endl;
        } else {
            std::cout << "Clic hors du bouton GENERATE" << std::endl;
        }
    }
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Erreur : Impossible d'initialiser GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Map Editor", nullptr, nullptr);
    if (!window) {
        std::cerr << "Erreur : Impossible de créer la fenêtre GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Erreur : Impossible d'initialiser GLAD" << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 800);

    GUI gui;
    gui.init();

    Setting setting;

    UserData userData;
    userData.gui = &gui;
    userData.setting = &setting;
    userData.generateClicked = false;

    glfwSetWindowUserPointer(window, &userData);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while (!glfwWindowShouldClose(window)) {
        setting.update();

        gui.render();

        if (userData.generateClicked) {
            std::cout << "Génération de la carte..." << std::endl;
            int mapSize = userData.setting->getMapSize();
            std::cout << "Taille de la carte : " << mapSize << std::endl;
            generateMap(mapSize, "map.txt");
            userData.generateClicked = false;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}