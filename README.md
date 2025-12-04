# Diagnostic Center Management System

A **C++ console-based project** implementing **Object-Oriented Programming (OOP)** concepts to manage doctors, patients, and compounder operations in a diagnostic center. 

---

## 🖥️ Features

### Admin
- Add new doctors
- Remove doctors
- View all doctors

### Doctor
- Login with credentials
- View all patients for the day
- View waiting patients
- View revenue report

### Compounder
- Process next serial
- Cancel serials
- View waiting queue
- Collect payment from patients
- View payment summary

### Patient
- Get a serial for a doctor
- View serials by doctor and date
- Cancel a serial

---

## 🧩 OOP Concepts Used

- **Classes & Objects**: `User`, `Admin`, `Doctor`, `Patient`, `DiagnosticCenter`
- **Inheritance**: `Admin` and `Doctor` inherit from `User`
- **Polymorphism**: `menu()` function is **virtual** and overridden in derived classes
- **Encapsulation**: Data members are protected/private; accessed via methods
- **Operator Overloading**: `<<`, `>>`, `==`, `+`, and type conversion in `Doctor` class
- **Templates**: `filterVector` function to filter patients dynamically
- **File Handling**: Persistent storage for doctors, patients, and login credentials
- **Vectors & STL**: Storing and managing collections of doctors and patients
- **Date Handling**: Checking and comparing dates for booking serials

---

