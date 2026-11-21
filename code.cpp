#include<iostream>
#include<string>
#include<vector>
#include<stdexcept>
#include<fstream>
using namespace std;

//CLASSES

class User{
    private :
    int id;
    string name;
    string password;
    string username;
    public :
    
    User(int id, string name, string username, string password) {
        this->id = id;
        this->name = name;
        this->username = username;
        this->password = password;
    }

    virtual ~User() {}

    int getId() { return id; }
    string getName() { return name; }
    string getUsername() { return username; }
    string getPassword() { return password; }
    bool checkPassword(string p) { return password == p; }
    
    virtual void displayMenu() = 0;

};
class Doctor : public User{
    private :
    string specialization;

    public : 
    Doctor(int id, string name, string username, string password, string spec)
        : User(id, name, username, password) {
        specialization = spec;
    }
    string getSpecialization() {return specialization ;}

    void displayMenu() {
        cout << "\n--- DOCTOR MENU (Dr. " << getName() << ") ---\n";
        cout << "1. View my appointments\n";
        cout << "2. View my slots for a date\n";
        cout << "3. Logout\n";
    }

};
class Patient : public User {
private:
    int age;

public:
    Patient(int id, string name, string username, string password, int age)
        : User(id, name, username, password) {
        this->age = age;
    }

    int getAge() { return age; }

    void displayMenu() {
        cout << "\n--- PATIENT MENU (" << getName() << ") ---\n";
        cout << "1. Search doctors\n";
        cout << "2. View doctor slots for a date\n";
        cout << "3. Book appointment\n";
        cout << "4. Cancel appointment\n";
        cout << "5. My appointment history\n";
        cout << "6. Logout\n";
    }
};
class Admin : public User {
public:
    Admin(int id, string name, string username, string password)
        : User(id, name, username, password) {}

    void displayMenu() {
        cout << "\n--- ADMIN MENU ---\n";
        cout << "1. Add doctor\n";
        cout << "2. View all doctors\n";
        cout << "3. Delete doctor\n";
        cout << "4. Logout\n";
    }
};
class Appointment {
private:
    int id;
    int patientId;      
    int doctorId;
    string date;
    string slot;
    bool active;

public:
    Appointment(int id, int patientId, int doctorId, string date, string slot, bool active) {
        this->id = id;
        this->patientId = patientId;
        this->doctorId = doctorId;
        this->date = date;
        this->slot = slot;
        this->active = active;
    }

    int getId() { return id; }
    int getPatientId() { return patientId; }
    int getDoctorId() { return doctorId; }
    string getDate() { return date; }
    string getSlot() { return slot; }
    bool isActive() { return active; }
    void cancel() { active = false; }
};

// STL

vector<Doctor> doctors;
vector<Patient> patients;
vector<Appointment> appointments;
Admin admin(0,"Admin","admin","admin123");

int nextDoctorId = 1;
int nextPatientId = 1;
int nextApptId = 1;

const int SLOT_COUNT = 5;
string slots[SLOT_COUNT] = {"10:00", "11:00", "12:00", "14:00", "15:00"};


// INPUT HEPLERS(throws exception instead crashing)


string readWord(){
    string s;
    if(!(cin >> s)) exit(0);
    cin.ignore(1000,'\n');
    return s;
}

int readInt(){
    int x;
    if(!(cin >> x)) {
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(1000,'\n');
        throw runtime_error("PLEASE ENTER A NUMBER\n");
    }
    cin.ignore(1000,'\n');
    return x;
}

// FILE HANDELING(WILL BE DONE LATER PROPERLY)

void saveAll() {

    // Save doctor records
    ofstream doctorFile("doctors.txt");

    for (int i = 0; i < doctors.size(); i++) {
        doctorFile << doctors[i].getId() << " "
                   << doctors[i].getName() << " "
                   << doctors[i].getUsername() << " "
                   << doctors[i].getPassword() << " "
                   << doctors[i].getSpecialization() << "\n";
    }

    doctorFile.close();


    // Save patient records
    ofstream patientFile("patients.txt");

    for (int i = 0; i < patients.size(); i++) {
        patientFile << patients[i].getId() << " "
                    << patients[i].getName() << " "
                    << patients[i].getUsername() << " "
                    << patients[i].getPassword() << " "
                    << patients[i].getAge() << "\n";
    }

    patientFile.close();


    // Save appointment records
    ofstream appointmentFile("appointments.txt");

    for (int i = 0; i < appointments.size(); i++) {
        appointmentFile << appointments[i].getId() << " "
                        << appointments[i].getPatientId() << " "
                        << appointments[i].getDoctorId() << " "
                        << appointments[i].getDate() << " "
                        << appointments[i].getSlot() << " "
                        << appointments[i].isActive() << "\n";
    }

    appointmentFile.close();
}

void loadAll() {

    // Load doctor records
    ifstream doctorFile("doctors.txt");

    if (doctorFile.is_open()) {
        int id;
        string name, username, password, spec;

        while (doctorFile >> id >> name >> username >> password >> spec) {
            doctors.push_back(Doctor(id, name, username, password, spec));

            if (id >= nextDoctorId)
                nextDoctorId = id + 1;
        }

        doctorFile.close();
    }
    else {
        doctors.push_back(Doctor(nextDoctorId++, "Meera_Sharma", "meera", "doc123", "Cardiology"));
        doctors.push_back(Doctor(nextDoctorId++, "Arjun_Singh", "arjun", "doc123", "Dermatology"));
        doctors.push_back(Doctor(nextDoctorId++, "Kavita_Rao", "kavita", "doc123", "Pediatrics"));

        saveAll();
    }


    // Load patient records
    ifstream patientFile("patients.txt");

    int pid, age;
    string pname, username, password;

    while (patientFile >> pid >> pname >> username >> password >> age) {
        patients.push_back(Patient(pid, pname, username, password, age));

        if (pid >= nextPatientId)
            nextPatientId = pid + 1;
    }

    patientFile.close();


    // Load appointment records
    ifstream appointmentFile("appointments.txt");

    int aid, patientId, doctorId, active;
    string date, slot;

    while (appointmentFile >> aid >> patientId >> doctorId >> date >> slot >> active) {
        appointments.push_back(Appointment(aid, patientId, doctorId, date, slot, active));

        if (aid >= nextApptId)
            nextApptId = aid + 1;
    }

    appointmentFile.close();
}

//HELPER FUNCTIONS


string toLower(string s){                        //converts to lowwer case letters
    for(int i =0;i<s.size();i++){
        s[i] = tolower(s[i]);
    }
    return s;
}
bool contains(string text,string key){
    return toLower(text).find(toLower(key)) != string :: npos;
}
int findDoctorIndex(int id){
    for (int i = 0; i < doctors.size(); i++){
        if(doctors[i].getId() == id){
            return i;
        }
    }return -1;
}
string doctorName(int id){
    int i = findDoctorIndex(id);
    if(i == -1)return "(REMOVED DOCTOR)";
    return "DR. " + doctors[i].getName();
}
string patientName(int id){
    for (int i =0; i<patients.size(); i++){
        if(patients[i].getId()== id) return patients[i].getName();
        
    }return "(unknown patient)";
}

void printDoctor(Doctor& d) {
    cout << "  ID " << d.getId() << " | Dr. " << d.getName()
         << " | " << d.getSpecialization() << "\n";
}

bool usernameTaken(string u){
    if (admin.getUsername() == u )return true;
    for(int i = 0; i<doctors.size();i++){
        if(doctors[i].getUsername() == u) return true;
    }
    for (int i = 0; i < patients.size(); i++)
        if (patients[i].getUsername() == u) return true;
    return false;
}

//two versions of searchDoctors()
void searchDoctors(string keyword) {                    // name OR specialization
    bool found = false;
    for (int i = 0; i < doctors.size(); i++) {
        if (contains(doctors[i].getName(), keyword) ||
            contains(doctors[i].getSpecialization(), keyword)) {
            printDoctor(doctors[i]);
            found = true;
        }
    }
    if (!found) cout << "  No doctors found.\n";
}

void searchDoctors(string name, string spec) {          // name AND specialization
    bool found = false;
    for (int i = 0; i < doctors.size(); i++) {
        if (contains(doctors[i].getName(), name) &&
            contains(doctors[i].getSpecialization(), spec)) {
            printDoctor(doctors[i]);
            found = true;
        }
    }
    if (!found) cout << "  No doctors found.\n";
}
bool isValidSlot(string slot) {
    for (int i = 0; i < SLOT_COUNT; i++)
        if (slots[i] == slot) return true;
    return false;
}

bool isSlotTaken(int doctorId, string date, string slot) {
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments[i].isActive() && appointments[i].getDoctorId() == doctorId &&
            appointments[i].getDate() == date && appointments[i].getSlot() == slot)
            return true;
    }
    return false;
}

void showSlots(int doctorId, string date) {
    if (findDoctorIndex(doctorId) == -1) throw runtime_error("Doctor not found.");
    cout << "\nSlots for " << doctorName(doctorId) << " on " << date << ":\n";
    for (int i = 0; i < SLOT_COUNT; i++) {
        cout << "  " << slots[i];
        if (isSlotTaken(doctorId, date, slots[i])) cout << "  [BOOKED]\n";
        else cout << "  [free]\n";
    }
}



int bookAppointment(int patientId, int doctorId, string date, string slot) {
    if (findDoctorIndex(doctorId) == -1) throw runtime_error("Doctor not found.");
    if (!isValidSlot(slot)) throw runtime_error("Invalid slot. Use a time from the list.");
    if (isSlotTaken(doctorId, date, slot)) throw runtime_error("Slot is not available.");

    int id = nextApptId++;
    appointments.push_back(Appointment(id, patientId, doctorId, date, slot, true));
    saveAll();
    return id;
}

void cancelAppointment(int apptId, int patientId) {
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments[i].getId() == apptId &&
            appointments[i].getPatientId() == patientId &&
            appointments[i].isActive()) {
            appointments[i].cancel();
            saveAll();
            return;
        }
    }
    throw runtime_error("No active appointment with that number found for you.");
}

User* login(int role, string username, string password) {
    if (role == 1) {
        for (int i = 0; i < patients.size(); i++)
            if (patients[i].getUsername() == username && patients[i].checkPassword(password))
                return &patients[i];
    } else if (role == 2) {
        for (int i = 0; i < doctors.size(); i++)
            if (doctors[i].getUsername() == username && doctors[i].checkPassword(password))
                return &doctors[i];
    } else if (role == 3) {
        if (admin.getUsername() == username && admin.checkPassword(password))
            return &admin;
    }
    throw runtime_error("Wrong username or password.");
}
void deleteDoctor(int id) {
    int index = findDoctorIndex(id);
    if (index == -1) throw runtime_error("Doctor not found.");
    for (int i = 0; i < appointments.size(); i++)      // cancel his appointments
        if (appointments[i].getDoctorId() == id) appointments[i].cancel();
    doctors.erase(doctors.begin() + index);
    saveAll();
}


//menu


void runPatient(User* u) {
    int pid = u->getId();
    while (true) {
        try {
            u->displayMenu();                       
            cout << "Choice: ";
            int c = readInt();

            if (c == 1) {
                cout << "1. Search by one keyword (name or specialization)\n";
                cout << "2. Search by name AND specialization\n";
                cout << "Choice: ";
                int s = readInt();
                if (s == 1) {
                    cout << "Keyword: ";
                    searchDoctors(readWord());
                } else if (s == 2) {
                    cout << "Name: ";
                    string n = readWord();
                    cout << "Specialization: ";
                    string sp = readWord();
                    searchDoctors(n, sp);
                } else {
                    throw runtime_error("Choose 1 or 2.");
                }
            } else if (c == 2) {
                cout << "Doctor ID: ";
                int did = readInt();
                cout << "Date (e.g. 15-03-2026): ";
                showSlots(did, readWord());
            } else if (c == 3) {
                cout << "Doctor ID: ";
                int did = readInt();
                cout << "Date (e.g. 15-03-2026): ";
                string date = readWord();
                showSlots(did, date);
                cout << "Choose slot (e.g. 10:00): ";
                string slot = readWord();
                int id = bookAppointment(pid, did, date, slot);
                cout << "Appointment booked! Your appointment number is #" << id << "\n";
            } else if (c == 4) {
                cout << "Appointment number to cancel: ";
                int aid = readInt();
                cancelAppointment(aid, pid);
                cout << "Appointment cancelled.\n";
            } else if (c == 5) {
                cout << "\nYour appointment history:\n";
                bool any = false;
                for (int i = 0; i < appointments.size(); i++) {
                    if (appointments[i].getPatientId() == pid) {
                        cout << "  #" << appointments[i].getId() << " | "
                             << appointments[i].getDate() << " " << appointments[i].getSlot()
                             << " | " << doctorName(appointments[i].getDoctorId())
                             << " | " << (appointments[i].isActive() ? "Active" : "Cancelled") << "\n";
                        any = true;
                    }
                }
                if (!any) cout << "  No appointments yet.\n";
            } else if (c == 6) {
                cout << "Logged out.\n";
                return;
            } else {
                throw runtime_error("Choose a number from 1 to 6.");
            }
        } catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}
void runDoctor(User* u) {
    int did = u->getId();
    while (true) {
        try {
            u->displayMenu();
            cout << "Choice: ";
            int c = readInt();

            if (c == 1) {
                cout << "\nYour appointments:\n";
                bool any = false;
                for (int i = 0; i < appointments.size(); i++) {
                    if (appointments[i].getDoctorId() == did && appointments[i].isActive()) {
                        cout << "  #" << appointments[i].getId() << " | "
                             << appointments[i].getDate() << " " << appointments[i].getSlot()
                             << " | Patient: " << patientName(appointments[i].getPatientId()) << "\n";
                        any = true;
                    }
                }
                if (!any) cout << "  No appointments.\n";
            } else if (c == 2) {
                cout << "Date (e.g. 15-03-2026): ";
                showSlots(did, readWord());
            } else if (c == 3) {
                cout << "Logged out.\n";
                return;
            } else {
                throw runtime_error("Choose a number from 1 to 3.");
            }
        } catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}
void runAdmin(User* u) {
    while (true) {
        try {
            u->displayMenu();
            cout << "Choice: ";
            int c = readInt();

            if (c == 1) {
                cout << "Doctor name (one word): ";
                string name = readWord();
                cout << "Specialization (one word): ";
                string spec = readWord();
                cout << "Login username: ";
                string user = readWord();
                cout << "Login password: ";
                string pass = readWord();
                if (usernameTaken(user)) throw runtime_error("Username already taken.");
                int id = nextDoctorId++;
                doctors.push_back(Doctor(id, name, user, pass, spec));
                saveAll();
                cout << "Doctor added with ID " << id << ".\n";
            } else if (c == 2) {
                cout << "\nAll doctors:\n";
                if (doctors.empty()) cout << "  No doctors.\n";
                for (int i = 0; i < doctors.size(); i++) printDoctor(doctors[i]);
            } else if (c == 3) {
                cout << "Doctor ID to delete: ";
                int id = readInt();
                deleteDoctor(id);
                cout << "Doctor deleted (their appointments were cancelled).\n";
            } else if (c == 4) {
                cout << "Logged out.\n";
                return;
            } else {
                throw runtime_error("Choose a number from 1 to 4.");
            }
        } catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }
}


// main()



int main() {
    loadAll();
    cout << "====================================\n";
    cout << "     DOCTOR APPOINTMENT PORTAL\n";
    cout << "====================================\n";

    while (true) {
        try {
            cout << "\n--- MAIN MENU ---\n";
            cout << "1. Patient login\n";
            cout << "2. Doctor login\n";
            cout << "3. Admin login\n";
            cout << "4. Register as new patient\n";
            cout << "5. Exit\n";
            cout << "Choice: ";
            int c = readInt();

            if (c == 5) break;

            if (c == 4) {
                cout << "Full name (one word): ";
                string name = readWord();
                cout << "Age: ";
                int age = readInt();
                if (age < 1 || age > 120) throw runtime_error("Age must be between 1 and 120.");
                cout << "Choose username: ";
                string user = readWord();
                cout << "Choose password (min 4 characters): ";
                string pass = readWord();
                if (usernameTaken(user)) throw runtime_error("Username already taken.");
                if (pass.size() < 4) throw runtime_error("Password is too short.");
                patients.push_back(Patient(nextPatientId++, name, user, pass, age));
                saveAll();
                cout << "Registration successful! You can now log in.\n";
            } else if (c >= 1 && c <= 3) {
                cout << "Username: ";
                string user = readWord();
                cout << "Password: ";
                string pass = readWord();
                User* u = login(c, user, pass);         
                cout << "\nWelcome, " << u->getName() << "!\n";
                if (c == 1) runPatient(u);
                else if (c == 2) runDoctor(u);
                else runAdmin(u);
            } else {
                throw runtime_error("Choose a number from 1 to 5.");
            }
        } catch (exception& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    cout << "Goodbye!\n";
    return 0;
}
