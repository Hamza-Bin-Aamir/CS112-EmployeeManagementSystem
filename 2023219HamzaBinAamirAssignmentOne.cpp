// EMPLOYEE MANAGEMENT SYSTEM
// AUTHORED BY: Hamza Bin Aamir
// Tested on the following compilers: G++ and MSVC
// Minimum C++ version: C++11
#include <iostream> // cin, cout lives here
#include <vector> // dynamic arrays
#include <algorithm> // for the swap() function

using namespace std; // we dont have to keep doing std::cout, std::cin etc.

// -------------------- Preprocessed Constants -------------------- 
// You can undefine any of these to disable the features

#define CALCULATE_SALARY_WITH_OT_BONUS
//#undef CALCULATE_SALARY_WITH_OT_BONUS
#define ENABLE_SORTING
#undef ENABLE_SORTING

// -------------------- Custom Data Structures -------------------- 

// Employee data: collection of useful info on employees
struct Employee {
    // Identity
    int empID;
    string Name;

    // Classification
    string Position;

    // Compensation
    float hourlyRate;
    float hoursWorked;
    float overtimeHours;
    float bonus;
    float totalSalary;
};

// -------------------- Global Constants -------------------- 
// It is best practice to use global constants to allow easy adjusting
// of the code front-end without messing with the rest of the program
// Also helps readability

const int MenuOptionsCount = 5;
const string WelcomeMessage = "Welcome to EMS v0.0.1!";
const string MenuMessage = "Currently available menu options are: ";
const string InvalidMessage = "The last input was invalid, please try again.";
const string DisabledMessage = "This feature is disabled :(";
const string RequestChoice = "Please choose: ";
const string AllMenuOptions[MenuOptionsCount] = { "(A) Add a new employee",
                                                    "(X) Show all data",
                                                    "(D) Display specific employee",
                                                    "(S) Sort employees based on ID",
                                                    "(E) Exit" };
const string AddEmployeeMessage = "Adding new employee!";
const string AllDataMessage = "Showing all data";
const string NoMatchesMessage = "No employees with the given ID were found.";
const string SortMessage = "Sorting...";
const string SortSuccessMessage = "Sorted successfully!";
const string SearchMessage = "Please enter the ID of the employee you want to search for: ";

// Let's apply more of our knowledge from the course!
enum ErrorCodes { NO_ERRORS = 0, INVALID_INPUT, DISALLOWED_FEATURE, USED_EXAMPLE, SEARCH_NO_MATCHES };
// -------------------- Global Variables -------------------- 

// Second menu option is for debugging so will be disabled before submission
// This helps us handle disabling the sorting feature
// This variable is not a constant because we can dynamically enable or disable menu options
bool DisabledMenuOptions[MenuOptionsCount] = { false, true, false, false, false };

Employee example1;
vector<Employee> employees;

// -------------------- Function Prototypes -------------------- 
int AddEmployee();
int ShowAllData();
int SortEmployees();
int DisplaySpecificEmployee(int);
void AddExampleEmployee();
int SelectionSort(vector<Employee>&);

// -------------------- Function Declarations -------------------- 

int main()
{
    // ***** INITIALISE THE PROGRAM BEFORE MAIN LOOP *****

    // User Choice
    char choice;

    // Error Codes (for debugging)
    int returnCode = NO_ERRORS, searchTerm;

    // Example employees (useful for debugging)
    example1.Name = "Ma'am Beenish"; example1.empID = 2000123; example1.Position = "Instructor"; example1.hourlyRate = 6000; example1.hoursWorked = 40; example1.overtimeHours = 10; example1.bonus = 50000;

    // Enabling or disabling the sorting feature
    // #undef ENABLE_SORTING // Uncomment this line for easy disabling of the feature!
#   ifndef ENABLE_SORTING
        DisabledMenuOptions[3] = true;
#   endif
    cout << WelcomeMessage << endl;

    // We can use "break;" to exit the loop whenever we want
    while (true)
    {
        // ***** STEP ONE: SHOW THE MENU *****
        cout << MenuMessage << endl;

        // Loop through all the options
        for (int i = 0; i < MenuOptionsCount; i++) {
            // Print an option only if it isn't disabled
            if (!DisabledMenuOptions[i]) {
                cout << AllMenuOptions[i] << endl;
            }
        }

        // Actually get the user choice
        cout << RequestChoice;
        cin >> choice;

        // ***** STEP TWO: HANDLE THE USER INPUT *****
        choice = tolower(choice); // The choice is case insensitive

        // Check if user wants to exit
        if (choice == 'e') { break; }

        // Otherwise process user's choice
        switch (choice)
        {
            // Case (A)dd employee
        case 'a':
            // Just making sure the feature isnt disabled
            if (DisabledMenuOptions[0]) { cout << DisabledMessage << endl; returnCode = DISALLOWED_FEATURE; break; }
            returnCode = AddEmployee();
            break;
            // Case (X) Show all data
        case 'x':
            if (DisabledMenuOptions[1]) { cout << DisabledMessage << endl; returnCode = DISALLOWED_FEATURE; break; }
            returnCode = ShowAllData();
            break;
            // Case (D)isplay specific employee
        case 'd':
            if (DisabledMenuOptions[2]) { cout << DisabledMessage << endl; returnCode = DISALLOWED_FEATURE; break; }
            cout << SearchMessage;
            cin >> searchTerm;
            returnCode = DisplaySpecificEmployee(searchTerm);
            break;
            // Case (S)ort employees based on ID
        case 's':
            if (DisabledMenuOptions[3]) { cout << DisabledMessage << endl; returnCode = DISALLOWED_FEATURE; break; }
            returnCode = SortEmployees();
            returnCode = ShowAllData(); // We can repurpose our debugging function here 
            break;
            // Invalid inputs
        default:
            cout << InvalidMessage << endl;
            returnCode = INVALID_INPUT;
        }
    }

    // ***** RETURN RELEVANT ERROR CODE *****
    return returnCode;
}

int AddEmployee() {
    // ***** INITIALISATION *****
    Employee Curr;
    float totalSalary;
    // ***** STEP ONE: COLLECT DATA *****
    cout << AddEmployeeMessage << endl;
    cout << "Please enter the employee's name: "; cin >> Curr.Name;
    // For debugging and testing
    if (Curr.Name == "eg") { AddExampleEmployee(); return USED_EXAMPLE; }
    cout << "Please enter the employee's ID: "; cin >> Curr.empID;
    cout << "Pleas enter the employee's position: "; cin >> Curr.Position;
    cout << "Please enter the employee's hourly rate: "; cin >> Curr.hourlyRate;
    cout << "Please enter the employee's hours worked: "; cin >> Curr.hoursWorked;
#   ifdef CALCULATE_SALARY_WITH_OT_BONUS
        cout << "Please enter the employee's overtime hours: "; cin >> Curr.overtimeHours;
        cout << "Please enter the employee's bonus: "; cin >> Curr.bonus;
#   endif

    // ***** STEP TWO: CALCULATE TOTAL SALARY *****
    totalSalary = Curr.hourlyRate * Curr.hoursWorked;
#   ifdef CALCULATE_SALARY_WITH_OT_BONUS
        totalSalary += Curr.hourlyRate * 2 * Curr.overtimeHours; // Overtime is paid at twice the hourly rate
        totalSalary += Curr.bonus;
#   endif
    Curr.totalSalary = totalSalary;

    // ***** STEP THREE: PASSING DATA TO THE VECTOR *****
    employees.push_back(Curr);

    // ***** RETURN RELEVANT ERROR CODE *****
    return NO_ERRORS;
}

void AddExampleEmployee() {
    employees.push_back(example1);
    cout << "example employee added" << endl;
}

int ShowAllData() {
    // ***** INITIALISATION *****
    int len = employees.size();
    cout << AllDataMessage << endl;

    // ***** LOOP THROUGH ALL THE EMPLOYEES AND DISPLAY THEIR DATA
    for (int i = 0; i < len; i++) {
        cout << "----------- INFORMATION FOR " << employees[i].Name << " -----------" << endl;
        cout << "EMPLOYEE ID: " << employees[i].empID << endl;
        cout << "CURRENT POSITION: " << employees[i].Position << endl;
        cout << "-----" << endl << "TOTAL SALARY: " << employees[i].totalSalary << endl;
        cout << "HOURS WORKED: " << employees[i].hoursWorked << endl;
        cout << "HOURLY RATE: " << employees[i].hourlyRate << endl;
#       ifdef CALCULATE_SALARY_WITH_OT_BONUS
            cout << "OVERTIME HOURS: " << employees[i].overtimeHours << endl;
            cout << "BONUS: " << employees[i].bonus << endl;
#       endif
    }

    // ***** RETURN THE RELEVANT ERROR MESSAGE *****
    return NO_ERRORS;
}
int SortEmployees() {
    cout << SortMessage;
    SelectionSort(employees);
    cout << SortSuccessMessage;
    return NO_ERRORS;
}
int SelectionSort(vector<Employee> &Target) {
    // ***** INITIALISE *****
    // Save the value of target's size
    int len = Target.size();

    // ***** STEP TWO: REPEATEDLY CHECK FOR THE SMALLEST ID AND SWAP IT WITH THE EARLIEST ELEMENT *****
    int minIndex;
    for (int i = 0; i < len - 1; i++) {
        minIndex = i; // No swap if this element is in the correct place

        for (int j = i + 1; j < len; j++) {
            if (Target[j].empID < Target[minIndex].empID) {
                minIndex = j;
            }
        }

        // Swap the elements
        // Simple to understand sorting algorithm, thats why I used it xD
        swap(Target[i], Target[minIndex]);
    }

    return NO_ERRORS;
}

int DisplaySpecificEmployee(int Target) {
    // ***** INITIALISATION *****
    int len = employees.size();
    bool Matched = false;

    // ***** STEP ONE: LOOP THROUGH EMPLOYEE IDs AND DISPLAY EACH MATCH *****
    for (int i = 0; i < len; i++) {
        if (employees[i].empID == Target) {
			cout << "----------- INFORMATION FOR " << employees[i].Name << " -----------" << endl;
			cout << "EMPLOYEE ID: " << employees[i].empID << endl;
			cout << "CURRENT POSITION: " << employees[i].Position << endl;
			cout << "-----" << endl << "TOTAL SALARY: " << employees[i].totalSalary << endl;
			cout << "HOURS WORKED: " << employees[i].hoursWorked << endl;
			cout << "HOURLY RATE: " << employees[i].hourlyRate << endl;
#           ifdef CALCULATE_SALARY_WITH_OT_BONUS
                cout << "OVERTIME HOURS: " << employees[i].overtimeHours << endl;
                cout << "BONUS: " << employees[i].bonus << endl;
#           endif
            Matched = true;
        }
    }
    if (!Matched) {
		cout << NoMatchesMessage << endl;
        return SEARCH_NO_MATCHES;
	}

    // ***** RETURN THE RELEVANT ERROR MESSAGE *****
    return NO_ERRORS;
}