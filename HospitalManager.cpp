#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

// Base class: personType
class personType {
protected:
    string name;
    int age;
public:
    personType(string n = "", int a = 0) : name(n), age(a) {}
    virtual void DisplayData() const = 0; // Pure virtual function
    virtual ~personType() {}
};

// Derived class: HospitalPerson
class HospitalPerson : public personType {
protected:
    string ID;
    string contactNumber;
    string role; // Doctor or Patient
public:
    HospitalPerson(string n, int a, string id, string contact, string r)
        : personType(n, a), ID(id), contactNumber(contact), role(r) {}

    virtual void DisplayData() const override {
        cout << "---------------------------------\n";
        cout << "Hospital Person Information:\n";
        cout << "Name         : " << name << "\n";
        cout << "Age          : " << age << "\n";
        cout << "ID           : " << ID << "\n";
        cout << "Contact      : " << contactNumber << "\n";
        cout << "Role         : " << role << "\n";
    }

    virtual ~HospitalPerson() {}
};

// Derived class: Doctor
class Doctor : public HospitalPerson {
private:
    string specialty;
public:
    Doctor(string n, int a, string id, string contact, string spec)
        : HospitalPerson(n, a, id, contact, "Doctor"), specialty(spec) {}

    void DisplayData() const override {
        HospitalPerson::DisplayData(); // استخدم دالة عرض البيانات من الفئة الأساسية
        cout << "Specialty    : " << specialty << "\n";
        cout << "---------------------------------\n";
    }

    string getID() const { return ID; }
    string getName() const { return name; } // دالة لإرجاع الاسم
};

// Derived class: Patient
class Patient : public HospitalPerson {
private:
    string patientType; // Inpatient or Outpatient
    Doctor* attendingDoctor;
    double totalBill;
public:
    Patient(string n, int a, string id, string contact, string pType, Doctor* doc)
        : HospitalPerson(n, a, id, contact, "Patient"), patientType(pType),
          attendingDoctor(doc), totalBill(0) {}

    void AddBill(double amount) {
        totalBill += amount;
    }

    void DisplayData() const override {
        HospitalPerson::DisplayData();
        cout << "Patient Type : " << patientType << "\n";
        cout << "Attending Doctor : " << (attendingDoctor ? attendingDoctor->getName() : "None") << "\n";
        cout << "Total Bill   : " << fixed << setprecision(2) << totalBill << "\n";
        cout << "---------------------------------\n";
    }

    string getID() const { return ID; }
};

// Class: HospitalManagement
class HospitalManagement {
private:
    vector<Doctor*> doctors;
    vector<Patient*> patients;

public:
    ~HospitalManagement() {
        // تنظيف الذاكرة عند إنهاء البرنامج
        for (auto doctor : doctors) delete doctor;
        for (auto patient : patients) delete patient;
    }

    // إضافة طبيب جديد
    void AddDoctor(string name, int age, string id, string contact, string specialty) {
        doctors.push_back(new Doctor(name, age, id, contact, specialty));
    }

    // إضافة مريض جديد
    void AddPatient(string name, int age, string id, string contact, string pType, string doctorID) {
        Doctor* assignedDoctor = FindDoctorByID(doctorID);
        if (assignedDoctor) {
            patients.push_back(new Patient(name, age, id, contact, pType, assignedDoctor));
        } else {
            cout << "Error: Doctor with ID " << doctorID << " not found.\n";
        }
    }

    // البحث عن طبيب باستخدام الـ ID
    Doctor* FindDoctorByID(string id) {
        for (auto doctor : doctors) {
            if (doctor->getID() == id) return doctor;
        }
        return nullptr;
    }

    // عرض جميع الأطباء
    void DisplayAllDoctors() const {
        cout << "### Doctors in Hospital ###\n";
        for (auto doctor : doctors) {
            doctor->DisplayData();
        }
    }

    // عرض جميع المرضى
    void DisplayAllPatients() const {
        cout << "### Patients in Hospital ###\n";
        for (auto patient : patients) {
            patient->DisplayData();
        }
    }

    // إضافة فاتورة لمريض
    void AddPatientBill(string patientID, double amount) {
        for (auto patient : patients) {
            if (patient->getID() == patientID) {
                patient->AddBill(amount);
                return;
            }
        }
        cout << "Error: Patient with ID " << patientID << " not found.\n";
    }
};

// Main function
int main() {
    HospitalManagement hospital;

    // إضافة أطباء
    hospital.AddDoctor("Dr. Ahmed", 45, "D001", "0123456789", "Cardiology");
    hospital.AddDoctor("Dr. Salma", 38, "D002", "0111122233", "Neurology");

    // إضافة مرضى
    hospital.AddPatient("Ali Hassan", 30, "P001", "0112233445", "Inpatient", "D001");
    hospital.AddPatient("Sara Omar", 25, "P002", "0105566778", "Outpatient", "D002");

    // إضافة فواتير للمرضى
    hospital.AddPatientBill("P001", 500.0);
    hospital.AddPatientBill("P002", 300.0);

    // عرض الأطباء والمرضى
    hospital.DisplayAllDoctors();
    hospital.DisplayAllPatients();

    return 0;
}
