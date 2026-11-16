#define NOMINMAX
#include <windows.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

//colors code
#define WHITE         "\033[97m"
#define MIDNIGHT_BLUE "\033[38;5;17m"
#define MUTED_ROSE  "\033[38;5;181m"
#define LIGHT_WINE  "\033[38;5;132m"
#define DEEP_WINE   "\033[38;5;88m"
#define DUSTY_ROSE  "\033[38;5;175m"
#define GOLD        "\033[38;5;220m"
#define MUTED_RED   "\033[38;5;167m"
#define BRONZE      "\033[38;5;137m"
#define SILVER      "\033[38;5;250m"
#define EMERALD     "\033[38;5;29m"
#define PALE_GOLD   "\033[38;5;179m"
#define BURGUNDY    "\033[38;5;131m"
#define CHARCOAL    "\033[38;5;245m"
#define PLATINUM    "\033[38;5;146m"
#define CHAMPAGNE       "\033[38;5;223m"
#define CHAMPAGNE_BEIGE "\033[38;5;180m"
#define BOLD        "\033[1m"
#define RESET       "\033[0m"

struct Product {
    int id; string name; string category; string brand; double price; int stockQuantity;
};
int readInt(const string& prompt, int minValue, int maxValue) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue && value <= maxValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); return value;
        }
        cout << MUTED_RED BOLD  "Invalid input. Enter a whole number from " << minValue
            << " to " << maxValue << ".\n" RESET;
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
double readDouble(const string& prompt, double minValue, double maxValue) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue && value <= maxValue) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); return value;
        }
        cout << MUTED_RED << BOLD "Invalid price. Please enter a value from " << fixed
            << setprecision(2) << minValue << " to " << maxValue << ".\n" << RESET;
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}
string readLine(const string& prompt) {
    string value;
    while (true) {
        cout << prompt; getline(cin, value);
        if (!value.empty()) return value;
        cout << MUTED_RED << BOLD "Input cannot be empty. Try again.\n" << RESET;
    }
}
string lower(string value) {
    transform(value.begin(), value.end(), value.begin(),
        [](unsigned char c) { return static_cast<char>(tolower(c)); });
    return value;
}
void clearScreen() {
    system("cls"); // Clears the terminal on Windows.
}
void pauseScreen() {
    cout << CHARCOAL << "\nPress Enter to return..." << RESET; cin.get();
}
void printHeader() {
    cout << "\n" << GOLD
        << "==========================================================================\n" << BOLD << "                             LUMIERE LUXE\n"
        << WHITE << "                    LUXURY LIGHTING & CHANDELIERS\n" << GOLD << "==========================================================================\n" << RESET;
}
void printTableHeader() {
    cout << CHAMPAGNE << left << setw(7) << "ID" << setw(25) << "PRODUCT"
        << setw(19) << "CATEGORY" << setw(14) << "BRAND" << right << setw(15) << "PRICE (PHP)" << setw(9) << "STOCK\n" << RESET;
    cout << GOLD << string(89, '-') << RESET << '\n';
}
void printProduct(const Product& p) {
    cout << left << setw(7) << p.id << setw(25) << p.name.substr(0, 24) << setw(19) << p.category.substr(0, 18) << setw(14) << p.brand.substr(0, 13)
        << right << setw(15) << fixed << setprecision(2) << p.price << setw(9) << p.stockQuantity << '\n';
}

// LAB 1: Dynamic memory allocation and resizing of the product catalog.
class Inventory {
    Product* data; int count; int capacity;
    void resize() {
        int newCapacity = capacity * 2; Product* bigger = new Product[newCapacity];
        for (int i = 0; i < count; ++i) bigger[i] = data[i];
        delete[] data; data = bigger; capacity = newCapacity; cout << EMERALD BOLD "[Inventory resized to " << capacity << " product slots.]\n" RESET;
    }
public:
    Inventory() : data(new Product[8]), count(0), capacity(8) {}
    ~Inventory() { delete[] data; }
    Inventory(const Inventory&) = delete; Inventory& operator=(const Inventory&) = delete;
    int size() const { return count; }
    Product* items() { return data; }
    const Product* items() const { return data; }
    Product* find(int id) {
        for (int i = 0; i < count; ++i)
            if (data[i].id == id) return &data[i];
        return nullptr;
    }
    const Product* find(int id) const {
        for (int i = 0; i < count; ++i)
            if (data[i].id == id) return &data[i];
        return nullptr;
    }
    bool add(const Product& p) {
        if (find(p.id)) return false;
        if (count == capacity) resize();
        data[count++] = p; return true;
    }
    bool remove(int id, Product& removed) {
        for (int i = 0; i < count; ++i) {
            if (data[i].id == id) {
                removed = data[i];
                for (int j = i; j < count - 1; ++j)
                    data[j] = data[j + 1];
                --count; return true;
            }
        }
        return false;
    }
    void display() const {
        if (!count) { cout << MIDNIGHT_BLUE BOLD "\nNo products in the catalog.\n" RESET; return; }
        cout << BRONZE << BOLD << "COMPLETE PRODUCT CATALOG" << RESET << endl; printTableHeader();
        for (int i = 0; i < count; ++i) printProduct(data[i]);
        cout << GOLD << string(89, '-') << RESET << PALE_GOLD << "\nTotal products: " << RESET << PALE_GOLD << BOLD << count << RESET << '\n';
    }
};

// LAB 2: Doubly linked list for shopping-cart navigation.
struct CartNode {
    int id, quantity; string name, brand; double price; CartNode* prev, * next; CartNode(const Product& p, int q)
        : id(p.id), quantity(q), name(p.name), brand(p.brand), price(p.price),
        prev(nullptr), next(nullptr) {
    }
};
struct OrderItem {
    int id, quantity; string name, brand; double price;
};
class Cart {
    CartNode* head; CartNode* tail;
public:
    Cart() : head(nullptr), tail(nullptr) {}
    ~Cart() { clear(); }
    Cart(const Cart&) = delete; Cart& operator=(const Cart&) = delete;
    bool empty() const { return head == nullptr; }
    CartNode* find(int id) {
        for (CartNode* p = head; p; p = p->next) if (p->id == id) return p;
        return nullptr;
    }
    int quantityFor(int id) {
        CartNode* p = find(id); return p ? p->quantity : 0;
    }
    void add(const Product& p, int q) {
        CartNode* existing = find(p.id);
        if (existing) existing->quantity += q;
        else {
            CartNode* node = new CartNode(p, q);
            if (!head) head = tail = node;
            else { tail->next = node; node->prev = tail; tail = node; }
        }
        cout << EMERALD BOLD "Product added to cart.\n" RESET;
    }
    bool remove(int id) {
        CartNode* p = find(id);
        if (!p) return false;
        if (p->prev) p->prev->next = p->next; else head = p->next;
        if (p->next) p->next->prev = p->prev; else tail = p->prev;
        delete p; return true;
    }
    void displayForward() const {
        if (!head) { cout << MIDNIGHT_BLUE BOLD "\nYour shopping cart is empty.\n" RESET; return; }
        cout << BRONZE BOLD "\nSHOPPING CART (FORWARD)\n" RESET; cout << left << setw(8) << "ID" << setw(25) << "PRODUCT"
            << setw(15) << "BRAND" << setw(8) << "QTY" << "SUBTOTAL\n";
        double total = 0;
        for (CartNode* p = head; p; p = p->next) {
            double sub = p->price * p->quantity; total += sub; cout << left << setw(8) << p->id << setw(25) << p->name.substr(0, 24)
                << setw(15) << p->brand << setw(8) << p->quantity << fixed << setprecision(2) << sub << '\n';
        }
        cout << SILVER BOLD "Cart total: PHP " RESET << fixed << setprecision(2) << total << '\n';
    }
    void displayBackward() const {
        if (!tail) { cout << MIDNIGHT_BLUE BOLD "\nYour shopping cart is empty.\n" RESET; return; }
        cout << BRONZE BOLD "\nCART NAVIGATION (BACKWARD)\n" RESET;
        for (CartNode* p = tail; p; p = p->prev)
            cout << p->id << " - " << p->name << " (" << p->brand << ")\n";
    }
    double total() const {
        double sum = 0;
        for (CartNode* p = head; p; p = p->next) sum += p->price * p->quantity;
        return sum;
    }
    bool stockAvailable(const Inventory& inv) const {
        for (CartNode* p = head; p; p = p->next) {
            const Product* product = inv.find(p->id);
            if (!product || product->stockQuantity < p->quantity) return false;
        }
        return true;
    }
    void deductStock(Inventory& inv) const {
        for (CartNode* p = head; p; p = p->next) {
            Product* product = inv.find(p->id);
            if (product) product->stockQuantity -= p->quantity;
        }
    }
    OrderItem* makeOrderItems(int& n) const {
        n = 0;
        for (CartNode* p = head; p; p = p->next) ++n;
        if (!n) return nullptr;
        OrderItem* result = new OrderItem[n]; int i = 0;
        for (CartNode* p = head; p; p = p->next)
            result[i++] = { p->id, p->quantity, p->name, p->brand, p->price };
        return result;
    }
    void clear() {
        while (head) {
            CartNode* next = head->next; delete head; head = next;
        }
        tail = nullptr;
    }
};

// LAB 5: FIFO queue for customer checkout orders.
struct Order {
    int number, itemCount; string customer; OrderItem* items; double total; Order* next; Order(int no, const string& name, OrderItem* list, int count, double amount)
        : number(no), itemCount(count), customer(name), items(list), total(amount), next(nullptr) {
    }
    ~Order() { delete[] items; }
};
class OrderQueue {
    Order* front; Order* rear; int nextNumber;
public:
    OrderQueue() : front(nullptr), rear(nullptr), nextNumber(1001) {}
    ~OrderQueue() {
        while (front) { Order* temp = front; front = front->next; delete temp; }
    }
    bool empty() const { return front == nullptr; }
    void enqueue(const string& customer, OrderItem* items, int count, double total) {
        Order* node = new Order(nextNumber++, customer, items, count, total);
        if (!rear) front = rear = node;
        else { rear->next = node; rear = node; }
        cout << "Order #" << node->number << EMERALD BOLD " added to FIFO checkout queue.\n" RESET;
    }
    void processAll() {
        if (empty()) { cout << MIDNIGHT_BLUE BOLD "\nNo pending orders.\n" RESET; return; }
        cout << BRONZE BOLD "\nFIFO CHECKOUT PROCESSING\n" RESET;
        while (front) {
            Order* current = front; cout << BURGUNDY BOLD "\nProcessing Order #" RESET << current->number
                << " | Customer: " << current->customer << '\n';
            for (int i = 0; i < current->itemCount; ++i)
                cout << " - " << current->items[i].name << "x" << current->items[i].quantity << " (" << current->items[i].brand << ")\n";
            cout << "Total: PHP " << fixed << setprecision(2) << current->total
                << EMERALD BOLD "\nStatus: PROCESSED\n" RESET;
            front = front->next;
            if (!front) rear = nullptr;
            delete current;
        }
        cout << EMERALD BOLD  "\nAll pending orders processed in FIFO order.\n" RESET;
    }
};

// LAB 4: Linked stack (LIFO) for admin undo actions.
enum class ActionType {
    PRICE, STOCK, ADD, DELETE_PRODUCT
};
struct AdminAction {
    ActionType type; int id; double oldValue; Product product;
};
struct StackNode {
    AdminAction action; StackNode* next;
    StackNode(const AdminAction& a, StackNode* n) : action(a), next(n) {}
};
class UndoStack {
    StackNode* topNode;
public:
    UndoStack() : topNode(nullptr) {}
    ~UndoStack() { while (topNode) { StackNode* t = topNode; topNode = topNode->next; delete t; } }
    void push(const AdminAction& a) { topNode = new StackNode(a, topNode); }
    bool pop(AdminAction& a) {
        if (!topNode) return false;
        StackNode* t = topNode; a = t->action; topNode = topNode->next; delete t; return true;
    }
};

// LAB 6: Binary Search Tree indexes product IDs.
struct BSTNode {
    int id; BSTNode* left, * right;
    explicit BSTNode(int value) : id(value), left(nullptr), right(nullptr) {}
};
class ProductBST {
    BSTNode* root;
    BSTNode* insertNode(BSTNode* node, int id) {
        if (!node) return new BSTNode(id);
        if (id < node->id) node->left = insertNode(node->left, id);
        else if (id > node->id) node->right = insertNode(node->right, id);
        return node;
    }
    bool searchNode(BSTNode* node, int id) const {
        if (!node) return false;
        if (id == node->id) return true;
        return id < node->id ? searchNode(node->left, id) : searchNode(node->right, id);
    }
    BSTNode* minimumNode(BSTNode* node) {
        while (node && node->left) node = node->left;
        return node;
    }
    BSTNode* removeNode(BSTNode* node, int id) {
        if (!node) return nullptr;
        if (id < node->id) node->left = removeNode(node->left, id);
        else if (id > node->id) node->right = removeNode(node->right, id);
        else {
            if (!node->left) {
                BSTNode* rightChild = node->right; delete node; return rightChild;
            }
            if (!node->right) {
                BSTNode* leftChild = node->left; delete node; return leftChild;
            }
            BSTNode* successor = minimumNode(node->right); node->id = successor->id; node->right = removeNode(node->right, successor->id);
        }
        return node;
    }
    void inOrder(BSTNode* node) const {
        if (!node) return;
        inOrder(node->left); cout << node->id << ' '; inOrder(node->right);
    }
    void destroy(BSTNode* node) {
        if (!node) return;
        destroy(node->left); destroy(node->right); delete node;
    }
public:
    ProductBST() : root(nullptr) {}
    ~ProductBST() { destroy(root); }
    void insert(int id) { root = insertNode(root, id); }
    void remove(int id) { root = removeNode(root, id); }
    bool search(int id) const { return searchNode(root, id); }
    void displayIDs() const { cout << "BST product IDs (ascending): "; inOrder(root); cout << '\n'; }
};

// LAB 3: Circular linked list for the four partner brands.
struct BrandNode { string brand; BrandNode* next; explicit BrandNode(const string& b) : brand(b), next(nullptr) {} };
class BrandShowcase {
    BrandNode* tail;
public:
    BrandShowcase() : tail(nullptr) {
        add("Flos"); add("Bocci"); add("Tom Dixon"); add("Lasvit");
    }
    ~BrandShowcase() {
        if (!tail) return;
        BrandNode* current = tail->next; tail->next = nullptr;
        while (current) { BrandNode* next = current->next; delete current; current = next; }
    }
    void add(const string& brand) {
        BrandNode* node = new BrandNode(brand);
        if (!tail) { tail = node; tail->next = tail; }
        else { node->next = tail->next; tail->next = node; tail = node; }
    }
    void display() const {
        if (!tail) return;
        cout << BRONZE << BOLD "\nOFFICIAL PARTNER BRAND SHOWCASE\n" RESET; BrandNode* current = tail->next;
        do { cout << " * " << current->brand << '\n'; current = current->next; } while (current != tail->next);
        cout << "Circular cycle: after Lasvit, the showcase returns to Flos.\n";
    }
};

// Brand filter and LAB 7 linear keyword search.
void filterBrand(const Inventory& inv) {
    const string brands[] = { "Flos", "Bocci", "Tom Dixon", "Lasvit" };
    cout << BRONZE << BOLD  "\nOFFICIAL PARTNER BRANDS\n" << RESET;
    for (int i = 0; i < 4; ++i) cout << i + 1 << ". " << brands[i] << '\n';
    int choice = readInt(PLATINUM "Select brand: " RESET, 1, 4); bool found = false; cout << BURGUNDY << BOLD "\nProducts from " << brands[choice - 1] << RESET ":\n";
    printTableHeader();
    for (int i = 0; i < inv.size(); ++i) {
        if (lower(inv.items()[i].brand) == lower(brands[choice - 1])) {
            printProduct(inv.items()[i]); found = true;
        }
    }
    if (!found) cout << MUTED_ROSE << BOLD "No products found for this brand.\n" RESET;
}
void linearSearch(const Inventory& inv) {
    string keyword = lower(readLine("\n" PLATINUM BOLD "Enter keyword (name, category, or brand): " RESET)); bool found = false;
    cout << BRONZE << BOLD "\nLINEAR SEARCH RESULTS\n" RESET; printTableHeader();
    for (int i = 0; i < inv.size(); ++i) {
        const Product& p = inv.items()[i]; string text = lower(p.name + " " + p.category + " " + p.brand);
        if (text.find(keyword) != string::npos) { printProduct(p); found = true; }
    }
    if (!found) cout << MUTED_ROSE << BOLD "No matching products found.\n" RESET;
}

// Quick Sort helper used to prepare price-ordered data for binary search.
int pricePartition(Product* a, int lowIndex, int highIndex) {
    double pivot = a[highIndex].price; int i = lowIndex - 1;
    for (int j = lowIndex; j < highIndex; ++j)
        if (a[j].price <= pivot) { ++i; swap(a[i], a[j]); }
    swap(a[i + 1], a[highIndex]); return i + 1;
}
void quickSortPrice(Product* a, int lowIndex, int highIndex) {
    if (lowIndex < highIndex) {
        int p = pricePartition(a, lowIndex, highIndex); quickSortPrice(a, lowIndex, p - 1); quickSortPrice(a, p + 1, highIndex);
    }
}

// LAB 7: Two binary searches find the inclusive price interval.
void binaryPriceSearch(const Inventory& inv) {
    int n = inv.size();
    if (!n) { cout << MUTED_ROSE << BOLD "No products available.\n" RESET; return; }
    double minPrice = readDouble("\n" LIGHT_WINE BOLD "Minimum price (PHP): " RESET, 0.0, 1.0e12);
    double maxPrice = readDouble(DEEP_WINE BOLD "Maximum price (PHP): " RESET, minPrice, 1.0e12); Product* sorted = new Product[n];
    for (int i = 0; i < n; ++i) sorted[i] = inv.items()[i];
    quickSortPrice(sorted, 0, n - 1); int low = 0, high = n;
    while (low < high) { // first price >= minimum
        int mid = low + (high - low) / 2;
        if (sorted[mid].price < minPrice) low = mid + 1; else high = mid;
    }
    int first = low; low = 0; high = n;
    while (low < high) { // first price > maximum
        int mid = low + (high - low) / 2;
        if (sorted[mid].price <= maxPrice) low = mid + 1; else high = mid;
    }
    int last = low; cout << BRONZE << BOLD "\nBINARY PRICE SEARCH RESULTS (PHP " << fixed << setprecision(2)
        << minPrice << " - " << maxPrice << ")\n" RESET;
    if (first == last) cout << MUTED_ROSE << BOLD "No products found in this price range.\n" RESET;
    else { printTableHeader(); for (int i = first; i < last; ++i) printProduct(sorted[i]); }
    delete[] sorted;
}

// LAB 8: Insertion Sort by price and brand; Quick Sort by stock.
void insertionSortPrice(Product* a, int n) {
    for (int i = 1; i < n; ++i) {
        Product key = a[i]; int j = i - 1;
        while (j >= 0 && a[j].price > key.price) { a[j + 1] = a[j]; --j; }
        a[j + 1] = key;
    }
}
void insertionSortBrand(Product* a, int n) {
    for (int i = 1; i < n; ++i) {
        Product key = a[i]; int j = i - 1;
        while (j >= 0 && lower(a[j].brand) > lower(key.brand)) { a[j + 1] = a[j]; --j; }
        a[j + 1] = key;
    }
}
void quickSortStock(Product* a, int lowIndex, int highIndex) {
    int i = lowIndex, j = highIndex; int pivot = a[lowIndex + (highIndex - lowIndex) / 2].stockQuantity;
    while (i <= j) {
        while (a[i].stockQuantity < pivot) ++i;
        while (a[j].stockQuantity > pivot) --j;
        if (i <= j) { swap(a[i], a[j]); ++i; --j; }
    }
    if (lowIndex < j) quickSortStock(a, lowIndex, j);
    if (i < highIndex) quickSortStock(a, i, highIndex);
}
void sortInventory(const Inventory& inv) {
    int n = inv.size();
    if (!n) { cout << MUTED_ROSE << BOLD "No products to sort.\n" RESET; return; }
    cout << "\nSORT INVENTORY\n1. By Price (Insertion Sort)\n2. By Stock (Quick Sort)\n3. By Brand (Insertion Sort)\n"; int choice = readInt("Choose sorting method: ", 1, 3);
    Product* sorted = new Product[n];
    for (int i = 0; i < n; ++i) sorted[i] = inv.items()[i];
    if (choice == 1) { insertionSortPrice(sorted, n); cout << "\nSorted by price (low to high):\n"; }
    else if (choice == 2) { quickSortStock(sorted, 0, n - 1); cout << "\nSorted by stock (low to high):\n"; }
    else { insertionSortBrand(sorted, n); cout << "\nSorted alphabetically by brand:\n"; }
    printTableHeader();
    for (int i = 0; i < n; ++i) printProduct(sorted[i]);
    delete[] sorted;
}
void cartMenu(Inventory& inv, Cart& cart) {
    while (true) {
        clearScreen(); cout << BRONZE BOLD "\nSHOPPING CART\n" RESET
            << "1. View Catalog\n" << "2. Add Product\n" << "3. Remove Product\n" << "4. View Cart Forward\n" << "5. View Cart Backward\n" << "6. Return\n";
        int choice = readInt(PLATINUM BOLD "Enter choice: " RESET, 1, 6); clearScreen();
        if (choice == 1) {
            inv.display(); pauseScreen();
        }
        else if (choice == 2) {
            inv.display(); int id = readInt(CHAMPAGNE_BEIGE "Enter product ID: " RESET, 1, 1000000000); Product* p = inv.find(id);
            if (!p) {
                cout << MUTED_ROSE BOLD "Product not found.\n" RESET; pauseScreen(); continue;
            }
            int available = p->stockQuantity - cart.quantityFor(id);
            if (available <= 0) {
                cout << MUTED_ROSE BOLD "No additional stock is available for this cart.\n" RESET; pauseScreen(); continue;
            }
            int qty = readInt(CHAMPAGNE_BEIGE "Enter quantity: " RESET, 1, available); cart.add(*p, qty); pauseScreen();
        }
        else if (choice == 3) {
            if (cart.empty()) {
                cout << SILVER BOLD "Cart is empty.\n" RESET; pauseScreen(); continue;
            }
            cart.displayForward(); int id = readInt(CHAMPAGNE_BEIGE "Enter product ID to remove: " RESET, 1, 1000000000);
            cout << (cart.remove(id) ? EMERALD BOLD "Product removed.\n" RESET
                : BURGUNDY BOLD "Product is not in the cart.\n" RESET);
            pauseScreen();
        }
        else if (choice == 4) {
            cart.displayForward(); pauseScreen();
        }
        else if (choice == 5) {
            cart.displayBackward(); pauseScreen();
        }
        else {
            break;
        }
    }
}
void checkout(Cart& cart, Inventory& inv, OrderQueue& queue) {
    if (cart.empty()) { cout << SILVER BOLD "\nCart is empty. Add products before checkout.\n" RESET; return; }
    cart.displayForward();
    if (!cart.stockAvailable(inv)) { cout << SILVER BOLD "Insufficient stock. Checkout cancelled.\n" RESET; return; }
    string customer = readLine(CHAMPAGNE_BEIGE "Enter customer name: " RESET); int count = 0; OrderItem* items = cart.makeOrderItems(count); double amount = cart.total();
    cart.deductStock(inv); queue.enqueue(customer, items, count, amount); cart.clear(); cout << EMERALD BOLD "Stock updated and cart cleared.\n" RESET;
    int process = readInt(PLATINUM BOLD "Process pending queue now? (1=Yes, 2=Later): " RESET, 1, 2);
    if (process == 1) queue.processAll();
    else cout << MIDNIGHT_BLUE BOLD "Order is waiting in the FIFO queue.\n" RESET;
}
void adminMenu(Inventory& inv, UndoStack& undo, ProductBST& tree) {
    while (true) {
        clearScreen(); cout << BRONZE BOLD "\nADMIN INVENTORY MANAGEMENT\n" RESET
            << "1. Update Product Price\n" << "2. Update Product Stock\n" << "3. Undo Last Admin Action (LIFO)\n" << "4. Add Product\n" << "5. Delete Product\n" << "6. Return\n";
        int choice = readInt(PLATINUM "Enter choice: " RESET, 1, 6); clearScreen();
        if (choice == 6) return;
        if (choice == 3) {
            AdminAction action;
            if (!undo.pop(action)) {
                cout << MUTED_ROSE BOLD "No admin action is available to undo.\n" RESET; pauseScreen(); continue;
            }
            if (action.type == ActionType::PRICE || action.type == ActionType::STOCK) {
                Product* p = inv.find(action.id);
                if (!p) {
                    cout << MUTED_ROSE BOLD "Product no longer exists; action cannot be undone.\n" RESET;
                }
                else if (action.type == ActionType::PRICE) {
                    p->price = action.oldValue; cout << EMERALD BOLD "Previous price restored.\n" RESET;
                }
                else {
                    p->stockQuantity = static_cast<int>(action.oldValue); cout << EMERALD BOLD "Previous stock restored.\n" RESET;
                }
            }
            else if (action.type == ActionType::ADD) {
                Product removed;
                if (inv.remove(action.id, removed)) {
                    tree.remove(action.id); cout << EMERALD BOLD "Last added product removed successfully.\n" RESET;
                }
                else {
                    cout << MUTED_RED BOLD "Added product could not be found; undo failed.\n" RESET;
                }
            }
            else if (action.type == ActionType::DELETE_PRODUCT) {
                if (inv.add(action.product)) {
                    tree.insert(action.product.id); cout << EMERALD BOLD "Deleted product restored successfully.\n" RESET;
                }
                else {
                    cout << DEEP_WINE BOLD "Product could not be restored because its ID already exists.\n" RESET;
                }
            }
            pauseScreen(); continue;
        }
        if (choice == 4) {
            cout << "\nADD NEW PRODUCT\n"; Product p; p.id = readInt(PLATINUM "Enter product ID: " RESET, 1, 1000000000);
            if (inv.find(p.id)) {
                cout << BURGUNDY BOLD"A product with this ID already exists.\n" RESET; pauseScreen(); continue;
            }
            p.name = readLine(PLATINUM "Enter product name: " RESET); p.category = readLine(PLATINUM "Enter product category: " RESET);
            p.brand = readLine(PLATINUM "Enter product brand: " RESET); p.price = readDouble(PLATINUM "Enter product price (PHP): " RESET, 0.01, 1.0e12);
            p.stockQuantity = readInt(PLATINUM "Enter stock quantity: " RESET, 0, 1000000000);
            if (inv.add(p)) {
                tree.insert(p.id);
                undo.push({ ActionType::ADD, p.id, 0.0, p });
                cout << EMERALD BOLD "\nProduct added successfully!\n" RESET;
            }
            else {
                cout << MUTED_RED BOLD "\nUnable to add product. Please check the product ID.\n" RESET;
            }
            pauseScreen(); continue;
        }
        if (choice == 5) {
            inv.display(); int id = readInt(PLATINUM "\nEnter product ID to delete: " RESET, 1, 1000000000); Product removed;
            if (inv.remove(id, removed)) {
                tree.remove(id);
                undo.push({ ActionType::DELETE_PRODUCT, id, 0.0, removed });
                cout << EMERALD BOLD "Product deleted successfully.\n" RESET;
            }
            else {
                cout << MUTED_ROSE << BOLD "Product ID not found. No product was deleted.\n" RESET;
            }
            pauseScreen(); continue;
        }
        inv.display(); int id = readInt(PLATINUM "Enter product ID: " RESET, 1, 1000000000); Product* p = inv.find(id);
        if (!p) {
            cout << MUTED_ROSE << BOLD "Product ID not found.\n" RESET; pauseScreen(); continue;
        }
        if (choice == 1) {
            double newPrice = readDouble(PLATINUM "Enter new price (PHP): " RESET, 0.01, 1.0e12);
            undo.push({ ActionType::PRICE, id, p->price, *p });
            p->price = newPrice; cout << EMERALD BOLD "Price updated.\n" RESET;
        }
        else if (choice == 2) {
            int newStock = readInt(PLATINUM "Enter new stock quantity: " RESET, 0, 1000000000);
            undo.push({ ActionType::STOCK, id, static_cast<double>(p->stockQuantity), *p });
            p->stockQuantity = newStock; cout << EMERALD BOLD "Stock updated.\n" RESET;
        }
        pauseScreen();
    }
}
void searchByBST(const Inventory& inv, const ProductBST& tree) {
    int id = readInt(PLATINUM"Enter product ID to search in BST: " RESET, 1, 1000000000);
    if (!tree.search(id)) { cout << MUTED_ROSE BOLD "Product ID was not found in the BST.\n" RESET; return; }
    const Product* p = inv.find(id);
    if (p) { cout << EMERALD BOLD "\nBST MATCH\n" RESET; printTableHeader(); printProduct(*p); }
}
void loadProducts(Inventory& inv) {
    // Illustrative project catalog and prices; verify product names before
    // describing them as official manufacturer model names.
    const Product samples[] = {
        {101, "Crystal Radiance", "Chandeliers", "Flos", 185000, 10},
        {102, "Halo Pendant", "Pendant Lights", "Flos", 125000, 8},
        {103, "Luna Table Lamp", "Table Lamps", "Flos", 85000, 12},
        {104, "Arc Floor Lamp", "Floor Lamps", "Flos", 145000, 6},
        {201, "Sculpted Glass", "Chandeliers", "Bocci", 350000, 4},
        {202, "Orb Pendant", "Pendant Lights", "Bocci", 220000, 7},
        {203, "Glass Table Light", "Table Lamps", "Bocci", 115000, 5},
        {204, "Cascade Floor Light", "Floor Lamps", "Bocci", 275000, 3},
        {301, "Melt Chandelier", "Chandeliers", "Tom Dixon", 320000, 3},
        {302, "Melt Pendant", "Pendant Lights", "Tom Dixon", 165000, 9},
        {303, "Bell Table Lamp", "Table Lamps", "Tom Dixon", 85000, 11},
        {304, "Curve Floor Lamp", "Floor Lamps", "Tom Dixon", 195000, 5},
        {401, "Crystal Cascade", "Chandeliers", "Lasvit", 450000, 2},
        {402, "Bohemian Pendant", "Pendant Lights", "Lasvit", 285000, 4},
        {403, "Glass Table Sculpture", "Table Lamps", "Lasvit", 135000, 6},
        {404, "Prism Floor Light", "Floor Lamps", "Lasvit", 225000, 3}
    };
    int n = static_cast<int>(sizeof(samples) / sizeof(samples[0]));
    for (int i = 0; i < n; ++i) inv.add(samples[i]);
}
void mainMenu() {
    printHeader(); cout << " 1. View Complete Catalog\n"
        << " 2. Filter Products by Official Brand\n" << " 3. Search Products by Keyword (Linear Search)\n" << " 4. Filter Products by Price Range (Binary Search)\n"
        << " 5. Add Item to Shopping Cart (Doubly Linked List)\n" << " 6. Process Customer Checkout (FIFO Queue)\n" << " 7. Perform Admin Undo Action (LIFO Stack)\n"
        << " 8. Display Partner Brand Showcase (Circular List)\n" << " 9. Sort Inventory Display\n" << "10. Exit\n" << GOLD
        << "==========================================================================\n" << RESET;
}
void enableColors() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); DWORD mode = 0;
    if (GetConsoleMode(hConsole, &mode)) {
        SetConsoleMode(
            hConsole,
            mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING
        );
    }
}
int main() {
    enableColors(); Inventory inventory; Cart cart; OrderQueue checkoutQueue; UndoStack undoStack; BrandShowcase showcase; ProductBST productTree;
    loadProducts(inventory); // 16 entries also demonstrate dynamic resizing.
    for (int i = 0; i < inventory.size(); ++i) productTree.insert(inventory.items()[i].id);
    int choice;
    do {
        clearScreen(); mainMenu(); choice = readInt("Enter your choice (1-10): ", 1, 10);
        clearScreen(); // Show only the selected feature on the terminal.
        switch (choice) {
        case 1: inventory.display(); pauseScreen(); break;
        case 2: filterBrand(inventory); pauseScreen(); break;
        case 3:
            linearSearch(inventory);
            {
                int useBST = readInt("\n" DUSTY_ROSE BOLD
                    "Also search a product ID using the BST index? (1=Yes, 2=No): " RESET, 1, 2);
                if (useBST == 1) searchByBST(inventory, productTree);
            }
            pauseScreen(); break;
        case 4: binaryPriceSearch(inventory); pauseScreen(); break;
        case 5: cartMenu(inventory, cart); break;
        case 6: checkout(cart, inventory, checkoutQueue); pauseScreen(); break;
        case 7: adminMenu(inventory, undoStack, productTree); break;
        case 8: showcase.display(); pauseScreen(); break;
        case 9: sortInventory(inventory); pauseScreen(); break;
        case 10: cout << GOLD BOLD  "\nThank you for visiting Lumiere Luxe!\nWhere Luxury Meets Light.\n" RESET; break;
        }
    } while (choice != 10);
    // Destructors release all owned dynamic memory automatically.
    return 0;
}