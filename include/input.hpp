#pragma once

#include <unordered_map>

namespace Eng {

class Input {
public:
    Input() = default;
    void process_key(int key, int action);
    void update();

    bool key_down(int key) const;
    bool key_pressed(int key) const;
    bool key_released(int key) const;

private:
    std::unordered_map<int, int> current;
    std::unordered_map<int, int> previous;
};

}