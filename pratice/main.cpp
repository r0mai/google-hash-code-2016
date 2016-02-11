#include "stuff.h"

struct Point {
    int r, c;
};

struct Line {
    int r1, c1;
    int r2, c2;
};

struct RectangleCandidate {
    int x, y; // bal felso
    int size;
    std::vector<Point> holes;
};

std::vector<Line> scanLine(const std::vector<std::string>& m) {
    int width = m[0].size();
    int height = m.size();

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

    return lines;
}

int main(int argc, char* argv[]) {
    int width;
    int height;
    std::cin >> height >> width;
    std::cin.ignore();

    std::vector<std::string> m(height);
    for (int y = 0; y < height; ++y) {
        std::getline(std::cin, m[y]);
    }

    std::vector<Line> lines = scanLine(m);

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
