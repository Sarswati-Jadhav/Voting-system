#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdio>   // for remove() and rename()
using namespace std;

// Structure to hold voter details
struct Voter {
    string voterID;
    string name;
    bool hasVoted;
};

// Candidate names
string candidates[3] = {"A", "B", "C"};

// Function prototypes
void registerVoter();
void castVote();
void showResults();
bool checkVoter(string id);

//-----------------------------------------------------------
int main() {
    int choice;
    do {
        cout << "\n===== VOTING SYSTEM MENU =====";
        cout << "\n1. Register New Voter";
        cout << "\n2. Cast Your Vote";
        cout << "\n3. Show Election Results";
        cout << "\n4. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch(choice) {
            case 1: registerVoter(); break;
            case 2: castVote(); break;
            case 3: showResults(); break;
            case 4: cout << "\nThank you for using the Voting System!\n"; break;
            default: cout << "\nInvalid choice. Try again.\n";
        }
    } while(choice != 4);

    return 0;
}

//-----------------------------------------------------------
// Register Voter
void registerVoter() {
    Voter v;
    cout << "\nEnter Voter ID: ";
    cin >> v.voterID;

    // Check if already exists
    if(checkVoter(v.voterID)) {
        cout << "Voter already registered!\n";
        return;
    }

    cout << "Enter Voter Name: ";
    cin.ignore();
    getline(cin, v.name);

    v.hasVoted = false;

    ofstream fout("voters.txt", ios::app);
    if(!fout) {
        cout << "Error opening file!\n";
        return;
    }

    fout << v.voterID << "," << v.name << "," << v.hasVoted << endl;
    fout.close();

    cout << "Voter Registered Successfully!\n";
}

//-----------------------------------------------------------
// Check if voter exists
bool checkVoter(string id) {
    ifstream fin("voters.txt");
    if(!fin) return false;

    string line;
    while(getline(fin, line)) {
        if(line.find(id + ",") == 0) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

//-----------------------------------------------------------
// Cast Vote
void castVote() {
    string id;
    cout << "\nEnter your Voter ID: ";
    cin >> id;

    ifstream fin("voters.txt");
    if(!fin) {
        cout << "No voters registered yet!\n";
        return;
    }

    ofstream temp("temp.txt");

    string line;
    bool found = false, alreadyVoted = false;
    string name;

    while(getline(fin, line)) {
        string voterID = line.substr(0, line.find(','));
        string rest = line.substr(line.find(',') + 1);
        string voterName = rest.substr(0, rest.find(','));
        string voteStatus = rest.substr(rest.find(',') + 1);

        if(voterID == id) {
            found = true;
            name = voterName;

            if(voteStatus == "1") {
                cout << "You have already voted!\n";
                alreadyVoted = true;
            } else {
                cout << "\nHello, " << voterName << "! Cast your vote:\n";
                for(int i = 0; i < 3; i++) {
                    cout << i + 1 << ". " << candidates[i] << endl;
                }

                int choice;
                cout << "Enter your choice: ";
                cin >> choice;

                if(choice >= 1 && choice <= 3) {
                    ofstream fout("votes.txt", ios::app);
                    fout << candidates[choice - 1] << endl;
                    fout.close();

                    cout << "Vote recorded successfully!\n";
                    voteStatus = "1";  // mark voted
                } else {
                    cout << "Invalid choice! Vote not recorded.\n";
                }
            }
        }

        temp << voterID << "," << voterName << "," << voteStatus << endl;
    }

    fin.close();
    temp.close();

    remove("voters.txt");
    rename("temp.txt", "voters.txt");

    if(!found)
        cout << "Voter ID not found! Please register first.\n";
    else if(!alreadyVoted)
        cout << "Thank you for voting, " << name << "!\n";
}

//-----------------------------------------------------------
// Show Results
void showResults() {
    int count[3] = {0, 0, 0};

    ifstream fin("votes.txt");
    if(!fin) {
        cout << "No votes cast yet!\n";
        return;
    }

    string vote;
    while(getline(fin, vote)) {
        for(int i = 0; i < 3; i++) {
            if(vote == candidates[i]) {
                count[i]++;
            }
        }
    }
    fin.close();

    cout << "\n===== ELECTION RESULTS =====\n";
    for(int i = 0; i < 3; i++) {
        cout << setw(10) << candidates[i] << " : " << count[i] << " votes\n";
    }

    // Find winner
    int maxVotes = count[0];
    int winnerIndex = 0;

    for(int i = 1; i < 3; i++) {
        if(count[i] > maxVotes) {
            maxVotes = count[i];
            winnerIndex = i;
        }
    }

    cout << "\nWinner: " << candidates[winnerIndex] << " 🎉\n";
}