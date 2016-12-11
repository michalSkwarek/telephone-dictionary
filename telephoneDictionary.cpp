#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

class Contact;
class TelephoneDictionary;
class Telephone;

class Contact {
    string name;
    long long contactNumber;
    friend Contact* createContact(string, long long);
    friend void addContactToTelephoneDictionary(string, long long, TelephoneDictionary *);
    friend void printContact(Contact *);
    friend long long returnTelephoneNumber(Contact *);
};

Contact* createContact(string name, long long telephoneNumber) {
    Contact *contact = new Contact;
    contact->name = name;
    contact->contactNumber = telephoneNumber;
    return contact;
}

void printContact(Contact *contact) {
    cout << contact->name << "\t" << "Numer kontaktu: " << contact->contactNumber << endl;
}

long long returnTelephoneNumber(Contact *contact) {
    return contact->contactNumber;
}

class TelephoneDictionary {
    vector<Contact*> listOfContacts;
    friend TelephoneDictionary* createTelephoneDictionary();
    friend void addContactToTelephoneDictionary(string, long long, TelephoneDictionary *);
    friend void printTelephoneDictionary(TelephoneDictionary *);
    friend int returnNumberOfContacts(TelephoneDictionary *);
    friend long long returnContactNumber(TelephoneDictionary *, int i);
    friend void printContacts(int, int, int, Telephone **, TelephoneDictionary **);
};

TelephoneDictionary* createTelephoneDictionary() {
    TelephoneDictionary *dictionary = new TelephoneDictionary;
    return dictionary;
}

void printTelephoneDictionary(TelephoneDictionary *dictionary) {
    int no = 1;
    vector<Contact *>::iterator i = dictionary->listOfContacts.begin();

    if (i == dictionary->listOfContacts.end()) {
        cout << "Ksiazka telefoniczna jest pusta\n";
    } else {
        for (i; i != dictionary->listOfContacts.end(); ++i) {
            cout << no++ << ". ";
            printContact(*i);
        }
    }
}

void addContactToTelephoneDictionary(string name, long long telephoneNumber, TelephoneDictionary *dictionary) {
    dictionary->listOfContacts.push_back(createContact(name, telephoneNumber));
}

int returnNumberOfContacts(TelephoneDictionary *dictionary) {
    return (int) (dictionary->listOfContacts.size() + 1);
}

long long returnContactNumber(TelephoneDictionary *dictionary, int lp) {
    int j = 0;
    vector<Contact*>::iterator i = dictionary->listOfContacts.begin();
    while(j < lp) {
        ++i;
        ++j;
    }
    return returnTelephoneNumber(*i);
}

class Telephone {
    string model;
    long long number;
    friend Telephone* createTelephone(string, long long);
    friend void printTelephone(Telephone *);
    friend long long returnNumber(Telephone *);
    friend void printContacts(int, int, int, Telephone **, TelephoneDictionary **);
};

Telephone* createTelephone(string model, long long number) {
    Telephone *telephone = new Telephone;
    telephone->model = model;
    telephone->number = number;
    return telephone;
}

void printTelephone(Telephone *telephone) {
    cout << telephone->model << "  -  Numer: " << telephone->number << endl;
}

long long returnNumber(Telephone *telephone) {
    return telephone->number;
}

void printContacts(int yourNumber, int yourContactNumber, int numberOfPhones, Telephone **telephones, TelephoneDictionary **dictionaries) {
    bool isContactExist = false;
    int no = 0;

    for(int k = 0; k < numberOfPhones; ++k) {
        if (returnNumber(telephones[k]) == returnContactNumber(dictionaries[yourNumber - 1], yourContactNumber - 1)) {
            isContactExist = true;
            no = k;
        }
    }

    if(isContactExist) {
        cout << "\n===== Kontakt o numerze: ";
        cout << returnContactNumber(dictionaries[yourNumber - 1], yourContactNumber - 1) << endl;;
        cout << "\nMa w swojej ksiazce telefonicznej kontakty:\n\n";
        printTelephoneDictionary(dictionaries[no]);
        cout << "\n";
    } else {
        cout << "Ten numer ma pusta ksiazke telefoniczna\n" << endl;
    }
}

//==============================//
int main(int argc, char *argv[]) {
    fstream file;
    string models[] = {"Samsung", "iPhone", "Sony", "Asus", "Motorola", "Nexus", "Acer", "Alcatel", "Huawei", "OnePlus"};

    long long number;
    string model;
    long long telephoneNumber;

    Telephone* telephones[10];
    TelephoneDictionary* dictionaries[10];
    int numberOfPhones = 0;

    file.open("telephoneDictionaryFile.txt");
    if(file.good()) {
        while(!file.eof()) {
            file >> number >> model >> telephoneNumber;
            bool isDuplicate = false;

            for(int i = 0; i < numberOfPhones; ++i) {
                if (returnNumber(telephones[i]) == number) {
                    addContactToTelephoneDictionary(model, telephoneNumber, dictionaries[i]);
                    isDuplicate = true;
                }
            }

            if(!isDuplicate) {
                telephones[numberOfPhones] = createTelephone(models[numberOfPhones], number);
                dictionaries[numberOfPhones] = createTelephoneDictionary();
                addContactToTelephoneDictionary(model, telephoneNumber, dictionaries[numberOfPhones]);
                ++numberOfPhones;
            }
        }
    } else {
        cout << "Nie udalo sie otworzyc pliku." << endl;
    }

    while(1) {
        cout << "\nLista wszystkich dostepnych telefonow:\n";
        for(int i = 0; i < numberOfPhones; ++i) {
            cout << i + 1 << ". ";
            printTelephone(telephones[i]);
        }

        int yourTelephoneNumber = 0;
        while(!(yourTelephoneNumber > 0 && yourTelephoneNumber <= numberOfPhones)) {
            cout << "\nWprowadz numer porzadkowy TELEFONU z listy dostepnych telefonow (np. 1) \n*** 0 - wyjscie z programu ***: Twoj wybor: ";
            cin >> yourTelephoneNumber;

            if(yourTelephoneNumber == 0) {
                break;
            }

            if(!(yourTelephoneNumber > 0 && yourTelephoneNumber <= numberOfPhones)) {
                cout << "Wprowadziels zly numer. Sprobuj jeszcze raz.\n";
            }
        }

        if(yourTelephoneNumber == 0)
            break;

        cout << "\n===== Ksiazka telefoniczna telefonu: ";
        printTelephone(telephones[yourTelephoneNumber - 1]);
        printTelephoneDictionary(dictionaries[yourTelephoneNumber - 1]);
        cout << "\n";

        while(1) {
            int yourContactNumber = 0;
            cout << "Wprowadz numer porzadkowy KONTAKTU ktorego chcecz obejrzec ksiazke telefoniczna z listy kontaktow (np. 1) telefonu: ";
            printTelephone(telephones[yourTelephoneNumber - 1]);
            cout << "Jesli wprowadzisz 9-cyfrowy numer kontaktu to zostanie on dopisany do ksiazki telefonicznej"
                    "\n*** 0 - powrot do poprzedniego menu ***: Twoj wybor: ";
            cin >> yourContactNumber;

            if(yourContactNumber == 0) {
                break;
            } else if(yourContactNumber > 0 && yourContactNumber < returnNumberOfContacts(dictionaries[yourTelephoneNumber - 1])) {
                printContacts(yourTelephoneNumber, yourContactNumber, numberOfPhones, telephones, dictionaries);
                yourContactNumber = 0;
            } else {
                cout << "Zly numer porzadkowy.\nKontakt o nazwie i numerze: " << yourContactNumber << " zostal wprowadzony do ksiazki telefonu: ";
                printTelephone(telephones[yourTelephoneNumber - 1]);
                ostringstream ss;
                ss << yourContactNumber;
                string nameOfYourContactNumber = ss.str();
                addContactToTelephoneDictionary(nameOfYourContactNumber, yourContactNumber, dictionaries[yourTelephoneNumber - 1]);
            }
        }
    }

    return 0;
}
