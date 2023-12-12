#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>

using namespace std;

class FoodItem
{
public:
    string name;
    double price;
 
    FoodItem() : name(""), price(0.0) {}

    FoodItem(const string &itemName, double itemPrice)
        : name(itemName), price(itemPrice) {}
};

class OrderItem : public FoodItem
{
public:
    int quantity;

    OrderItem() : FoodItem(), quantity(0) {}

    OrderItem(const string &itemName, double itemPrice, int itemQuantity = 0)
        : FoodItem(itemName, itemPrice), quantity(itemQuantity) {}
};

class FoodOutlet
{
public:
    string name;
    vector<FoodItem> menu;

    FoodOutlet(const string &outletName, const vector<FoodItem> &outletMenu)
        : name(outletName), menu(outletMenu) {}
};

void printCurrentDate();

double calculateTotal(const OrderItem orderItems[], int itemCount)
{
    double total = 0.0;
    for (int i = 0; i < itemCount; ++i)
    {
        total += orderItems[i].price * orderItems[i].quantity;
    }
    return total;
}

void displayMenu(const FoodItem menuItems[], int itemCount)
{
    cout << "Menu:\n";
    for (int i = 0; i < itemCount; ++i)
    {
        cout << menuItems[i].name << " - Rs " << menuItems[i].price << "\n";
    }
}

void printBill(const OrderItem orderItems[], int orderItemCount)
{
    ofstream outFile("bill.txt"); // Open a file for writing

    outFile << "\n\t\t\tJSPM Food Court Bill";
    outFile << "\n\t\t  ----------------------------\n";
    printCurrentDate();
    outFile << "Item\t\tPrice/Unit\t   Quantity\tTotal\n";

    for (int i = 0; i < orderItemCount; ++i)
    {
        outFile << orderItems[i].name << "\t    Rs." << orderItems[i].price
                << "\t\t  " << orderItems[i].quantity
                << "\t\tRs." << orderItems[i].price * orderItems[i].quantity << "\n";
    }

    outFile << "\nTotal:\t\t\t\t\tRs." << calculateTotal(orderItems, orderItemCount) << "\n";
    outFile << "\n\tYour order will be ready in a few minutes!! \n\tPick up your order at the Food Outlet ";

    // Close the file
    outFile.close();

    // Print the bill to the console
    ifstream inFile("bill.txt");
    cout << inFile.rdbuf();
    inFile.close();
}

void printCurrentDate()
{
    time_t now = time(0);
    tm *currentDate = localtime(&now);
    cout << "\t\t\t  Date: " << currentDate->tm_mday << "/" << (currentDate->tm_mon + 1)
         << "/" << (currentDate->tm_year + 1900) << endl;
}

void cancelItemQuantity(OrderItem orderItems[], int &orderItemCount)
{
    string itemName;
    cout << "Enter the name of the item to cancel: ";
    cin >> itemName;

    int quantityToCancel;
    cout << "Enter the quantity to cancel: ";
    cin >> quantityToCancel;

    for (int i = 0; i < orderItemCount; ++i)
    {
        if (orderItems[i].name == itemName)
        {
            if (quantityToCancel >= orderItems[i].quantity)
            {
                for (int j = i; j < orderItemCount - 1; ++j) // Remove the entire item from the order
                {
                    orderItems[j] = orderItems[j + 1];
                }
                orderItemCount--;
            }
            else
            {
                orderItems[i].quantity -= quantityToCancel; // Reduce the quantity of the item
            }
            cout << "Quantity canceled successfully.\n";
            return;
        }
    }

    cout << "Item not found in the order.\n";
}
void displayMenuWithNumbers(const FoodItem menuItems[], int itemCount)
{
    cout << "Menu:\n";
    for (int i = 0; i < itemCount; ++i)
    {
        cout << i + 1 << ". " << menuItems[i].name << " - Rs " << menuItems[i].price << "\n";
    }
}

int main()
{
    const int outletCount = 4;

    // Define menus for each food outlet
    FoodOutlet outlets[outletCount] = {
        {"Burger King", {{"Cheese Burger", 50}, {"Momos", 70}, {"Chicken Burger", 60}}},
        {"Pizza Hut", {{"Margherita", 100}, {"Pepperoni", 120}, {"Vegetarian", 90}}},
        {"Kiyansh cafe", {{"Sandwich", 60}, {"Twisto", 30}, {"Maggie", 40}}},
        {"Samarth juice", {{"Oreo ", 80}, {"Apple", 100}, {"Watermelon", 120}}}};

    const int maxOrderItems = 10;
    OrderItem orderItems[maxOrderItems];
    int orderItemCount = 0;

    char choice;
    do
    {
        cout << "\n\t\t\tWelcome to Food Delivery App" << endl;
        cout << "Available Food Outlets:\n";
        for (int i = 0; i < outletCount; ++i)
        {
            cout << i + 1 << ". " << outlets[i].name << "\n";
        }

        int outletChoice;
        cout << "Select a Food Outlet (1-" << outletCount << "): ";
        cin >> outletChoice;

        if (outletChoice >= 1 && outletChoice <= outletCount)
        {
            do
            {
                displayMenuWithNumbers(outlets[outletChoice - 1].menu.data(), outlets[outletChoice - 1].menu.size());

                int itemNumber;
                cout << "\nEnter the number of the item you want to order: ";
                cin >> itemNumber;

                if (itemNumber >= 1 && itemNumber <= outlets[outletChoice - 1].menu.size())
                {
                    int quantity;
                    cout << "Enter quantity: ";
                    cin >> quantity;

                    int menuIndex = itemNumber - 1;
                    orderItems[orderItemCount++] = OrderItem(outlets[outletChoice - 1].menu[menuIndex].name,
                                                              outlets[outletChoice - 1].menu[menuIndex].price,
                                                              quantity);
                    cout << "Item added to your order.\n";

                    cout << "\nDo you want to order another item from the same Food Outlet? (y/n): ";
                    cin >> choice;
                }
                else
                {
                    cout << "Invalid item number.\n";
                }

            } while (tolower(choice) == 'y');
        }
        else
        {
            cout << "Invalid Food Outlet selection.\n";
        }

        cout << "\nDo you want to order from another Food Outlet? (y/n): ";
        cin >> choice;


    } while (tolower(choice) == 'y');

    while (tolower(choice) == 'n')
    {
        string itemName;
        cout << "\n\nDo you want to Cancel the order?(yes/no):";
        cin >> itemName;
        if (itemName == "yes")
        {
            cancelItemQuantity(orderItems, orderItemCount);
            continue;
        }
        else
        {
            break;
        }
    }

    if (orderItemCount == 0)
    {
        printBill(orderItems, orderItemCount);
        cout << "Your order is empty. Thank you for visiting!\n";
    }
    else
    {
        printBill(orderItems, orderItemCount);
    }

    return 0;
}
