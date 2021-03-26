#include <objects/Maze.hpp>
#include <glm/gtc/constants.hpp>

void Maze::generate(Cache<std::string, struct Model> &cache, std::vector<Instance> &instances) {
    std::vector<std::pair<short, short>> positions = {
            {1, 0}, {3, 0}, {5, 0}, {7, 0}, {9, 0}, {11, 0}, {13, 0}, {15, 0},
            {0, 1}, {8, 1}, {16, 1},
            {1, 2}, {5, 2}, {7, 2}, {11, 2}, {13, 2},
            {0, 3}, {14, 3}, {16, 3},
            {5, 4}, {9, 4}, {11, 4}, {15, 4},
            {0, 5}, {2, 5}, {4, 5}, {10, 5}, {16, 5},
            {1, 6}, {5, 6}, {7, 6}, {9, 6}, {13, 6}, {15, 6},
            {0, 7}, {10, 7}, {12, 7}, {16, 7},
            {1, 8}, {3, 8}, {7, 8}, {11, 8},
            {0, 9}, {8, 9}, {14, 9}, {16, 9},
            {3, 10}, {7, 10}, {9, 10}, {13, 10}, {15, 10},
            {0, 11}, {2, 11}, {6, 11}, {10, 11}, {16, 11},
            {1, 12}, {5, 12}, {9, 12}, {13, 12}, {15, 12},
            {0, 13}, {6, 13}, {12, 13}, {16, 13},
            {1, 14}, {3, 14}, {7, 14}, {13, 14},
            {0, 15}, {8, 15}, {10, 15}, {16, 15},
            {1, 16}, {3, 16}, {5, 16}, {9, 16}, {11, 16}, {13, 16}, {15, 16},
    };

    for (std::pair<short, short> const& coordinate : positions) {
        instances.push_back(Instance::create_static(cache.get("models/wall.fbx")));
        Instance* instance = &instances.back();
        instance->position = glm::vec3( 8 - (float)coordinate.first, 0, 8 - (float)coordinate.second);
        if (coordinate.first % 2 == 0) {
            instance->rotation[1] = glm::half_pi<float>();
        }
        instance->xrayable = true;
    }
}
