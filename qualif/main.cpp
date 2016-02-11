#include "stuff.h"

struct Command {
    int drone_id;
    char type; // 'L' = Load; 'U' = Unload; 'D' = Deliver; 'W' = Wait;

    // load/unload
    int ware_house_id;
    int product_id;
    int product_count;

    // deliver
    int customer_id;

    // wait
    int turns;
};

std::ostream& operator<<(std::ostream& os, const Command& c) {
    os << c.drone_id << ' ' << c.type << ' ';
    switch (c.type) {
        case 'L':
        case 'U':
            os << c.ware_house_id << ' ' << c.product_id << ' ' << c.product_count;
            break;
        case 'D':
            os << c.customer_id << ' ' << c.product_id << ' ' << c.product_count;
            break;
        case 'W':
            os << c.turns;
            break;
        default:
            assert(false);
            break;
    }
    return os;
}

Command Load(int drone_id, int ware_house_id, int product_id, int product_count) {
    Command c;
    c.type = 'L';
    c.drone_id = drone_id;
    c.ware_house_id = ware_house_id;
    c.product_id = product_id;
    c.product_count = product_count;
    std::cerr << c << std::endl;
    return c;
}

Command Unload(int drone_id, int ware_house_id, int product_id, int product_count) {
    Command c;
    c.type = 'U';
    c.drone_id = drone_id;
    c.ware_house_id = ware_house_id;
    c.product_id = product_id;
    c.product_count = product_count;
    std::cerr << c << std::endl;
    return c;
}

Command Deliver(int drone_id, int customer_id, int product_id, int product_count) {
    Command c;
    c.type = 'D';
    c.drone_id = drone_id;
    c.customer_id = customer_id;
    c.product_id = product_id;
    c.product_count = product_count;
    std::cerr << c << std::endl;
    return c;
}

Command Wait(int drone_id, int turns) {
    Command c;
    c.type = 'W';
    c.drone_id = drone_id;
    c.turns = turns;
    std::cerr << c << std::endl;
    return c;
}

struct Warehouse {
    int r;
    int c;
    std::vector<int> product_counts;
};

struct Order {
    int index;

    int r;
    int c;
    std::vector<int> products;




    long long score;
};

struct Simulation {
    int rows;
    int columns;
    int drone_count;
    int simulation_deadline;
    int max_drone_load;

    std::vector<int> product_weights;
    std::vector<Warehouse> warehouses;
    std::vector<Order> orders;
};

template<class T, class U>
int distance(const T& t, const U& u) {
    int dr = t.r - u.r;
    int dc = t.c - u.c;
    return std::ceil(std::sqrt(float(dr * dr + dc * dc)));
}

int getWeigth(const Order& o, const Simulation& s) {
    int w = 0;
    for (int p : o.products) {
        w += s.product_weights[p];
    }
    return w;
}

Simulation read(std::istream& in = std::cin) {
    Simulation s;
    in >> s.rows >> s.columns >> s.drone_count >> s.simulation_deadline >> s.max_drone_load;

    int product_count;
    in >> product_count;
    s.product_weights.resize(product_count);
    for (int i = 0; i < product_count; ++i) {
        in >> s.product_weights[i];
    }

    int warehouse_count;
    in >> warehouse_count;
    s.warehouses.resize(warehouse_count);
    for (int i = 0; i < warehouse_count; ++i) {
        in >> s.warehouses[i].r >> s.warehouses[i].c;
        s.warehouses[i].product_counts.resize(product_count);
        for (int j = 0; j < product_count; ++j) {
            in >> s.warehouses[i].product_counts[j];
        }
    }

    int order_count;
    in >> order_count;
    s.orders.resize(order_count);
    for (int i = 0; i < order_count; ++i) {
        in >> s.orders[i].r >> s.orders[i].c;
        s.orders[i].index = i;
        int pc;
        in >> pc;
        s.orders[i].products.resize(pc);
        for (int j = 0; j < pc; ++j) {
            in >> s.orders[i].products[j];
        }
        std::sort(s.orders[i].products.begin(), s.orders[i].products.end(),
            [&s](auto l, auto r) {
                if (s.product_weights[l] !=
                    s.product_weights[r])
                {
                    return
                        s.product_weights[l] >
                        s.product_weights[r];
                }
                return l < r;
            }
        );
    }

    return s;
}

std::vector<Command> moho(Simulation s) {
    Warehouse& w = s.warehouses[0];
    for (Order& o : s.orders) {
        o.score = ((long long)(distance(w, o)) << 32) + getWeigth(o, s);
    }

    std::sort(s.orders.begin(), s.orders.end(),
        [](const auto& l, const auto& r) {
            return l.score < r.score;
        }
    );

    std::multimap<int, int> drones;
    for (int i = 0; i < s.drone_count; ++i) {
        drones.insert(std::make_pair(0, i));
    }

    std::vector<Command> commands;
    for (const Order& o : s.orders) {
        auto ps = o.products;

        while (!ps.empty()) {
            auto drone = *drones.begin();
            drones.erase(drones.begin());

            std::vector<Command> deliver_commands;

            int leftover_capacity = s.max_drone_load;
            for (int i = 0; i < ps.size(); ++i) {
                if (leftover_capacity >= s.product_weights[ps[i]]) {
                    commands.push_back(Load(drone.second, 0, ps[i], 1));
                    deliver_commands.push_back(Deliver(drone.second, o.index, ps[i], 1));
                    leftover_capacity -= s.product_weights[ps[i]];
                    ps.erase(ps.begin() + i);
                    --i;
                }
            }
            commands.insert(commands.end(), deliver_commands.begin(), deliver_commands.end());
            drones.insert(std::make_pair(drone.first + 2*distance(w, o), drone.second));
        }
    }

    return commands;
}

std::vector<Command> doIt(Simulation s) {
    if (s.warehouses.size() == 1) {
        return moho(s);
    }

    std::vector<Command> commands;
    commands.push_back(Load(0, 0, 0, 1));
    return commands;
}

int main() {
    auto commands = doIt(read());

    std::cout << commands.size() << '\n';
    for (const auto& c : commands) {
        std::cout << c << '\n';
    }

    std::cout << std::flush;
}
