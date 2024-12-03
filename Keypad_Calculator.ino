#include <Key.h>
#include <Keypad.h>

const int ROW_NUM = 4; // four rows
const int COLUMN_NUM = 4; // four columns

// Keypad layout
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'}, 
  {'4', '5', '6', 'B'}, 
  {'7', '8', '9', 'C'}, 
  {'*', '0', '#', 'D'}
};

// Pins for rows and columns
byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; // row pin connections
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; // column pin connections

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

String num1 = "";  // First number input
String num2 = "";  // Second number input
char operation = '\0';  // Operator (+, -, *, /)
bool isSecondNum = false;  // To switch to second number input
bool decimalAdded = false; // Track if decimal point is already added

void setup() {
  Serial.begin(9600);
  Serial.println("Enter numbers and operators:");
}

void loop() {
  char key = keypad.getKey();  // Get keypress from keypad

  if (key) {
    // If the key is a number (0-9) or a decimal point ('.')
    if ((key >= '0' && key <= '9') || (key == '*' && !decimalAdded)) {
      if (key == '*') { //press * key to give decimal values to the numbers
        key = '.';  // Replace '*' with actual decimal point '.'
        decimalAdded = true;  // Set flag once decimal is added
      }

      if (!isSecondNum) {
        num1 += key;  // Append the digit or decimal point to num1
      } else {
        num2 += key;  // Append the digit or decimal point to num2
      }
      Serial.print(key);  // Show the input
    }

    // If the key is an operator (+, -, *, /)
    if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      if (key == 'A') {
        operation = '+';  // Addition
      } else if (key == 'B') {
        operation = '-';  // Subtraction
      } else if (key == 'C') {
        operation = '*';  // Multiplication
      } else if (key == 'D') {
        operation = '/';  // Division
      }

      Serial.print(operation);
      isSecondNum = true;  // Switch to second number input
      decimalAdded = false;  // Reset decimal flag for the second number
    }

    // If '#' is pressed, perform the calculation
    if (key == '#') {
      performCalculation();  // Call the function to perform the calculation
      clearAll();  // Reset all values for the next calculation
    }
  }
}

// Function to perform the calculation based on inputs
void performCalculation() {
  float number1 = num1.toFloat();  // Convert num1 to float
  float number2 = num2.toFloat();  // Convert num2 to float
  float result = 0;

  if (operation == '+') {
    result = number1 + number2;
  } else if (operation == '-') {
    result = number1 - number2;
  } else if (operation == '*') {
    result = number1 * number2;
  } else if (operation == '/') {
    if (number2 != 0) {
      result = number1 / number2;  // Perform division only if num2 is not 0
    } else {
      Serial.println("Error: Division by zero");
      return;
    }
  }

  // Display the result
  Serial.print("\nResult: ");
  Serial.println(result, 2);  // Print result with 2 decimal places
}

// Function to clear all inputs
void clearAll() {
  num1 = "";
  num2 = "";
  operation = '\0';
  isSecondNum = false;
  decimalAdded = false;  // Reset decimal flag
}
