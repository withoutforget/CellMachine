#include <format>
#include <functional>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>

class Grid {
private:
    std::vector<bool> m_grid;
    size_t m_width;
    size_t m_height;
    void fill_zero() {
        m_grid.clear();
        for (size_t i = 0; i < m_grid.capacity(); i++) {
            m_grid.emplace_back(0);
        }
    }
    bool fits_in_size(size_t x, size_t y) const {
        return (x < m_width && y < m_height);
    }

public:
    Grid(size_t width, size_t heigth) : m_width(width), m_height(heigth) {
        m_grid.reserve(m_width * m_height);
        fill_zero();
    }

    auto data() const {
        return std::ranges::subrange(m_grid.cbegin(), m_grid.cend());
    }

    size_t width() const { return m_width; }

    size_t height() const { return m_height; }

    auto get_cell(size_t x, size_t y) {
        if (!fits_in_size(x, y))
            throw 0;
        return m_grid[y * m_width + x];
    }

    auto get_cell(size_t x, size_t y) const {
        if (!fits_in_size(x, y))
            throw 0;
        return m_grid[y * m_width + x];
    }

    auto get_row(size_t y) const {
        if (!fits_in_size(0, y))
            throw 0;
        auto r = std::ranges::subrange(m_grid.cbegin() + y * m_width,
            m_grid.cbegin() + (y + 1) * m_width);
        return r;
    }

    size_t count_alive_near(size_t xpos, size_t ypos) const {
        size_t c = 0;

        std::vector<std::pair<int, int>> near_cell_offsets = {
            {-1, 1}, {0, 1},   {1, 1},  {-1, 0},
            {1, 0},  {-1, -1}, {0, -1}, {1, -1} };

        for (const auto& [i, j] : near_cell_offsets) {
            if (xpos + i >= width() || ypos + j >= height()) {
                continue;
            }
            if (get_cell(xpos + i, ypos + j))
                c++;
        }
        return c;
    }

    std::string to_string(char TrueReplacement = '1',
        char FalseReplacement = '0') const {
        std::stringstream ss;
        for (size_t i = 0; i < m_height; i++) {
            for (auto el : get_row(i))
                ss << (el ? TrueReplacement : FalseReplacement);
            ss << std::endl;
        }
        return ss.str();
    }
};

class CellMachine {
private:
    Grid m_grid;

    std::function<bool(size_t)> KeepAlive = [](size_t near) -> bool {
        return near == 2 || near == 3;
        };
    std::function<bool(size_t)> RaiseDead = [](size_t near) -> bool {
        return near == 3;
        };

    bool new_state(size_t x, size_t y) const {
        auto near_alive = m_grid.count_alive_near(x, y);
        if (m_grid.get_cell(x, y)) {
            if (KeepAlive(near_alive))
                return 1;
            else
                return 0;
        }
        else {
            if (RaiseDead(near_alive))
                return 1;
            else
                return 0;
        }
    }

public:
    CellMachine(size_t width, size_t height) : m_grid(width, height) {}
    void
        set_default(const std::vector<std::pair<size_t, size_t>>& alive_cells) {
        for (const auto& [x, y] : alive_cells) {
            m_grid.get_cell(x, y) = 1;
        }
    }
    template <typename F> void setKeepAlive(F&& func) {
        KeepAlive = std::forward<F>(func);
    }
    template <typename F> void setRaiseDead(F&& func) {
        RaiseDead = std::forward<F>(func);
    }
    void update() {
        Grid tmp_grid(m_grid.width(), m_grid.height());
        for (size_t x = 0; x < m_grid.width(); x++) {
            for (size_t y = 0; y < m_grid.height(); y++) {
                tmp_grid.get_cell(x, y) = new_state(x, y);
            }
        }
        m_grid = std::move(tmp_grid);
    }
    const Grid& get_grid() const { return m_grid; }
};
