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
    return c;
}

Command Unload(int drone_id, int ware_house_id, int product_id, int product_count) {
    Command c;
    c.type = 'U';
    c.drone_id = drone_id;
    c.ware_house_id = ware_house_id;
    c.product_id = product_id;
    c.product_count = product_count;
    return c;
}

Command Deliver(int drone_id, int customer_id, int product_id, int product_count) {
    Command c;
    c.type = 'D';
    c.drone_id = drone_id;
    c.customer_id = customer_id;
    c.product_id = product_id;
    c.product_count = product_count;
    return c;
}

Command Wait(int drone_id, int turns) {
    Command c;
    c.type = 'W';
    c.drone_id = drone_id;
    c.turns = turns;
    return c;
}

struct Warehouse {
    int r;
    int c;
    std::vector<int> product_counts;
};

struct Order {
    int r;
    int c;
    std::vector<int> products;
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
        int pc;
        in >> pc;
        s.orders[i].products.resize(pc);
        for (int j = 0; j < pc; ++j) {
            in >> s.orders[i].products[j];
        }
    }

    return s;
}

std::vector<Command> doIt(Simulation s) {
    std::vector<Command> commands;
    commands.push_back(Load(0, 0, 0, 1));
    return commands;
}

int main() {
    auto commands = doIt(read());

    for (const auto& c : commands) {
        std::cout << c << '\n';
    }

    std::cout << std::flush;
}
