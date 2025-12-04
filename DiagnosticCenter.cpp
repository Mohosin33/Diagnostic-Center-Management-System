#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <ctime>
using namespace std;

class User {
protected:
    string username, password;
public:
    User() {}
    User(const string &u, const string &p) : username(u), password(p) {}
    virtual void menu() = 0;
    virtual ~User() {}
};

class Doctor : public User {
public:
    int id;
    string name;
    string department;
    double fee;
    static int doctorCount;
    Doctor() : id(0), name(""), department(""), fee(0.0) {}
    Doctor(int i, const string &n, const string &d, double f) : id(i), name(n), department(d), fee(f) {}
    ~Doctor() {}
    friend ostream& operator<<(ostream &out, const Doctor &d) {
        out << "ID: " << d.id << " | Name: " << d.name << " | Department: " << d.department << " | Fee: " << d.fee << " BDT";
        return out;
    }
    friend istream& operator>>(istream &in, Doctor &d) {
        cout << "Enter Doctor ID: "; in >> d.id; in.ignore();
        cout << "Enter Doctor Name: "; getline(in, d.name);
        cout << "Enter Department: "; getline(in, d.department);
        cout << "Enter Consultation Fee (BDT): "; in >> d.fee; in.ignore();
        return in;
    }
    bool operator==(const Doctor &other) const { return id == other.id; }
    double operator+(const Doctor &other) const { return fee + other.fee; }
    operator string() const { return name; }
    static void showDoctorCount() { cout << "Total Doctors in System: " << doctorCount << endl; }
    void menu() override {
        cout << "\n---------------------------------\n";
        cout << "     DOCTOR DASHBOARD        \n";
        cout << "---------------------------------\n";
        cout << "1. Show My Information\n";
        cout << "0. Back to Main Menu\n";
        cout << "---------------------------------\n";
        cout << "Enter your choice: ";
        int ch; cin >> ch;
        if (ch == 1) cout << "\n" << *this << endl;
    }
};
int Doctor::doctorCount = 0;

class Patient {
public:
    int serial;
    string name;
    string phone;
    string address;
    int age;
    int doctorId;
    bool processed;
    string date;
    bool paid;
    Patient() : serial(0), name(""), phone(""), address(""), age(0), doctorId(0), processed(false), date(""), paid(false) {}
    Patient(int s, const string &n, const string &ph, const string &ad, int a, int dId, const string &d, bool proc = false, bool p = false)
        : serial(s), name(n), phone(ph), address(ad), age(a), doctorId(dId), date(d), processed(proc), paid(p) {}
    friend ostream& operator<<(ostream &out, const Patient &p) {
        out << "Serial: " << p.serial
            << " | Name: " << p.name
            << " | Phone: " << p.phone
            << " | Address: " << p.address
            << " | Age: " << p.age
            << " | Doctor ID: " << p.doctorId
            << " | Date: " << p.date
            << " | " << (p.processed ? "Processed" : "Waiting")
            << " | " << (p.paid ? "Paid" : "Unpaid");
        return out;
    }
};

class Admin : public User {
public:
    Admin() {}
    Admin(const string &u, const string &p) : User(u,p) {}
    void addDoctor() {
        Doctor d; cin >> d;
        ifstream fin("doctors.txt");
        string line;
        bool exists = false;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            size_t idPos = line.find("ID: ");
            if (idPos != string::npos) {
                size_t namePos = line.find(" | Name: ");
                if (namePos != string::npos) {
                    int id = stoi(line.substr(idPos + 4, namePos - (idPos + 4)));
                    if (id == d.id) {
                        exists = true;
                        break;
                    }
                }
            }
        }
        fin.close();
        if (exists) {
            cout << "Doctor ID already exists! Cannot add duplicate doctor.\n";
            return;
        }
        ofstream fout("doctors.txt", ios::app);
        fout << d << '\n';
        fout.close();
        string docFile = "doctor_" + to_string(d.id) + ".txt";
        ifstream chk(docFile);
        if (!chk.is_open()) {
            ofstream create(docFile);
            create.close();
        } else chk.close();

        cout << "Doctor added successfully to the system!\n";
    }
    void removeDoctor() {
        int id;
        cout << "Enter Doctor ID to remove: ";
        cin >> id;
        ifstream fin("doctors.txt");
        if (!fin.is_open()) {
            cout << "doctors.txt file not found.\n";
            return;
        }
        vector<Doctor> remaining;
        string line;
        bool found = false;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            size_t idPos = line.find("ID: ");
            size_t namePos = line.find(" | Name: ");
            size_t deptPos = line.find(" | Department: ");
            size_t feePos = line.find(" | Fee: ");
            if (idPos == string::npos || namePos == string::npos ||
                deptPos == string::npos || feePos == string::npos) continue;
            int dId = stoi(line.substr(idPos + 4, namePos - (idPos + 4)));
            string name = line.substr(namePos + 9, deptPos - (namePos + 9));
            string dept = line.substr(deptPos + 15, feePos - (deptPos + 15));
            size_t bdtPos = line.find(" BDT", feePos);
            double fee = stod(line.substr(feePos + 8, bdtPos - (feePos + 8)));
            if (dId == id) {
                found = true;
                continue;
            }
            remaining.push_back(Doctor(dId, name, dept, fee));
        }
        fin.close();
        if (!found) {
            cout << "Doctor ID not found in the system!\n";
            return;
        }
        ofstream fout("doctors.txt");
        for (auto &d : remaining) fout << d << '\n';
        fout.close();
        string docFile = "doctor_" + to_string(id) + ".txt";
        if (remove(docFile.c_str()) == 0) {
        }
        cout << "Doctor removed successfully from the system!\n";
    }
    void seeDoctors() {
        ifstream fin("doctors.txt");
        if (!fin.is_open()) {
            cout << "No doctors found in the system.\n";
            return;
        }
        string line;
        cout << "\n================================================================\n";
        cout << "                    DOCTORS LIST                           \n";
        cout << "================================================================\n";
        while (getline(fin, line)) {
            if (line.empty()) continue;
            cout << "  " << line << '\n';
        }
        fin.close();
        cout << "================================================================\n";
    }
    void menu() override {
        int choice;
        do {
            cout << "\n========================================\n";
            cout << "       ADMIN CONTROL PANEL          \n";
            cout << "========================================\n";
            cout << "  1. View All Doctors\n";
            cout << "  2. Add New Doctor\n";
            cout << "  3. Remove Doctor\n";
            cout << "  0. Logout\n";
            cout << "========================================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            if (choice == 1) seeDoctors();
            else if (choice == 2) addDoctor();
            else if (choice == 3) removeDoctor();
        } while (choice != 0);
    }
};

class DiagnosticCenter {
    vector<Doctor> doctors;
public:
    DiagnosticCenter() { loadDoctors(); }
    static string doctorFilename(int did) {
        return "doctor_" + to_string(did) + ".txt";
    }
    template<typename T, typename Predicate>
    vector<T> filterVector(const vector<T>& vec, Predicate pred) {
        vector<T> result;
        for (const auto& item : vec) {
            if (pred(item)) {
                result.push_back(item);
            }
        }
        return result;
    }
    void loadDoctors() {
        doctors.clear();
        ifstream fin("doctors.txt");
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            size_t idPos = line.find("ID: ");
            size_t namePos = line.find(" | Name: ");
            size_t deptPos = line.find(" | Department: ");
            size_t feePos = line.find(" | Fee: ");
            if (idPos == string::npos || namePos == string::npos ||
                deptPos == string::npos || feePos == string::npos) continue;
            int id = stoi(line.substr(idPos + 4, namePos - (idPos + 4)));
            string name = line.substr(namePos + 9, deptPos - (namePos + 9));
            string dept = line.substr(deptPos + 15, feePos - (deptPos + 15));
            size_t bdtPos = line.find(" BDT", feePos);
            double fee = stod(line.substr(feePos + 8, bdtPos - (feePos + 8)));
            doctors.emplace_back(id, name, dept, fee);
        }
        fin.close();
        Doctor::doctorCount = (int)doctors.size();
    }
    bool doctorExists(int did) {
        return any_of(doctors.begin(), doctors.end(), [&](const Doctor &d){ return d.id == did; });
    }
    double getDoctorFee(int did) {
        for (auto &d : doctors) {
            if (d.id == did) return d.fee;
        }
        return 0.0;
    }
    vector<Patient> readDoctorPatients(int did) {
        vector<Patient> list;
        string file = doctorFilename(did);
        ifstream fin(file);
        if (!fin.is_open()) return list;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string sSerial, name, phone, address, sAge, date, sProcessed, sPaid;
            getline(ss, sSerial, '|');
            getline(ss, name, '|');
            getline(ss, phone, '|');
            getline(ss, address, '|');
            getline(ss, sAge, '|');
            getline(ss, date, '|');
            getline(ss, sProcessed, '|');
            getline(ss, sPaid, '|');
            if (sSerial.empty()) continue;
            try {
                int serial = stoi(sSerial);
                int age = sAge.empty() ? 0 : stoi(sAge);
                bool processed = (sProcessed == "1");
                bool paid = (sPaid == "1");
                list.emplace_back(serial, name, phone, address, age, did, date, processed, paid);
            } catch(...) {

                continue;
            }
        }
        fin.close();
        return list;
    }
    void writeDoctorPatients(int did, const vector<Patient> &list) {
        string file = doctorFilename(did);
        ofstream fout(file);
        for (auto &p : list) {
            fout << p.serial << "|" << p.name << "|" << p.phone << "|" << p.address << "|" << p.age << "|" << p.date << "|" << (p.processed ? "1" : "0") << "|" << (p.paid ? "1" : "0") << "\n";
        }
        fout.close();
    }
    int nextSerialForDoctor(int did, const string& date) {
        vector<Patient> list = readDoctorPatients(did);
        if (list.empty()) return 1;
        int maxSerial = 0;
        for (auto &p : list) {
            if (p.date == date && p.serial > maxSerial)
                maxSerial = p.serial;
        }
        return maxSerial + 1;
    }
    string getCurrentDate() {
        string date;
        cout << "Enter date (DD-MM-YYYY): ";
        cin >> date;
        return date;
    }
    string getRealDate() {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        string day = to_string(ltm->tm_mday);
        string month = to_string(1 + ltm->tm_mon);
        string year = to_string(1900 + ltm->tm_year);
        if (day.length() == 1) day = "0" + day;
        if (month.length() == 1) month = "0" + month;

        return day + "-" + month + "-" + year;
    }
    bool isDateBefore(const string &date1, const string &date2) {
        int d1 = stoi(date1.substr(0, 2));
        int m1 = stoi(date1.substr(3, 2));
        int y1 = stoi(date1.substr(6, 4));

        int d2 = stoi(date2.substr(0, 2));
        int m2 = stoi(date2.substr(3, 2));
        int y2 = stoi(date2.substr(6, 4));

        if (y1 != y2) return y1 < y2;
        if (m1 != m2) return m1 < m2;
        return d1 < d2;
    }
    void doctorSignup() {
        string u, p;
        int did;
        cout << "Enter Doctor ID: "; cin >> did;
        loadDoctors();
        if (!doctorExists(did)) {
            cout << "Doctor ID not found! Please contact admin.\n";
            return;
        }
        cout << "Choose Username: "; cin >> u;
        cout << "Choose Password: "; cin >> p;
        ofstream fout("doctor_login.txt", ios::app);
        fout << did << " " << u << " " << p << "\n";
        fout.close();
        cout << "Doctor account created successfully!\n";
    }
    int doctorLoginPrompt() {
        string u, p;
        cout << "Username: "; cin >> u;
        cout << "Password: "; cin >> p;
        ifstream fin("doctor_login.txt");
        if (!fin.is_open()) {
            cout << "No doctor accounts found. Please signup first.\n";
            return -1;
        }
        int did;
        string user, pass;
        while (fin >> did >> user >> pass) {
            if (user == u && pass == p) {
                fin.close();
                return did;
            }
        }
        fin.close();
        return -1;
    }
    void adminSignup() {
        string u,p; cout << "Choose Admin Username: "; cin >> u;
        cout << "Choose Password: "; cin >> p;
        ofstream fout("admin_login.txt", ios::app);
        fout << u << " " << p << "\n";
        fout.close();
        cout << "Admin signed up successfully!\n";
    }
    bool adminLoginPrompt() {
        string u,p; cout << "Username: "; cin >> u; cout << "Password: "; cin >> p;
        ifstream fin("admin_login.txt");
        if (!fin.is_open()) { cout << "No admin found. Please signup first.\n"; return false; }
        string user, pass;
        while (fin >> user >> pass) {
            if (user == u && pass == p) { fin.close(); return true; }
        }
        fin.close();
        return false;
    }
    void patientMenu() {
        int choice;
        do {
            cout << "\n--- Patient Menu ---\n1. Get Serial\n2. See Serials (by Doctor)\n3. Cancel Serial\n0. Back\nChoice: ";
            cin >> choice;
            if (choice == 1) {
                int did; cout << "Enter Doctor ID to get serial for: "; cin >> did;
                loadDoctors();
                if (!doctorExists(did)) { cout << "Doctor ID not found!\n"; continue; }
                string date = getCurrentDate();
                string today = getRealDate();
                if (isDateBefore(date, today)) {
                    cout << "Error: Cannot book serial for past dates! Today's date is " << today << "\n";
                    continue;
                }
                cin.ignore();
                string name, phone, address;
                int age;
                cout << "Name: "; getline(cin, name);
                cout << "Phone: "; getline(cin, phone);
                cout << "Address: "; getline(cin, address);
                cout << "Age: "; cin >> age;
                int serial = nextSerialForDoctor(did, date);
                Patient p(serial, name, phone, address, age, did, date, false);
                string file = doctorFilename(did);
                ofstream fout(file, ios::app);
                fout << p.serial << "|" << p.name << "|" << p.phone << "|" << p.address << "|" << p.age << "|" << p.date << "|0|0\n";
                fout.close();
                cout << "Your serial for Doctor " << did << " on " << date << " is: " << serial << "\n";
                cout << "Doctor fee: " << getDoctorFee(did) << " BDT\n";
            }
            else if (choice == 2) {
                int did; cout << "Enter Doctor ID to view serials: "; cin >> did;
                loadDoctors();
                if (!doctorExists(did)) { cout << "Doctor ID not found!\n"; continue; }
                string date = getCurrentDate();
                vector<Patient> list = readDoctorPatients(did);
                vector<Patient> dateFiltered = filterVector(list, [&](const Patient& p) {
                    return p.date == date;
                });
                if (dateFiltered.empty()) {
                    cout << "No patients for Doctor " << did << " on " << date << ".\n";
                    continue;
                }
                cout << "--- Serials for Doctor " << did << " on " << date << " ---\n";
                for (auto &p : dateFiltered) cout << p << '\n';
            }
            else if (choice == 3) {
                int did; cout << "Enter Doctor ID: "; cin >> did;
                loadDoctors();
                if (!doctorExists(did)) { cout << "Doctor ID not found!\n"; continue; }
                string date = getCurrentDate();
                vector<Patient> allPatients = readDoctorPatients(did);
                vector<Patient> datePatients = filterVector(allPatients, [&](const Patient& p) {
                    return p.date == date && !p.processed;
                });
                if (datePatients.empty()) {
                    cout << "No waiting patients to cancel for Doctor " << did << " on " << date << ".\n";
                    continue;
                }
                cout << "--- Your Waiting Serials for Doctor " << did << " on " << date << " ---\n";
                for (auto &p : datePatients) cout << p << "\n";
                int s;
                cout << "Enter Serial Number to Cancel: ";
                cin >> s;
                bool found = false;
                for (size_t i = 0; i < allPatients.size(); ++i) {
                    if (allPatients[i].serial == s && allPatients[i].date == date && !allPatients[i].processed) {
                        Patient removed = allPatients[i];
                        allPatients.erase(allPatients.begin() + i);
                        writeDoctorPatients(did, allPatients);
                        cout << "Successfully cancelled serial " << removed.serial << " for " << removed.name << ".\n";
                        found = true;
                        break;
                    }
                }
                if (!found) cout << "Serial not found or already processed!\n";
            }
        } while (choice != 0);
    }
    void compounderMenu() {
        int did;
        cout << "\nEnter Doctor ID for Compounder Desk: ";
        cin >> did;
        loadDoctors();
        if (!doctorExists(did)) {
            cout << "Doctor ID not found in the system!\n";
            return;
        }
        double doctorFee = getDoctorFee(did);
        int choice;
        do {
            string date = getRealDate();
            cout << "\n========================================\n";
            cout << "    COMPOUNDER DESK - Dr." << did << "        \n";
            cout << "    Today's Date: " << date << "\n";
            cout << "========================================\n";
            cout << "  1. Process Next Serial\n";
            cout << "  2. Cancel a Serial\n";
            cout << "  3. View Waiting Queue\n";
            cout << "  4. Collect Payment\n";
            cout << "  5. View Payment Summary\n";
            cout << "  0. Back\n";
            cout << "========================================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            if (choice >= 1 && choice <= 5) {
                vector<Patient> allPatients = readDoctorPatients(did);
                vector<Patient> datePatients = filterVector(allPatients, [&](const Patient& p) {
                    return p.date == date;
                });
                if (choice == 1) {
                    bool found = false;
                    for (auto &p : allPatients) {
                        if (p.date == date && !p.processed) {
                            cout << "Processing patient: " << p << "\n";
                            p.processed = true;
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        cout << "No patients waiting for the doctor on " << date << ".\n";
                    } else {
                        writeDoctorPatients(did, allPatients);
                        cout << "Serial processed.\n";
                    }
                }
                else if (choice == 2) {
                    vector<Patient> waiting = filterVector(datePatients, [&](const Patient& p) {
                        return !p.processed;
                    });
                    if (waiting.empty()) {
                        cout << "No patients to cancel on " << date << ".\n";
                        continue;
                    }
                    cout << "--- Waiting patients for Doctor " << did << " on " << date << " ---\n";
                    for (auto &p : waiting) cout << p << "\n";
                    int s;
                    cout << "Enter Serial Number to Cancel: ";
                    cin >> s;
                    bool found = false;
                    for (size_t i = 0; i < allPatients.size(); ++i) {
                        if (allPatients[i].serial == s && allPatients[i].date == date && !allPatients[i].processed) {
                            Patient removed = allPatients[i];
                            allPatients.erase(allPatients.begin() + i);
                            writeDoctorPatients(did, allPatients);
                            cout << "Cancelled serial " << removed.serial << " for " << removed.name << ".\n";
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Serial not found or already processed!\n";
                }
                else if (choice == 3) {
                    if (datePatients.empty()) {
                        cout << "No patients for this doctor on " << date << ".\n";
                        continue;
                    }
                    vector<Patient> waiting = filterVector(datePatients, [&](const Patient& p) {
                        return !p.processed;
                    });
                    cout << "--- Waiting patients for Doctor " << did << " on " << date << " ---\n";
                    for (auto &p : waiting) cout << p << "\n";
                }
                else if (choice == 4) {
                    vector<Patient> processedUnpaid = filterVector(datePatients, [&](const Patient& p) {
                        return p.processed && !p.paid;
                    });
                    if (processedUnpaid.empty()) {
                        cout << "No unpaid processed patients on " << date << ".\n";
                        continue;
                    }
                    cout << "--- Processed Unpaid Patients for Doctor " << did << " on " << date << " ---\n";
                    for (auto &p : processedUnpaid) {
                        cout << p << " | Fee: " << doctorFee << " BDT\n";
                    }
                    int s;
                    cout << "Enter Serial Number to collect payment: ";
                    cin >> s;
                    bool found = false;
                    for (auto &p : allPatients) {
                        if (p.serial == s && p.date == date && p.processed && !p.paid) {
                            p.paid = true;
                            writeDoctorPatients(did, allPatients);
                            cout << "Payment of " << doctorFee << " BDT collected from " << p.name << " (Serial: " << s << ").\n";
                            found = true;
                            break;
                        }
                    }
                    if (!found) cout << "Serial not found, not processed, or already paid!\n";
                }
                else if (choice == 5) {
                    int totalPatients = 0;
                    int paidPatients = 0;
                    int unpaidPatients = 0;
                    double totalRevenue = 0.0;
                    double pendingRevenue = 0.0;

                    for (auto &p : datePatients) {
                        if (p.processed) {
                            totalPatients++;
                            if (p.paid) {
                                paidPatients++;
                                totalRevenue += doctorFee;
                            } else {
                                unpaidPatients++;
                                pendingRevenue += doctorFee;
                            }
                        }
                    }
                    cout << "\n--- Payment Summary for Doctor " << did << " on " << date << " ---\n";
                    cout << "Doctor Fee: " << doctorFee << " BDT\n";
                    cout << "Total Processed Patients: " << totalPatients << "\n";
                    cout << "Paid Patients: " << paidPatients << "\n";
                    cout << "Unpaid Patients: " << unpaidPatients << "\n";
                    cout << "Total Revenue Collected: " << totalRevenue << " BDT\n";
                    cout << "Pending Revenue: " << pendingRevenue << " BDT\n";
                    cout << "Expected Total Revenue: " << (totalRevenue + pendingRevenue) << " BDT\n";
                }
            }
        } while (choice != 0);
    }
    void doctorMenuAfterLogin(int did) {
        double doctorFee = getDoctorFee(did);
        int choice;
        do {
            string date = getRealDate();
            cout << "\n--- Doctor Menu (Doctor ID: " << did << ") ---\n";
            cout << "Today's Date: " << date << "\n";
            cout << "1. See All Patients Serial\n";
            cout << "2. See Waiting Patients Serial\n";
            cout << "3. View Revenue Report\n";
            cout << "0. Logout\n";
            cout << "Choice: ";
            cin >> choice;
            if (choice >= 1 && choice <= 3) {
                vector<Patient> allPatients = readDoctorPatients(did);
                vector<Patient> datePatients = filterVector(allPatients, [&](const Patient& p) {
                    return p.date == date;
                });
                if (choice == 1) {
                    if (datePatients.empty()) {
                        cout << "No patients found for " << date << ".\n";
                    } else {
                        cout << "\n--- All Patients for Doctor " << did << " on " << date << " ---\n";
                        for (auto &p : datePatients) {
                            cout << p << "\n";
                        }
                        cout << "Total patients: " << datePatients.size() << "\n";
                    }
                }
                else if (choice == 2) {
                    vector<Patient> waiting = filterVector(datePatients, [&](const Patient& p) {
                        return !p.processed;
                    });
                    if (waiting.empty()) {
                        cout << "No waiting patients on " << date << ".\n";
                    } else {
                        cout << "\n--- Waiting Patients for Doctor " << did << " on " << date << " ---\n";
                        for (auto &p : waiting) {
                            cout << p << "\n";
                        }
                        cout << "Total waiting: " << waiting.size() << "\n";
                    }
                }
                else if (choice == 3) {
                    int totalPatients = 0;
                    int paidPatients = 0;
                    int unpaidPatients = 0;
                    double totalRevenue = 0.0;
                    double pendingRevenue = 0.0;
                    for (auto &p : datePatients) {
                        if (p.processed) {
                            totalPatients++;
                            if (p.paid) {
                                paidPatients++;
                                totalRevenue += doctorFee;
                            } else {
                                unpaidPatients++;
                                pendingRevenue += doctorFee;
                            }
                        }
                    }
                    cout << "\n--- Revenue Report for Doctor " << did << " on " << date << " ---\n";
                    cout << "Your Fee per Patient: " << doctorFee << " BDT\n";
                    cout << "Total Processed Patients: " << totalPatients << "\n";
                    cout << "Paid Patients: " << paidPatients << "\n";
                    cout << "Unpaid Patients: " << unpaidPatients << "\n";
                    cout << "Total Revenue Collected: " << totalRevenue << " BDT\n";
                    cout << "Pending Revenue: " << pendingRevenue << " BDT\n";
                    cout << "Expected Total Revenue: " << (totalRevenue + pendingRevenue) << " BDT\n";
                    cout << "\nNote: Payment collection is managed by compounder.\n";
                }
            }
        } while (choice != 0);
    }
    void doctorFlow() {
        int did = doctorLoginPrompt();
        if (did != -1) {
            cout << "Login successful! Welcome Doctor " << did << "\n";
            doctorMenuAfterLogin(did);
        } else {
            cout << "Invalid credentials.\n";
        }
    }
    void adminFlow() {
        if (adminLoginPrompt()) {
            Admin a;
            a.menu();
            loadDoctors();
        } else {
            cout << "Invalid admin credentials.\n";
        }
    }
    void showDoctorsMain() {
        loadDoctors();
        if (doctors.empty()) { cout << "No doctors available.\n"; return; }
        cout << "--- Doctors ---\n";
        for (auto &d : doctors) cout << d << '\n';
    }
    void mainMenu() {
        loadDoctors();
        int choice;
        do {
            cout << "\n=== Diagnostic Center System ===\n";
            cout << "1. Admin Login\n";
            cout << "2. Admin Signup\n";
            cout << "3. Compounder\n";
            cout << "4. Patient\n";
            cout << "5. Doctor Login\n";
            cout << "6. Doctor Signup\n";
            cout << "7. See Doctors\n";
            cout << "0. Exit\n";
            cout << "Choice: ";
            cin >> choice;
            switch (choice) {
                case 1: adminFlow(); break;
                case 2: adminSignup(); break;
                case 3: compounderMenu(); break;
                case 4: patientMenu(); break;
                case 5: doctorFlow(); break;
                case 6: doctorSignup(); break;
                case 7: showDoctorsMain(); break;
                case 0: cout << "Exiting...\n"; break;
                default: cout << "Invalid option.\n"; break;
            }
        } while (choice != 0);
    }
};

int main() {
    DiagnosticCenter dc;
    dc.mainMenu();
    return 0;
}
