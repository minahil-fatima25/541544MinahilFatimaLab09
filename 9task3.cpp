//Task 3: Weighted M-ary Tree – Galaxy Representation Problem
#include <iostream>
#include <string>
#include <limits>
using namespace std;

//keeps asking until user enters a proper integer
int readInt(const char* msg) {
    int val;
    cout << msg;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, enter a number: ";
    }
    return val;
}

//same thing but for decimal numbers
float readFloat(const char* msg) {
    float val;
    cout << msg;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input, enter a number: ";
    }
    return val;
}

class StarSystem;

//edge between two star systems, has the travel cost
class TravelRoute {
public:
    float travel_cost;
    StarSystem* destination;

    TravelRoute() {
        travel_cost = 0;
        destination = nullptr;
    }

    TravelRoute(float cost, StarSystem* dest) {
        travel_cost = cost;
        destination = dest;
    }
};

//a node of the galaxy tree
class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    float x, y, z;
    bool is_habitable;
    TravelRoute* routes;  //dynamic array of size M, made when first route added
    int num_routes;       //how many routes are actually used

    StarSystem(int id, string n, int planets, float px, float py, float pz, bool habitable) {
        system_id = id;
        name = n;
        num_planets = planets;
        x = px;
        y = py;
        z = pz;
        is_habitable = habitable;
        routes = nullptr;
        num_routes = 0;
    }

    ~StarSystem() {
        delete[] routes;
    }
};

//manages the whole m-ary tree
class Galaxy {
private:
    int M;              //max children per node
    StarSystem* root;

    //returns subtree size and also sets ok false if some node is unbalanced
    //doing both in one pass so sizes are not calculated again and again
    int check_balance(StarSystem* x, bool& ok) const {
        if (x == nullptr)
            return 0;

        int total = 1;
        int minSize = 0, maxSize = 0;
        bool first = true;

        for (int i = 0; i < x->num_routes; i++) {
            int s = check_balance(x->routes[i].destination, ok);
            total += s;
            if (first) {
                minSize = s;
                maxSize = s;
                first = false;
            }
            else {
                if (s < minSize) minSize = s;
                if (s > maxSize) maxSize = s;
            }
        }

        //in m-ary tree empty child slots count as subtree of size 0
        if (x->num_routes < M && x->num_routes > 0)
            minSize = 0;

        if (maxSize - minSize > 1)
            ok = false;

        return total;
    }

    //prints the tree with indentation to show levels
    void print(StarSystem* x, int level, float cost) const {
        if (x == nullptr)
            return;
        for (int i = 0; i < level; i++)
            cout << "    ";
        cout << "[" << x->system_id << "] " << x->name
             << " | planets: " << x->num_planets
             << " | pos: (" << x->x << ", " << x->y << ", " << x->z << ")"
             << " | habitable: " << (x->is_habitable ? "yes" : "no");
        if (level > 0)
            cout << " | cost: " << cost;
        cout << endl;
        for (int i = 0; i < x->num_routes; i++)
            print(x->routes[i].destination, level + 1, x->routes[i].travel_cost);
    }

    //searches whole tree for a system with given id
    StarSystem* find(StarSystem* x, int id) const {
        if (x == nullptr)
            return nullptr;
        if (x->system_id == id)
            return x;
        for (int i = 0; i < x->num_routes; i++) {
            StarSystem* found = find(x->routes[i].destination, id);
            if (found != nullptr)
                return found;
        }
        return nullptr;
    }

    void destroy(StarSystem* x) {
        if (x == nullptr)
            return;
        for (int i = 0; i < x->num_routes; i++)
            destroy(x->routes[i].destination);
        delete x;
    }

public:
    //default constructor, binary by default with empty tree
    Galaxy() {
        M = 2;
        root = nullptr;
    }

    //overloaded constructor with M and optional root
    Galaxy(int m, StarSystem* r = nullptr) {
        M = m;
        root = r;
    }

    ~Galaxy() {
        destroy(root);
    }

    int getM() const {
        return M;
    }

    StarSystem* find_by_id(int id) const {
        return find(root, id);
    }

    StarSystem* getRoot() const {
        return root;
    }

    void setRoot(StarSystem* r) {
        root = r;
    }

    //connects child to parent with a cost, wont allow more than M children
    bool add_route(StarSystem* parent, StarSystem* child, float cost) {
        if (parent == nullptr || child == nullptr)
            return false;
        if (parent->num_routes >= M) {
            cout << "Cannot add " << child->name << ", " << parent->name
                 << " already has " << M << " routes" << endl;
            return false;
        }
        //first child so make the array with M slots
        if (parent->routes == nullptr)
            parent->routes = new TravelRoute[M];
        parent->routes[parent->num_routes] = TravelRoute(cost, child);
        parent->num_routes++;
        return true;
    }

    //balanced if at every node child subtree sizes differ by at most 1
    bool is_balanced(StarSystem* x) const {
        bool ok = true;
        check_balance(x, ok);
        return ok;
    }

    //leaf means no routes going out of it
    int count_leaves(StarSystem* x) const {
        if (x == nullptr)
            return 0;
        if (x->num_routes == 0)
            return 1;
        int count = 0;
        for (int i = 0; i < x->num_routes; i++)
            count += count_leaves(x->routes[i].destination);
        return count;
    }

    void display() const {
        cout << "Galaxy (M = " << M << "):" << endl;
        if (root == nullptr) {
            cout << "Galaxy is empty" << endl;
            return;
        }
        print(root, 0, 0);
    }
};

//takes all details of a star system from user
StarSystem* inputStarSystem(const Galaxy& g) {
    int id = readInt("Enter system ID: ");
    while (g.find_by_id(id) != nullptr)
        id = readInt("ID already exists, enter another ID: ");

    string name;
    cout << "Enter name: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, name);

    int planets = readInt("Enter number of planets: ");
    while (planets < 0)
        planets = readInt("Planets cant be negative, enter again: ");

    float x = readFloat("Enter x coordinate: ");
    float y = readFloat("Enter y coordinate: ");
    float z = readFloat("Enter z coordinate: ");

    int hab = readInt("Is it habitable? (1 = yes, 0 = no): ");
    while (hab != 0 && hab != 1)
        hab = readInt("Enter 1 or 0 only: ");

    return new StarSystem(id, name, planets, x, y, z, hab == 1);
}

int main() {
    cout << "===== Task 3: Weighted M-ary Tree - Galaxy =====" << endl << endl;

    int m = readInt("Enter M (max routes from each star system): ");
    while (m <= 0)
        m = readInt("M must be positive, enter again: ");

    //using the overloaded constructor, root is added after taking its details
    Galaxy galaxy(m);
    cout << endl << "Enter details of the root star system" << endl;
    galaxy.setRoot(inputStarSystem(galaxy));

    int choice;
    do {
        cout << endl << "----- Menu -----" << endl;
        cout << "1. Add Star System" << endl;
        cout << "2. Display Galaxy" << endl;
        cout << "3. Check if Balanced" << endl;
        cout << "4. Count Leaf Nodes" << endl;
        cout << "5. Exit" << endl;
        choice = readInt("Enter choice: ");
        cout << endl;

        if (choice == 1) {
            int pid = readInt("Enter parent system ID: ");
            StarSystem* parent = galaxy.find_by_id(pid);
            //checking parent first so user doesnt type all details for nothing
            if (parent == nullptr) {
                cout << "No system with ID " << pid << endl;
            }
            else if (parent->num_routes >= galaxy.getM()) {
                cout << parent->name << " already has " << galaxy.getM() << " routes" << endl;
            }
            else {
                float cost = readFloat("Enter travel cost: ");
                while (cost < 0)
                    cost = readFloat("Cost cant be negative, enter again: ");
                StarSystem* child = inputStarSystem(galaxy);
                galaxy.add_route(parent, child, cost);
                cout << child->name << " added under " << parent->name << endl;
            }
        }
        else if (choice == 2) {
            galaxy.display();
        }
        else if (choice == 3 || choice == 4) {
            int id = readInt("Enter system ID to check from (root ID for whole galaxy): ");
            StarSystem* node = galaxy.find_by_id(id);
            if (node == nullptr) {
                cout << "No system with ID " << id << endl;
            }
            else if (choice == 3) {
                cout << "Subtree of " << node->name << " is balanced: "
                     << (galaxy.is_balanced(node) ? "Yes" : "No") << endl;
            }
            else {
                cout << "Leaf nodes in subtree of " << node->name << ": "
                     << galaxy.count_leaves(node) << endl;
            }
        }
        else if (choice != 5) {
            cout << "Invalid choice" << endl;
        }
    } while (choice != 5);

    cout << "Exiting..." << endl;
    return 0;
}