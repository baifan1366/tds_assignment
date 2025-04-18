#include <iostream>
#include <string>
#include <iomanip>
#include <conio.h>
#include <limits>
#include <fstream>
#include <ctime>

using namespace std;

// Food item structure for restaurant inventory
struct FoodItem {
    string id;          // Unique identifier
    string name;        // Food item name
    double price;       // Price per unit
    int quantity;       // Current quantity in stock
    string category;    // Category (e.g., "Meat", "Vegetable", "Spice")
    bool isOccupied;    // Flag to mark if this position is occupied
    bool isDeleted;     // Flag to mark if item is deleted (for lazy deletion)
    string createdBy;   // Username who created this item
    string lastModifiedBy; // Username who last modified this item
    string createdDate; // Date when item was created
    string lastModifiedDate; // Date when item was last modified
    
    // Default constructor
    FoodItem() : isOccupied(false), isDeleted(false) {}
    
    // Parameterized constructor
    FoodItem(string id, string name, double price, int quantity, string category, string creator = "system")
        : id(id), name(name), price(price), quantity(quantity), category(category), 
          isOccupied(true), isDeleted(false), createdBy(creator), lastModifiedBy(creator) {
        // Set creation time
        time_t now = time(0);
        createdDate = ctime(&now);
        lastModifiedDate = createdDate;
    }
};

// Node structure for linked queue
struct Node {
    FoodItem data;
    Node* next;
};

// Linked Queue for processing inventory items
class LinkedQueue {
private:
    Node *front, *rear;
public:
    LinkedQueue() {
        front = NULL;
        rear = NULL;
    }
    
    int empty() {
        if (front == NULL)
            return 1;
        else
            return 0;
    }
    
    void append(FoodItem item) {
        if (rear != NULL) {
            rear->next = new Node;
            rear = rear->next;
            rear->data = item;
            rear->next = NULL;
        } else {
            front = rear = new Node;
            front->data = item;
            front->next = NULL;
        }
    }
    
    FoodItem serve() {
        FoodItem item;
        if (!empty()) {
            item = front->data;
            Node *temp = front;
            front = front->next;
            delete temp;
            if (front == NULL) {
                rear = NULL;
            }
            return item;
        } else {
            cout << "Queue is Empty" << endl;
            return item; // Return default empty FoodItem
        }
    }
    
    ~LinkedQueue() {
        while (!empty()) {
            serve();
        }
    }
};

// User structure for authentication
struct User {
    string username;
    string password;
    string role;      // "admin" or "staff"
    string fullName;
    string lastLogin;
    
    User() {}
    
    User(string username, string password, string role, string fullName) 
        : username(username), password(password), role(role), fullName(fullName) {
        // Set current time as last login
        time_t now = time(0);
        lastLogin = ctime(&now);
    }
};

// Authentication system for managing users
class AuthSystem {
private:
    static const int MAX_USERS = 50;
    User users[MAX_USERS];
    int userCount;
    User* currentUser;
    
    // Encrypt password (simple encryption for demonstration)
    string encryptPassword(const string& password) const {
        string encrypted = password;
        for (char& c : encrypted) {
            c = c + 1;  // Simple Caesar cipher
        }
        return encrypted;
    }
    
    // Decrypt password
    string decryptPassword(const string& encrypted) const {
        string decrypted = encrypted;
        for (char& c : decrypted) {
            c = c - 1;  // Reverse Caesar cipher
        }
        return decrypted;
    }
    
    // Save users to file
    void saveUsers() const {
        ofstream file("users.dat", ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(&userCount), sizeof(userCount));
            for (int i = 0; i < userCount; i++) {
                // Write string lengths first, then the strings
                int len = users[i].username.length();
                file.write(reinterpret_cast<const char*>(&len), sizeof(len));
                file.write(users[i].username.c_str(), len);
                
                len = users[i].password.length();
                file.write(reinterpret_cast<const char*>(&len), sizeof(len));
                file.write(users[i].password.c_str(), len);
                
                len = users[i].role.length();
                file.write(reinterpret_cast<const char*>(&len), sizeof(len));
                file.write(users[i].role.c_str(), len);
                
                len = users[i].fullName.length();
                file.write(reinterpret_cast<const char*>(&len), sizeof(len));
                file.write(users[i].fullName.c_str(), len);
                
                len = users[i].lastLogin.length();
                file.write(reinterpret_cast<const char*>(&len), sizeof(len));
                file.write(users[i].lastLogin.c_str(), len);
            }
            file.close();
        }
    }
    
    // Load users from file
    void loadUsers() {
        ifstream file("users.dat", ios::binary);
        if (file.is_open()) {
            file.read(reinterpret_cast<char*>(&userCount), sizeof(userCount));
            for (int i = 0; i < userCount; i++) {
                // Read string lengths first, then the strings
                int len;
                char* buffer;
                
                // Read username
                file.read(reinterpret_cast<char*>(&len), sizeof(len));
                buffer = new char[len + 1];
                file.read(buffer, len);
                buffer[len] = '\0';
                users[i].username = buffer;
                delete[] buffer;
                
                // Read password
                file.read(reinterpret_cast<char*>(&len), sizeof(len));
                buffer = new char[len + 1];
                file.read(buffer, len);
                buffer[len] = '\0';
                users[i].password = buffer;
                delete[] buffer;
                
                // Read role
                file.read(reinterpret_cast<char*>(&len), sizeof(len));
                buffer = new char[len + 1];
                file.read(buffer, len);
                buffer[len] = '\0';
                users[i].role = buffer;
                delete[] buffer;
                
                // Read fullName
                file.read(reinterpret_cast<char*>(&len), sizeof(len));
                buffer = new char[len + 1];
                file.read(buffer, len);
                buffer[len] = '\0';
                users[i].fullName = buffer;
                delete[] buffer;
                
                // Read lastLogin
                file.read(reinterpret_cast<char*>(&len), sizeof(len));
                buffer = new char[len + 1];
                file.read(buffer, len);
                buffer[len] = '\0';
                users[i].lastLogin = buffer;
                delete[] buffer;
            }
            file.close();
        }
        else {
            // Create default admin if file doesn't exist
            userCount = 1;
            users[0] = User("admin", encryptPassword("admin123"), "admin", "System Administrator");
            saveUsers();
        }
    }
    
    // Helper to clear input buffer
    void clearInputBuffer() const {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Print a formatted header for menus
    void printHeader(const string& title) const {
        cout << "\n" << string(70, '=') << endl;
        cout << string(30, ' ') << title << endl;
        cout << string(70, '=') << endl;
    }
    
    // Print a formatted footer
    void printFooter() const {
        cout << string(70, '-') << endl;
    }
    
public:
    // Constructor
    AuthSystem() : userCount(0), currentUser(nullptr) {
        loadUsers();
    }
    
    // Destructor
    ~AuthSystem() {
        saveUsers();
    }
    
    // Check if a username already exists
    bool usernameExists(const string& username) const {
        for (int i = 0; i < userCount; i++) {
            if (users[i].username == username) {
                return true;
            }
        }
        return false;
    }
    
    // Register a new user
    bool registerUser(const User& user) {
        if (userCount >= MAX_USERS) {
            cout << "Maximum number of users reached." << endl;
            return false;
        }
        
        if (usernameExists(user.username)) {
            cout << "Username already exists." << endl;
            return false;
        }
        
        users[userCount++] = user;
        users[userCount-1].password = encryptPassword(user.password);
        saveUsers();
        return true;
    }
    
    // Authenticate a user
    bool login(const string& username, const string& password) {
        for (int i = 0; i < userCount; i++) {
            if (users[i].username == username && 
                decryptPassword(users[i].password) == password) {
                currentUser = &users[i];
                
                // Update last login time
                time_t now = time(0);
                currentUser->lastLogin = ctime(&now);
                saveUsers();
                
                return true;
            }
        }
        return false;
    }
    
    // Log out the current user
    void logout() {
        currentUser = nullptr;
    }
    
    // Get current user
    User* getCurrentUser() const {
        return currentUser;
    }
    
    // Check if user is admin
    bool isAdmin() const {
        return currentUser && currentUser->role == "admin";
    }
    
    // Signup screen
    bool showSignupScreen() {
        printHeader("CREATE NEW ACCOUNT");
        
        string username, password, confirmPassword, fullName, role;
        
        cout << "Enter username: ";
        getline(cin, username);
        
        if (usernameExists(username)) {
            cout << "Username already exists. Please choose another one." << endl;
            cout << "Press any key to continue...";
            getch();
            return false;
        }
        
        char ch;
        cout << "Enter password: ";
        password = "";
        while ((ch = getch()) != 13) { // 13 is Enter key
            if (ch == 8) { // Backspace
                if (password.length() > 0) {
                    password.pop_back();
                    cout << "\b \b"; // Erase character on screen
                }
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        
        cout << "Confirm password: ";
        confirmPassword = "";
        while ((ch = getch()) != 13) { // 13 is Enter key
            if (ch == 8) { // Backspace
                if (confirmPassword.length() > 0) {
                    confirmPassword.pop_back();
                    cout << "\b \b"; // Erase character on screen
                }
            } else {
                confirmPassword.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        
        if (password != confirmPassword) {
            cout << "Passwords do not match." << endl;
            cout << "Press any key to continue...";
            getch();
            return false;
        }
        
        cout << "Enter full name: ";
        getline(cin, fullName);
        
        // Only admin can create admin accounts
        if (currentUser && currentUser->role == "admin") {
            cout << "Select role:" << endl;
            cout << "1. Staff" << endl;
            cout << "2. Admin" << endl;
            cout << "Enter choice (1-2): ";
            
            int choice;
            cin >> choice;
            clearInputBuffer();
            
            role = (choice == 2) ? "admin" : "staff";
        } else {
            role = "staff"; // Default role for self-registration
        }
        
        User newUser(username, password, role, fullName);
        if (registerUser(newUser)) {
            cout << "Account created successfully!" << endl;
            cout << "Press any key to continue...";
            getch();
            return true;
        } else {
            cout << "Failed to create account." << endl;
            cout << "Press any key to continue...";
            getch();
            return false;
        }
    }
    
    // Login screen
    bool showLoginScreen() {
        printHeader("LOGIN");
        
        string username, password;
        
        cout << "Enter username: ";
        getline(cin, username);
        
        char ch;
        cout << "Enter password: ";
        password = "";
        while ((ch = getch()) != 13) { // 13 is Enter key
            if (ch == 8) { // Backspace
                if (password.length() > 0) {
                    password.pop_back();
                    cout << "\b \b"; // Erase character on screen
                }
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        
        if (login(username, password)) {
            cout << "Login successful! Welcome, " << currentUser->fullName << "!" << endl;
            cout << "Press any key to continue...";
            getch();
            return true;
        } else {
            cout << "Invalid username or password." << endl;
            cout << "Press any key to continue...";
            getch();
            return false;
        }
    }
    
    // User management screen (admin only)
    void showUserManagementScreen() {
        if (!isAdmin()) {
            cout << "Access denied: Admin privileges required." << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        bool running = true;
        while (running) {
            system("cls");
            printHeader("USER MANAGEMENT");
            
            cout << left << setw(15) << "Username" 
                 << setw(25) << "Full Name" 
                 << setw(10) << "Role" 
                 << setw(25) << "Last Login" << endl;
            printFooter();
            
            for (int i = 0; i < userCount; i++) {
                cout << left << setw(15) << users[i].username 
                     << setw(25) << users[i].fullName 
                     << setw(10) << users[i].role 
                     << setw(25) << users[i].lastLogin << endl;
            }
            
            cout << "\n1. Add New User" << endl;
            cout << "2. Delete User" << endl;
            cout << "3. Back to Main Menu" << endl;
            cout << "Enter choice: ";
            
            int choice;
            cin >> choice;
            clearInputBuffer();
            
            // Declare variables that might be needed in the switch before the switch statement
            string username;
            int index;
            
            switch (choice) {
                case 1:
                    system("cls");
                    showSignupScreen();
                    break;
                    
                case 2:
                    system("cls");
                    printHeader("DELETE USER");
                    
                    if (userCount <= 1) {
                        cout << "Cannot delete the last user account." << endl;
                        cout << "Press any key to continue...";
                        getch();
                        break;
                    }
                    
                    cout << "Enter username to delete: ";
                    getline(cin, username);
                    
                    if (username == currentUser->username) {
                        cout << "Cannot delete your own account." << endl;
                        cout << "Press any key to continue...";
                        getch();
                        break;
                    }
                    
                    index = -1;
                    for (int i = 0; i < userCount; i++) {
                        if (users[i].username == username) {
                            index = i;
                            break;
                        }
                    }
                    
                    if (index == -1) {
                        cout << "User not found." << endl;
                        cout << "Press any key to continue...";
                        getch();
                        break;
                    }
                    
                    // Shift users to fill the gap
                    for (int i = index; i < userCount - 1; i++) {
                        users[i] = users[i+1];
                    }
                    userCount--;
                    saveUsers();
                    
                    cout << "User deleted successfully." << endl;
                    cout << "Press any key to continue...";
                    getch();
                    break;
                    
                case 3:
                    running = false;
                    break;
                    
                default:
                    cout << "Invalid choice. Press any key to try again...";
                    getch();
            }
        }
    }
    
    // Change password screen
    void showChangePasswordScreen() {
        if (!currentUser) {
            cout << "You must be logged in to change your password." << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        system("cls");
        printHeader("CHANGE PASSWORD");
        
        string currentPassword, newPassword, confirmPassword;
        
        char ch;
        cout << "Enter current password: ";
        currentPassword = "";
        while ((ch = getch()) != 13) { // 13 is Enter key
            if (ch == 8) { // Backspace
                if (currentPassword.length() > 0) {
                    currentPassword.pop_back();
                    cout << "\b \b"; // Erase character on screen
                }
            } else {
                currentPassword.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        
        if (decryptPassword(currentUser->password) != currentPassword) {
            cout << "Current password is incorrect." << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        cout << "Enter new password: ";
        newPassword = "";
        while ((ch = getch()) != 13) { // 13 is Enter key
            if (ch == 8) { // Backspace
                if (newPassword.length() > 0) {
                    newPassword.pop_back();
                    cout << "\b \b"; // Erase character on screen
                }
            } else {
                newPassword.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        
        cout << "Confirm new password: ";
        confirmPassword = "";
        while ((ch = getch()) != 13) { // 13 is Enter key
            if (ch == 8) { // Backspace
                if (confirmPassword.length() > 0) {
                    confirmPassword.pop_back();
                    cout << "\b \b"; // Erase character on screen
                }
            } else {
                confirmPassword.push_back(ch);
                cout << '*';
            }
        }
        cout << endl;
        
        if (newPassword != confirmPassword) {
            cout << "Passwords do not match." << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        currentUser->password = encryptPassword(newPassword);
        saveUsers();
        
        cout << "Password changed successfully." << endl;
        cout << "Press any key to continue...";
        getch();
    }
    
    // Show auth menu (before login)
    bool showAuthMenu() {
        bool loggedIn = false;
        bool quit = false;
        
        while (!loggedIn && !quit) {
            system("cls");
            printHeader("RESTAURANT INVENTORY SYSTEM");
            cout << "1. Login" << endl;
            cout << "2. Create Account" << endl;
            cout << "3. Exit" << endl;
            printFooter();
            
            cout << "Enter choice: ";
            int choice;
            cin >> choice;
            clearInputBuffer();
            
            // Declare any variables that might be needed in the switch before the switch statement
            string username; // Move declaration here
            
            switch (choice) {
                case 1:
                    system("cls");
                    loggedIn = showLoginScreen();
                    break;
                    
                case 2:
                    system("cls");
                    showSignupScreen();
                    break;
                    
                case 3:
                    quit = true;
                    break;
                    
                default:
                    cout << "Invalid choice. Press any key to try again...";
                    getch();
            }
        }
        
        return loggedIn;
    }
    
    // Show user menu (after login)
    void showUserMenu() {
        if (!currentUser) {
            return;
        }
        
        bool running = true;
        while (running) {
            system("cls");
            printHeader("USER MENU");
            cout << "Logged in as: " << currentUser->fullName << " (" << currentUser->role << ")" << endl;
            cout << "\n1. Change Password" << endl;
            
            if (isAdmin()) {
                cout << "2. User Management" << endl;
                cout << "3. Back to Main Menu" << endl;
                cout << "4. Logout" << endl;
            } else {
                cout << "2. Back to Main Menu" << endl;
                cout << "3. Logout" << endl;
            }
            
            printFooter();
            cout << "Enter choice: ";
            
            int choice;
            cin >> choice;
            clearInputBuffer();
            
            if (isAdmin()) {
                switch (choice) {
                    case 1:
                        showChangePasswordScreen();
                        break;
                        
                    case 2:
                        showUserManagementScreen();
                        break;
                        
                    case 3:
                        running = false;
                        break;
                        
                    case 4:
                        logout();
                        running = false;
                        break;
                        
                    default:
                        cout << "Invalid choice. Press any key to try again...";
                        getch();
                }
            } else {
                switch (choice) {
                    case 1:
                        showChangePasswordScreen();
                        break;
                        
                    case 2:
                        running = false;
                        break;
                        
                    case 3:
                        logout();
                        running = false;
                        break;
                        
                    default:
                        cout << "Invalid choice. Press any key to try again...";
                        getch();
                }
            }
        }
    }
};

// Inventory system using quadratic hashing
class InventorySystem {
private:
    static const int TABLE_SIZE = 101;  // Prime number for table size
    FoodItem* table;                    // Hash table array
    int itemCount;                      // Current item count
    
    // Hash function - convert ID to integer index
    int hashFunction(const string& id) const {
        int hash = 0;
        for (char c : id) {
            hash = (hash * 31 + c) % TABLE_SIZE;
        }
        return hash;
    }
    
    // Find position using quadratic probing
    int findPosition(const string& id) const {
        int hashValue = hashFunction(id);
        int i = 0;
        
        while (i < TABLE_SIZE) {
            // Quadratic probing formula: h(k, i) = (h(k) + i^2) % TABLE_SIZE
            int position = (hashValue + i*i) % TABLE_SIZE;
            
            // If position is empty or has the item we're looking for, return it
            if (!table[position].isOccupied || 
                (table[position].isOccupied && table[position].id == id && !table[position].isDeleted)) {
                return position;
            }
            
            // Try next position
            i++;
        }
        
        // Table is full or can't find suitable position
        return -1;
    }
    
    // Find position for inserting a new item
    int findInsertPosition(const string& id) const {
        int hashValue = hashFunction(id);
        int i = 0;
        int firstDeletedPos = -1;
        
        while (i < TABLE_SIZE) {
            int position = (hashValue + i*i) % TABLE_SIZE;
            
            // If position is empty, we can insert directly
            if (!table[position].isOccupied) {
                // If we found a deleted position earlier, return that instead
                return (firstDeletedPos != -1) ? firstDeletedPos : position;
            }
            
            // If this is a deleted position, remember the first such position
            if (table[position].isDeleted && firstDeletedPos == -1) {
                firstDeletedPos = position;
            }
            
            // If we find the same ID and it's not deleted, return this position for update
            if (table[position].id == id && !table[position].isDeleted) {
                return position;
            }
            
            i++;
        }
        
        // If we found a deleted position, return it
        if (firstDeletedPos != -1) {
            return firstDeletedPos;
        }
        
        // Table is full
        return -1;
    }
    
    // Binary search for sorted array of FoodItems (by ID)
    int binarySearch(FoodItem arr[], int size, const string& target) const {
        int first = 0;
        int last = size - 1;
        int found = 0;
        int mid;
        
        while ((found == 0) && (first <= last)) {
            mid = (first + last) / 2;
            if (target == arr[mid].id) {
                found = 1;
            } else {
                if (target < arr[mid].id) {
                    last = mid - 1;
                } else {
                    first = mid + 1;
                }
            }
        }
        
        if (found) {
            return mid;
        } else {
            return -1; // Target not found
        }
    }
    
    // Merge function for merge sort
    void merge(FoodItem arr[], int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        // Create temporary arrays
        FoodItem* leftArr = new FoodItem[n1];
        FoodItem* rightArr = new FoodItem[n2];
        
        // Copy data to temporary arrays
        for (int i = 0; i < n1; i++)
            leftArr[i] = arr[left + i];
        for (int j = 0; j < n2; j++)
            rightArr[j] = arr[mid + 1 + j];
        
        // Merge the temporary arrays back into arr[left..right]
        int i = 0;
        int j = 0;
        int k = left;
        
        while (i < n1 && j < n2) {
            if (leftArr[i].id <= rightArr[j].id) {
                arr[k] = leftArr[i];
                i++;
            } else {
                arr[k] = rightArr[j];
                j++;
            }
            k++;
        }
        
        // Copy the remaining elements of leftArr[], if there are any
        while (i < n1) {
            arr[k] = leftArr[i];
            i++;
            k++;
        }
        
        // Copy the remaining elements of rightArr[], if there are any
        while (j < n2) {
            arr[k] = rightArr[j];
            j++;
            k++;
        }
        
        // Free temporary arrays
        delete[] leftArr;
        delete[] rightArr;
    }
    
    // Merge sort implementation
    void mergeSort(FoodItem arr[], int left, int right) {
        if (left < right) {
            int mid = left + (right - left) / 2;
            
            // Sort first and second halves
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
            
            // Merge the sorted halves
            merge(arr, left, mid, right);
        }
    }
    
    // Helper function to clear input buffer
    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    // Get valid numeric input
    template <typename T>
    T getNumericInput(const string& prompt) {
        T value;
        cout << prompt;
        while (!(cin >> value)) {
            cout << "Invalid input. Please enter a number: ";
            clearInputBuffer();
        }
        clearInputBuffer();
        return value;
    }
    
    // Print a formatted header for menus
    void printHeader(const string& title) const {
        cout << "\n" << string(70, '=') << endl;
        cout << string(30, ' ') << title << endl;
        cout << string(70, '=') << endl;
    }
    
    // Print a formatted footer
    void printFooter() const {
        cout << string(70, '-') << endl;
    }
    
    // Helper method to find an item in a const context
    const FoodItem* findConst(const string& id) const {
        int position = findPosition(id);
        
        if (position != -1 && table[position].isOccupied && !table[position].isDeleted) {
            return &table[position];
        }
        
        return nullptr;  // Item not found
    }
    
public:
    // Constructor
    InventorySystem() : itemCount(0) {
        table = new FoodItem[TABLE_SIZE];
    }
    
    // Destructor
    ~InventorySystem() {
        delete[] table;
    }
    
    // Insert or update a food item
    bool insert(const FoodItem& item) {
        // If table is almost full, don't insert
        if (itemCount >= TABLE_SIZE * 0.7) {
            cout << "Hash table is near capacity. Cannot insert more items." << endl;
            return false;
        }
        
        int position = findInsertPosition(item.id);
        if (position == -1) {
            cout << "Cannot insert item: Table is full or no suitable position found." << endl;
            return false;
        }
        
        // Check if this is an update operation
        bool isUpdate = table[position].isOccupied && !table[position].isDeleted;
        
        // Insert or update the item
        table[position] = item;
        
        // Update count (only for new inserts)
        if (!isUpdate) {
            itemCount++;
        }
        
        return true;
    }
    
    // Find a food item by ID
    FoodItem* find(const string& id) const {
        int position = findPosition(id);
        
        if (position != -1 && table[position].isOccupied && !table[position].isDeleted) {
            return &table[position];
        }
        
        return nullptr;  // Item not found
    }
    
    // Remove a food item by ID (lazy deletion)
    bool remove(const string& id) {
        int position = findPosition(id);
        
        if (position != -1 && table[position].isOccupied && !table[position].isDeleted) {
            table[position].isDeleted = true;
            itemCount--;
            return true;
        }
        
        return false;  // Item not found
    }
    
    // Get array of all active items for sorting/searching
    FoodItem* getItemsArray(int& size) {
        size = 0;
        FoodItem* items = new FoodItem[itemCount];
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                items[size++] = table[i];
            }
        }
        
        return items;
    }
    
    // Sort items by ID using merge sort
    FoodItem* getSortedItems(int& size) {
        FoodItem* items = getItemsArray(size);
        mergeSort(items, 0, size - 1);
        return items;
    }
    
    // Search for an item in sorted array using binary search
    FoodItem* findInSortedArray(const string& id) {
        int size;
        FoodItem* sortedItems = getSortedItems(size);
        int position = binarySearch(sortedItems, size, id);
        
        if (position != -1) {
            FoodItem* result = new FoodItem(sortedItems[position]);
            delete[] sortedItems;
            return result;
        }
        
        delete[] sortedItems;
        return nullptr;
    }
    
    // Add items to process queue
    void addToProcessQueue(LinkedQueue& queue, const string& category) {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].isOccupied && !table[i].isDeleted && 
                (category.empty() || table[i].category == category)) {
                queue.append(table[i]);
            }
        }
    }
    
    // Display all inventory items
    void displayAll() const {
        printHeader("Restaurant Inventory System");
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(10) << "Price" 
             << setw(10) << "Quantity" 
             << setw(15) << "Category" 
             << setw(15) << "Created By" << endl;
        printFooter();
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                cout << left << setw(8) << table[i].id 
                     << setw(20) << table[i].name 
                     << setw(10) << fixed << setprecision(2) << table[i].price
                     << setw(10) << table[i].quantity
                     << setw(15) << table[i].category
                     << setw(15) << table[i].createdBy << endl;
            }
        }
        
        cout << "\nEnter item ID to view details or press Enter to continue: ";
        string id;
        getline(cin, id);
        
        if (!id.empty()) {
            const FoodItem* item = findConst(id);
            if (item != nullptr) {
                system("cls");
                printHeader("Item Details");
                cout << "ID: " << item->id << endl;
                cout << "Name: " << item->name << endl;
                cout << "Price: $" << fixed << setprecision(2) << item->price << endl;
                cout << "Quantity: " << item->quantity << endl;
                cout << "Category: " << item->category << endl;
                cout << "Created by: " << item->createdBy << endl;
                cout << "Created on: " << item->createdDate;
                cout << "Last modified by: " << item->lastModifiedBy << endl;
                cout << "Last modified on: " << item->lastModifiedDate;
                cout << "\nPress any key to continue...";
                getch();
            } else {
                cout << "Item with ID " << id << " not found!" << endl;
                cout << "Press any key to continue...";
                getch();
            }
        }
    }
    
    // Display items by category
    void displayByCategory(const string& category) const {
        printHeader("Category: " + category);
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(10) << "Price" 
             << setw(10) << "Quantity" << endl;
        printFooter();
        
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].isOccupied && !table[i].isDeleted && table[i].category == category) {
                cout << left << setw(8) << table[i].id 
                     << setw(20) << table[i].name 
                     << setw(10) << fixed << setprecision(2) << table[i].price
                     << setw(10) << table[i].quantity << endl;
            }
        }
    }
    
    // Display low stock items
    void displayLowStock(int threshold) const {
        printHeader("Low Stock Items (Below " + to_string(threshold) + ")");
        cout << left << setw(8) << "ID" 
             << setw(20) << "Name" 
             << setw(10) << "Price" 
             << setw(10) << "Quantity" 
             << setw(15) << "Category" << endl;
        printFooter();
        
        bool found = false;
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].isOccupied && !table[i].isDeleted && table[i].quantity < threshold) {
                cout << left << setw(8) << table[i].id 
                     << setw(20) << table[i].name 
                     << setw(10) << fixed << setprecision(2) << table[i].price
                     << setw(10) << table[i].quantity
                     << setw(15) << table[i].category << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No items below the threshold were found." << endl;
        }
    }
    
    // Update item quantity
    bool updateQuantity(const string& id, int newQuantity) {
        FoodItem* item = find(id);
        if (item != nullptr) {
            item->quantity = newQuantity;
            return true;
        }
        return false;
    }
    
    // Process items using the queue (e.g., for reports or batch operations)
    void processItems(const string& category = "") {
        LinkedQueue processQueue;
        addToProcessQueue(processQueue, category);
        
        printHeader("Processing Items" + (category.empty() ? "" : " in category " + category));
        
        while (!processQueue.empty()) {
            FoodItem item = processQueue.serve();
            cout << "Processed: " << item.id << " - " << item.name << endl;
        }
    }
    
    // Add a new item from user input with authentication
    void addItemFromUserInput(AuthSystem& authSystem) {
        printHeader("Add New Item");
        
        User* currentUser = authSystem.getCurrentUser();
        if (!currentUser) {
            cout << "Error: No user logged in." << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        string id, name, category;
        double price;
        int quantity;
        
        cout << "Enter item ID: ";
        getline(cin, id);
        
        // Check if ID already exists
        if (find(id) != nullptr) {
            cout << "Item with ID " << id << " already exists!" << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        cout << "Enter item name: ";
        getline(cin, name);
        
        price = getNumericInput<double>("Enter price: $");
        quantity = getNumericInput<int>("Enter quantity: ");
        
        cout << "Enter category: ";
        getline(cin, category);
        
        FoodItem newItem(id, name, price, quantity, category, currentUser->username);
        if (insert(newItem)) {
            cout << "Item added successfully!" << endl;
        } else {
            cout << "Failed to add item." << endl;
        }
        
        cout << "Press any key to continue...";
        getch();
    }
    
    // Update existing item from user input with authentication
    void updateItemFromUserInput(AuthSystem& authSystem) {
        printHeader("Update Item");
        
        User* currentUser = authSystem.getCurrentUser();
        if (!currentUser) {
            cout << "Error: No user logged in." << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        string id;
        cout << "Enter item ID to update: ";
        getline(cin, id);
        
        FoodItem* item = find(id);
        if (item == nullptr) {
            cout << "Item with ID " << id << " not found!" << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        cout << "Current details:" << endl;
        cout << "Name: " << item->name << endl;
        cout << "Price: $" << fixed << setprecision(2) << item->price << endl;
        cout << "Quantity: " << item->quantity << endl;
        cout << "Category: " << item->category << endl;
        cout << "Created by: " << item->createdBy << " on " << item->createdDate;
        cout << "Last modified by: " << item->lastModifiedBy << " on " << item->lastModifiedDate;
        cout << "\n";
        
        string name = item->name;
        double price = item->price;
        int quantity = item->quantity;
        string category = item->category;
        
        char choice;
        cout << "Update name? (y/n): ";
        cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            cout << "Enter new name: ";
            getline(cin, name);
        }
        
        cout << "Update price? (y/n): ";
        cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            price = getNumericInput<double>("Enter new price: $");
        }
        
        cout << "Update quantity? (y/n): ";
        cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            quantity = getNumericInput<int>("Enter new quantity: ");
        }
        
        cout << "Update category? (y/n): ";
        cin >> choice;
        clearInputBuffer();
        if (choice == 'y' || choice == 'Y') {
            cout << "Enter new category: ";
            getline(cin, category);
        }
        
        // Create the updated item with new modification info
        FoodItem updatedItem = *item;
        updatedItem.name = name;
        updatedItem.price = price;
        updatedItem.quantity = quantity;
        updatedItem.category = category;
        updatedItem.lastModifiedBy = currentUser->username;
        
        // Update the last modified date
        time_t now = time(0);
        updatedItem.lastModifiedDate = ctime(&now);
        
        if (insert(updatedItem)) {
            cout << "Item updated successfully!" << endl;
        } else {
            cout << "Failed to update item." << endl;
        }
        
        cout << "Press any key to continue...";
        getch();
    }
    
    // Adjust inventory quantity (add/subtract) with authentication
    void adjustQuantityFromUserInput(AuthSystem& authSystem) {
        printHeader("Adjust Inventory Quantity");
        
        User* currentUser = authSystem.getCurrentUser();
        if (!currentUser) {
            cout << "Error: No user logged in." << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        string id;
        cout << "Enter item ID: ";
        getline(cin, id);
        
        FoodItem* item = find(id);
        if (item == nullptr) {
            cout << "Item with ID " << id << " not found!" << endl;
            cout << "Press any key to continue...";
            getch();
            return;
        }
        
        cout << "Current item: " << item->name << ", Quantity: " << item->quantity << endl;
        
        cout << "1. Add to inventory" << endl;
        cout << "2. Remove from inventory" << endl;
        cout << "3. Set exact quantity" << endl;
        cout << "Enter choice (1-3): ";
        
        int choice;
        cin >> choice;
        clearInputBuffer();
        
        int quantity, newQuantity;
        FoodItem updatedItem = *item;
        
        switch (choice) {
            case 1:
                quantity = getNumericInput<int>("Enter quantity to add: ");
                newQuantity = item->quantity + quantity;
                updatedItem.quantity = newQuantity;
                updatedItem.lastModifiedBy = currentUser->username;
                
                // Update the last modified date
                {
                    time_t now = time(0);
                    updatedItem.lastModifiedDate = ctime(&now);
                }
                
                if (insert(updatedItem)) {
                    cout << "Added " << quantity << " to inventory. New quantity: " << newQuantity << endl;
                } else {
                    cout << "Failed to update inventory." << endl;
                }
                break;
                
            case 2:
                quantity = getNumericInput<int>("Enter quantity to remove: ");
                if (quantity > item->quantity) {
                    cout << "Warning: Removing more than available!" << endl;
                    cout << "Continue? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    clearInputBuffer();
                    
                    if (confirm != 'y' && confirm != 'Y') {
                        cout << "Operation cancelled." << endl;
                        break;
                    }
                }
                
                newQuantity = max(0, item->quantity - quantity);
                updatedItem.quantity = newQuantity;
                updatedItem.lastModifiedBy = currentUser->username;
                
                // Update the last modified date
                {
                    time_t now = time(0);
                    updatedItem.lastModifiedDate = ctime(&now);
                }
                
                if (insert(updatedItem)) {
                    cout << "Removed " << quantity << " from inventory. New quantity: " << newQuantity << endl;
                } else {
                    cout << "Failed to update inventory." << endl;
                }
                break;
                
            case 3:
                newQuantity = getNumericInput<int>("Enter new quantity: ");
                updatedItem.quantity = newQuantity;
                updatedItem.lastModifiedBy = currentUser->username;
                
                // Update the last modified date
                {
                    time_t now = time(0);
                    updatedItem.lastModifiedDate = ctime(&now);
                }
                
                if (insert(updatedItem)) {
                    cout << "Quantity updated to: " << newQuantity << endl;
                } else {
                    cout << "Failed to update inventory." << endl;
                }
                break;
                
            default:
                cout << "Invalid choice!" << endl;
        }
        
        cout << "Press any key to continue...";
        getch();
    }
    
    // Remove item from user input
    void removeItemFromUserInput() {
        printHeader("Remove Item");
        
        string id;
        cout << "Enter item ID to remove: ";
        getline(cin, id);
        
        if (remove(id)) {
            cout << "Item removed successfully!" << endl;
        } else {
            cout << "Item with ID " << id << " not found!" << endl;
        }
        
        cout << "Press any key to continue...";
        getch();
    }
    
    // Find and display an item
    void findItemFromUserInput() {
        printHeader("Find Item");
        
        string id;
        cout << "Enter item ID to find: ";
        getline(cin, id);
        
        FoodItem* item = find(id);
        if (item != nullptr) {
            cout << "\nItem Found:" << endl;
            cout << "ID: " << item->id << endl;
            cout << "Name: " << item->name << endl;
            cout << "Price: $" << fixed << setprecision(2) << item->price << endl;
            cout << "Quantity: " << item->quantity << endl;
            cout << "Category: " << item->category << endl;
        } else {
            cout << "Item with ID " << id << " not found!" << endl;
        }
        
        cout << "\nPress any key to continue...";
        getch();
    }
    
    // Display categories and let user select one
    void displayCategoryFromUserInput() {
        printHeader("Display By Category");
        
        string category;
        cout << "Enter category to display (or press Enter for all categories): ";
        getline(cin, category);
        
        if (category.empty()) {
            displayAll();
        } else {
            displayByCategory(category);
        }
        
        cout << "\nPress any key to continue...";
        getch();
    }
    
    // Check low stock items
    void checkLowStockFromUserInput() {
        printHeader("Check Low Stock");
        
        int threshold = getNumericInput<int>("Enter threshold quantity: ");
        displayLowStock(threshold);
        
        cout << "\nPress any key to continue...";
        getch();
    }
    
    // Generate a report using a queue
    void generateReportFromUserInput() {
        printHeader("Generate Report");
        
        cout << "1. Process all items" << endl;
        cout << "2. Process items by category" << endl;
        cout << "Enter choice (1-2): ";
        
        int choice;
        cin >> choice;
        clearInputBuffer();
        
        if (choice == 1) {
            processItems();
        } else if (choice == 2) {
            string category;
            cout << "Enter category: ";
            getline(cin, category);
            processItems(category);
        } else {
            cout << "Invalid choice!" << endl;
        }
        
        cout << "\nPress any key to continue...";
        getch();
    }
    
    // Check if inventory is empty
    bool isEmpty() const {
        for (int i = 0; i < TABLE_SIZE; i++) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                return false;
            }
        }
        return true;
    }
    
    // Run the main menu interface with authentication
    int runMainMenu(AuthSystem& authSystem) {
        int choice;
        User* currentUser = authSystem.getCurrentUser();
        
        system("cls");  // Clear screen
        
        printHeader("RESTAURANT INVENTORY MANAGEMENT SYSTEM");
        cout << "Logged in as: " << currentUser->fullName << " (" << currentUser->role << ")" << endl << endl;
        
        cout << "1. Display All Items" << endl;
        cout << "2. Add New Item" << endl;
        cout << "3. Update Existing Item" << endl;
        cout << "4. Remove Item" << endl;
        cout << "5. Find Item" << endl;
        cout << "6. Display by Category" << endl;
        cout << "7. Check Low Stock Items" << endl;
        cout << "8. Adjust Inventory Quantity" << endl;
        cout << "9. Generate Report" << endl;
        cout << "10. User Account Settings" << endl;
        cout << "0. Exit" << endl;
        printFooter();
        
        cout << "Enter your choice: ";
        cin >> choice;
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                system("cls");
                displayAll();
                cout << "\nPress any key to continue...";
                getch();
                break;
                
            case 2:
                // Only staff and admin can add items
                system("cls");
                addItemFromUserInput(authSystem);
                break;
                
            case 3:
                system("cls");
                updateItemFromUserInput(authSystem);
                break;
                
            case 4:
                // Only admin can remove items
                if (!authSystem.isAdmin()) {
                    system("cls");
                    cout << "Access denied: Admin privileges required to remove items." << endl;
                    cout << "Press any key to continue...";
                    getch();
                    break;
                }
                system("cls");
                removeItemFromUserInput();
                break;
                
            case 5:
                system("cls");
                findItemFromUserInput();
                break;
                
            case 6:
                system("cls");
                displayCategoryFromUserInput();
                break;
                
            case 7:
                system("cls");
                checkLowStockFromUserInput();
                break;
                
            case 8:
                system("cls");
                adjustQuantityFromUserInput(authSystem);
                break;
                
            case 9:
                system("cls");
                generateReportFromUserInput();
                break;
                
            case 10:
                return 1; // Return 1 indicates user menu selected
                
            case 0:
                return 0; // Return 0 indicates exit selected
                
            default:
                cout << "Invalid choice! Press any key to try again...";
                getch();
        }
        
        return 2; // Return 2 indicates continue in main menu
    }
};

// Main function to run the system
int main() {
    AuthSystem authSystem;
    InventorySystem inventory;
    
    // Try to login first
    if (!authSystem.showAuthMenu()) {
        cout << "System exiting. Goodbye!" << endl;
        return 0;
    }
    
    // Add sample food items if the inventory is empty
    if (inventory.isEmpty()) {
        User* currentUser = authSystem.getCurrentUser();
        string creator = currentUser ? currentUser->username : "system";
        
        inventory.insert(FoodItem("R001", "Rice", 2.99, 50, "Staple", creator));
        inventory.insert(FoodItem("V001", "Carrots", 1.50, 30, "Vegetable", creator));
        inventory.insert(FoodItem("M001", "Chicken Breast", 5.99, 15, "Meat", creator));
        inventory.insert(FoodItem("S001", "Salt", 0.99, 100, "Seasoning", creator));
        inventory.insert(FoodItem("V002", "Onions", 1.25, 40, "Vegetable", creator));
        inventory.insert(FoodItem("M002", "Beef", 8.99, 10, "Meat", creator));
    }
    
    // Main application loop
    bool running = true;
    while (running) {
        // Run the main menu
        int menuResult = inventory.runMainMenu(authSystem);
        
        if (menuResult == 1) {
            // User menu selected
            authSystem.showUserMenu();
        }
        else if (menuResult == 0) {
            // Exit selected
            running = false;
        }
    }
    
    cout << "Thank you for using the Restaurant Inventory System!" << endl;
    return 0;
}
