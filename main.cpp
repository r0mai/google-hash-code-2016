#include "stuff.h"

struct Command {
    virtual ~Command() {}
    virtual void draw(std::vector<std::string>& img) const = 0;
    virtual std::string str() const = 0;
};

struct Line : public Command {
    int r1, c1;
    int r2, c2;

    Line(int r1, int c1, int r2, int c2)
        : r1(r1), c1(c1), r2(r2), c2(c2) { }

    virtual void draw(std::vector<std::string>& img) const override {
        if (c1 == c2) {
            int y1 = std::min(r1, r2);
            int y2 = std::max(r1, r2);

            for (int i = y1; i <= y2; i++) {
                img[i][c1] = '#';
            }
        }
        else {
            int x1 = std::min(c1, c2);
            int x2 = std::max(c1, c2);

            for (int i = x1; i <= x2; i++) {
                img[r1][i] = '#';
            }
        }
    }

    virtual std::string str() const override {
        std::stringstream ss;
        ss << "PAINT_LINE "
            << r1 << ' '
            << c1 << ' '
            << r2 << ' '
            << c2 << std::endl;
        return ss.str();
    }
};

struct Square : public Command {
    int r, c, s;

    Square(int r, int c, int s)
        : r(r), c(c), s(s) { }

    virtual void draw(std::vector<std::string>& img) const override {
        for (int y = r - s; y <= r + s; ++y) {
            for (int x = c - s; x <= c + s; ++x) {
                img[y][x] = '#';
            }
        }
    }

    virtual std::string str() const override {
        std::stringstream ss;
        ss << "PAINT_SQUARE "
            << r << ' '
            << c << ' '
            << s << std::endl;
        return ss.str();
    }
};

struct Erase : public Command {
    int r, c;

    Erase(int r, int c)
        : r(r), c(c) { }

    virtual void draw(std::vector<std::string>& img) const override {
        img[r][c] = '.';
    }

    virtual std::string str() const override {
        std::stringstream ss;
        ss << "ERASE_CELL "
            << r << ' '
            << c << std::endl;
        return ss.str();
    }
};

std::vector<std::string> draw(int w, int h, const std::vector<Command*>& commands) {
    std::vector<std::string> img;
    img.resize(h);

    for (auto& row : img) {
        row = std::string(w, '.');
    }

    for (const auto cmd : commands) {
        cmd->draw(img);
    }

    return img;
}

bool test(const std::vector<std::string>& img, std::vector<Command*>& commands) {
    int width = img[0].size();
    int height = img.size();

    auto result = draw(width, height, commands);
    for (int i = 0; i < height; i++) {
        if (result[i] != img[i]) {
            std::cout << "mismatch: " << i << std::endl;
            std::cout << result[i] << std::endl;
            std::cout << img[i] << std::endl;
            return false;
        }
    }

    return true;
}

std::vector<Command*> build(const std::vector<std::string>& m) {
    int width = m[0].size();
    int height = m.size();

    std::vector<Command*> commands;

    for (int y = 0; y < height; ++y) {
        int start = -1;
        for (int x = 0; x < width; ++x) {
            if (m[y][x] == '#') {
                if (start == -1) {
                    start = x;
                }
            }
            else {
                if (start != -1) {
                    commands.push_back(new Line(y, start, y, x - 1));
                    start = -1;
                }
            }
        }
        if (start != -1) {
            commands.push_back(new Line(y, start, y, width - 1));
        }
    }

    return commands;
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

    auto commands = build(m);
    std::cout << commands.size() << std::endl;

    for (const auto cmd : commands) {
        std::cout << cmd->str();
    }

    return 0;
}
