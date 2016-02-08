#include "stuff.h"

struct Line {
    int r1, c1;
    int r2, c2;
};

int main(int argc, char* argv[]) {
    int width;
    int height;
    std::cin >> height >> width;
    std::cin.ignore();

    std::vector<std::string> m(height);
    for (int y = 0; y < height; ++y) {
        std::getline(std::cin, m[y]);
    }

    std::vector<Line> lines;

    for (int y = 0; y < height; ++y) {
        int start = -1;
        for (int x = 0; x < width; ++x) {
            if (m[y][x] == '#') {
                if (start == -1) {
                    start = x;
                }
            } else {
                if (start != -1) {
                    lines.push_back({y, start, y, x - 1});
                    start = -1;
                }
            }
        }
        if (start != -1) {
            lines.push_back({y, start, y, width - 1});
        }
    }
    std::cout << lines.size() << std::endl;
    for (const Line& line : lines) {
        std::cout << "PAINT_LINE "
            << line.r1 << ' '
            << line.c1 << ' '
            << line.r2 << ' '
            << line.c2 << std::endl;
    }
	return 0;
}
