#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <limits> // Required for numeric limits

using namespace std;

// --- Linux Helper Functions to replace conio.h ---
void clrscr() {
    cout << "\033[2J\033[1;1H"; // ANSI escape code to clear screen
}

void gotoxy(int x, int y) {
    printf("\033[%d;%dH", y, x); // ANSI escape code to move cursor
}

// A simple replacement for getch() that waits for Enter
void waitForKey() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer
    cin.get(); // Wait for input
}

// --- Class Definitions ---

class book {
    char bno[6];
    char bname[50];
    char aname[20];
public:
    void createbook() {
        cout << "\nNEW BOOK ENTRY...\n";
        cout << "\nENTER BOOK NO.: ";
        cin >> bno;
        cin.ignore(); // Clear newline from buffer
        cout << "\nENTER BOOK NAME: ";
        cin.getline(bname, 50); // Replaces gets()
        cout << "\nENTER AUTHOR NAME: ";
        cin.getline(aname, 20);
        cout << "\n\n\nBook Created..";
    }
    void showbook() {
        cout << "\nBook Number: " << bno;
        cout << "\nBook Name: " << bname;
        cout << "\nBook Author Name: " << aname;
    }
    void modifybook() {
        cout << "\nBook Number: " << bno;
        cout << "\nModify Book Name: ";
        cin.ignore();
        cin.getline(bname, 50);
        cout << "\nModify Author's Name: ";
        cin.getline(aname, 20);
    }
    char* retbno() { return bno; }
    void report() {
        cout << bno << setw(30) << bname << setw(30) << aname << endl;
    }
};

class student {
    char admno[6];
    char name[20];
    char stbno[6];
    int token;
public:
    void createstudent() {
        clrscr();
        cout << "\nNEW STUDENT ENTRY...\n";
        cout << "\nEnter The Admission No.: ";
        cin >> admno;
        cin.ignore();
        cout << "Enter The Student Name: ";
        cin.getline(name, 20);
        token = 0;
        stbno[0] = '\0';
        cout << "\n\nStudent Record Created...";
    }
    void showstudent() {
        cout << "\nAdmission Number : " << admno;
        cout << "\nStudent Name : " << name;
        cout << "\nNo of Book Issued : " << token;
        if (token == 1) {
            cout << "\nBook Number " << stbno;
        }
    }
    void modifystudent() {
        cout << "\nAdmission No. " << admno;
        cout << "\nModify Student Name : ";
        cin.ignore();
        cin.getline(name, 20);
    }
    char* retadmno() { return admno; }
    char* retstbno() { return stbno; }
    int rettoken() { return token; }
    void addtoken() { token = 1; }
    void resettoken() { token = 0; }
    void getstbno(char t[]) { strcpy(stbno, t); }
    void report() {
        cout << "\t" << admno << setw(20) << name << setw(10) << token << endl;
    }
};

fstream fp, fp1;
book bk;
student st;

// --- File Handling Functions ---

void writebook() {
    char ch;
    fp.open("book.dat", ios::out | ios::app | ios::binary); // Added ios::binary
    do {
        clrscr();
        bk.createbook();
        fp.write((char*)&bk, sizeof(book));
        cout << "\n\nDo you want to add more record...(y/n?) ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fp.close();
}

void writestudent() {
    char ch;
    fp.open("student.dat", ios::out | ios::app | ios::binary);
    do {
        clrscr();
        st.createstudent();
        fp.write((char*)&st, sizeof(student));
        cout << "\n\nDo you want to add more record...(y/n?) ";
        cin >> ch;
    } while (ch == 'y' || ch == 'Y');
    fp.close();
}

void displayspb(char n[]) {
    cout << "\nBOOK DETAILS\n";
    int flag = 0;
    fp.open("book.dat", ios::in | ios::binary);
    if (!fp) { cout << "File not found"; waitForKey(); return; }
    while (fp.read((char*)&bk, sizeof(book))) {
        if (strcasecmp(bk.retbno(), n) == 0) { // strcasecmp is Linux version of strcmpi
            bk.showbook();
            flag = 1;
        }
    }
    fp.close();
    if (flag == 0) cout << "\n\nBook does not exist";
    waitForKey();
}

void displaysps(char n[]) {
    cout << "\nSTUDENT DETAILS\n";
    int flag = 0;
    fp.open("student.dat", ios::in | ios::binary);
    if (!fp) { cout << "File not found"; waitForKey(); return; }
    while (fp.read((char*)&st, sizeof(student))) {
        if (strcasecmp(st.retadmno(), n) == 0) {
            st.showstudent();
            flag = 1;
        }
    }
    fp.close();
    if (flag == 0) cout << "\n\nStudent does not exist";
    waitForKey();
}

void modifybook() {
    char n[6];
    int found = 0;
    clrscr();
    cout << "\n\nMODIFY BOOK RECORD...";
    cout << "\n\nEnter the book no.: ";
    cin >> n;
    fp.open("book.dat", ios::in | ios::out | ios::binary);
    if (!fp) { cout << "File not found"; waitForKey(); return; }
    while (fp.read((char*)&bk, sizeof(book)) && found == 0) {
        if (strcasecmp(bk.retbno(), n) == 0) {
            bk.showbook();
            cout << "\nEnter the new details book";
            bk.modifybook();
            int pos = -1 * sizeof(bk);
            fp.seekp(pos, ios::cur);
            fp.write((char*)&bk, sizeof(book));
            cout << "\n\nRecord Updated";
            found = 1;
        }
    }
    fp.close();
    if (found == 0) cout << "\n\nRecord Not Found";
    waitForKey();
}

void modifystudent() {
    char n[6];
    int found = 0;
    clrscr();
    cout << "\n\nMODIFY STUDENT RECORD...";
    cout << "\n\nEnter the Admission no.: ";
    cin >> n;
    fp.open("student.dat", ios::in | ios::out | ios::binary);
    if (!fp) { cout << "File not found"; waitForKey(); return; }
    while (fp.read((char*)&st, sizeof(student)) && found == 0) {
        if (strcasecmp(st.retadmno(), n) == 0) {
            st.showstudent();
            cout << "\nEnter the new details of student";
            st.modifystudent();
            int pos = -1 * sizeof(st);
            fp.seekp(pos, ios::cur);
            fp.write((char*)&st, sizeof(student));
            cout << "\n\nRecord Updated";
            found = 1;
        }
    }
    fp.close();
    if (found == 0) cout << "\n\nRecord Not Found";
    waitForKey();
}

// Helper to copy file (since rename/remove can be tricky with open streams)
void copyDataExcept(const char* filename, const char* id, bool isBook) {
    ifstream infile(filename, ios::binary);
    ofstream outfile("temp.dat", ios::binary);
    bool flag = false;

    if (isBook) {
        while (infile.read((char*)&bk, sizeof(book))) {
            if (strcasecmp(bk.retbno(), id) != 0) outfile.write((char*)&bk, sizeof(book));
            else flag = true;
        }
    } else {
        while (infile.read((char*)&st, sizeof(student))) {
            if (strcasecmp(st.retadmno(), id) != 0) outfile.write((char*)&st, sizeof(student));
            else flag = true;
        }
    }
    infile.close();
    outfile.close();
    remove(filename);
    rename("temp.dat", filename);
    if (flag) cout << "\n\n\tRecord Deleted..";
    else cout << "\n\nRecord not Found";
}

void deletestudent() {
    char n[6];
    clrscr();
    cout << "\n\n\n\tDELETE STUDENT...";
    cout << "\n\nEnter the Admission no>: ";
    cin >> n;
    copyDataExcept("student.dat", n, false);
    waitForKey();
}

void deletebook() {
    char n[6];
    clrscr();
    cout << "\n\n\n\tDELETE BOOK...";
    cout << "\n\nEnter the Book no>: ";
    cin >> n;
    copyDataExcept("book.dat", n, true);
    waitForKey();
}

void displayalls() {
    clrscr();
    fp.open("student.dat", ios::in | ios::binary);
    if (!fp) { cout << "File Could Not Be Open"; waitForKey(); return; }
    cout << "\n\n\t\tStudent List\n\n";
    cout << "==================================================================\n";
    cout << "\tAdm No." << setw(10) << "Name" << setw(20) << "Book Issued\n";
    cout << "==================================================================\n";
    while (fp.read((char*)&st, sizeof(student))) {
        st.report();
    }
    fp.close();
    waitForKey();
}

void displayallb() {
    clrscr();
    fp.open("book.dat", ios::in | ios::binary);
    if (!fp) { cout << "File Could Not Be Open"; waitForKey(); return; }
    cout << "\n\n\t\tBook List\n\n";
    cout << "==================================================================\n";
    cout << "\tBook No." << setw(20) << "Book Name" << setw(25) << "Author\n";
    cout << "==================================================================\n";
    while (fp.read((char*)&bk, sizeof(book))) {
        bk.report();
    }
    fp.close();
    waitForKey();
}

void bookissue() {
    char sn[6], bn[6];
    int found = 0, flag = 0;
    clrscr();
    cout << "\n\nBOOK ISSUE...";
    cout << "\n\n\tEnter Admission no.: ";
    cin >> sn;
    fp.open("student.dat", ios::in | ios::out | ios::binary);
    fp1.open("book.dat", ios::in | ios::out | ios::binary);
    
    if(!fp || !fp1) { cout << "Files not found."; waitForKey(); return; }

    while (fp.read((char*)&st, sizeof(student)) && found == 0) {
        if (strcasecmp(st.retadmno(), sn) == 0) {
            found = 1;
            if (st.rettoken() == 0) {
                cout << "\n\n\tEnter The Book No.: ";
                cin >> bn;
                while (fp1.read((char*)&bk, sizeof(book)) && flag == 0) {
                    if (strcasecmp(bk.retbno(), bn) == 0) {
                        flag = 1;
                        st.addtoken();
                        st.getstbno(bk.retbno());
                        int pos = -1 * sizeof(st);
                        fp.seekp(pos, ios::cur);
                        fp.write((char*)&st, sizeof(student));
                        cout << "\n\n\tBook Issued Successfully\n\n Please Return Within 15 Days";
                    }
                }
                if (flag == 0) cout << "Book No. Does Not Exists";
            } else {
                cout << "You Have Not Returned The Last Book";
            }
        }
    }
    if (found == 0) cout << "Student Record Not Exists...";
    waitForKey();
    fp.close();
    fp1.close();
}

void bookdeposit() {
    char sn[6];
    int found = 0, flag = 0, day, fine;
    clrscr();
    cout << "\n\nBOOK DEPOSIT...";
    cout << "\n\n\tEnter Admission no. Of Student: ";
    cin >> sn;
    fp.open("student.dat", ios::in | ios::out | ios::binary);
    fp1.open("book.dat", ios::in | ios::out | ios::binary);
    
    if(!fp || !fp1) { cout << "Files not found."; waitForKey(); return; }

    while (fp.read((char*)&st, sizeof(student)) && found == 0) {
        if (strcasecmp(st.retadmno(), sn) == 0) {
            found = 1;
            if (st.rettoken() == 1) {
                while (fp1.read((char*)&bk, sizeof(book)) && flag == 0) {
                    if (strcasecmp(bk.retbno(), st.retstbno()) == 0) {
                        flag = 1;
                        bk.showbook();
                        cout << "\n\n Book Deposited In No. Of Days: ";
                        cin >> day;
                        if (day > 15) {
                            fine = (day - 15) * 1;
                            cout << "\n\n Fine = " << fine;
                        }
                        st.resettoken();
                        int pos = -1 * sizeof(st);
                        fp.seekp(pos, ios::cur);
                        fp.write((char*)&st, sizeof(student));
                        cout << "\n\n\tBook Deposited Successfully";
                    }
                }
                if (flag == 0) cout << "Book No. Does Not Exists";
            } else {
                cout << "No Book Issued";
            }
        }
    }
    if (found == 0) cout << "Student Record Not Exists...";
    waitForKey();
    fp.close();
    fp1.close();
}

void start() {
    clrscr();
    // Simplified startup screen for web terminal
    cout << "\n\n\tLIBRARY MANAGEMENT SYSTEM\n";
    cout << "\n\t     by: Chahat\n\n";
    waitForKey();
}

void adminmenu() {
    int ch2;
    clrscr(); // Added clear screen here to fix menu clutter
    cout << "\n\n\n\tADMINISTRATOR MENU";
    cout << "\n\t1.CREATE STUDENT RECORD";
    cout << "\n\t2.DISPLAY ALL STUDENT RECORD";
    cout << "\n\t3.DISPLAY SPECIFIC STUDENT RECORD";
    cout << "\n\t4.MODIFY STUDENT RECORD";
    cout << "\n\t5.DELETE STUDENT RECORD";
    cout << "\n\t6.CREATE BOOK";
    cout << "\n\t7.CREATE ALL BOOKS";
    cout << "\n\t8.DISPLAY SPECIFIC BOOK";
    cout << "\n\t9.MODIFY BOOK RECORD";
    cout << "\n\t10.DELETE BOOK RECORD";
    cout << "\n\t11.BACK TO MAIN MENU";
    cout << "\n\tPLEASE ENTER YOUR CHOICE(1-11): ";
    cin >> ch2;
    switch (ch2) {
        case 1: writestudent(); break;
        case 2: displayalls(); break;
        case 3: { char num[6]; clrscr(); cout << "\n\n\t Please enter admission no.: "; cin >> num; displaysps(num); break; }
        case 4: modifystudent(); break;
        case 5: deletestudent(); break;
        case 6: writebook(); break;
        case 7: displayallb(); break;
        case 8: { char num[6]; clrscr(); cout << "\n\n\tPlease enter book no.: "; cin >> num; displayspb(num); break; }
        case 9: modifybook(); break;
        case 10: deletebook(); break;
        case 11: return;
        default: cout << "Invalid choice";
    }
    adminmenu();
}

int main() {
    char ch;
    start();
    do {
        clrscr();
        cout << "\n\n\n\t MAIN MENU";
        cout << "\n\n\t1 BOOK ISSUE";
        cout << "\n\n\t2 BOOK DEPOSIT";
        cout << "\n\n\t3 ADMINISTRATOR MENU";
        cout << "\n\n\t4 EXIT";
        cout << "\n\n\t PLEASE SELECT YOUR OPTION(1-4): ";
        cin >> ch;
        switch (ch) {
            case '1': bookissue(); break;
            case '2': bookdeposit(); break;
            case '3': adminmenu(); break;
            case '4': exit(0); break;
            default: cout << "INVALID CHOICE";
        }
    } while (ch != '4');
    return 0;
}