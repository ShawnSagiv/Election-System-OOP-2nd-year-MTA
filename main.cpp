

#include "Elections.h"
#include <string>
#include <typeinfo>
using namespace election;

// ************************** Static **************************
static int DistrictID = 0;
static int PartyID = 0;

// ************************** Functions Declaretions **************************
void first_menu();
void main_menu();
int GetInput();

// check if possible to operate:
bool validElections(ElectionsBASE* elec);

// load& save functions from/to a file:
int load(ifstream& infile, ElectionsBASE* elec);
void save(ofstream& outfile, ElectionsBASE* elec);

// functions of main menu - operate building the election round:
void addCitizen(ElectionsBASE* elec);
void addDistrict(ElectionsBASE* elec);
void addDate(ElectionsBASE* elec);
void addParty(ElectionsBASE* elec);
void addRep(ElectionsBASE* elec);
void vote(ElectionsBASE* elec);
void updateResults(ElectionsBASE* elec);

enum main_input
{
	add_district = 1, add_citizen = 2, add_party = 3, rep=4, p_districts= 5, p_citizens = 6, p_parties = 7, do_vote = 8, update_result = 9, exit_code = 10, save_file = 11, load_file = 12
};
enum first_input {
	create = 1, load_elections = 2, exit_fisrt_code = 3
};

// ********************************** MAIN **************************************
int main()
{
	// ************************** Variables **************************

	// This will contain the election round
	ElectionsBASE* elec = nullptr;

	int		input;									// This will contain the user's choice
	string	name;									// inner variable to save name of - citizen / district / party
	int		numOfElec, cID, YOB, disID, prtID;		// inner variables to save input from user
	bool	flag = false;							// flags for while loop of the building of election round
	bool	first_flag = false;
	int		type;									// type of election round

	// The 2 files which we:
	ifstream infile;		//  write to
	ofstream outfile;		//  read from
	
	// ************************** Code **************************
	first_menu();
	cin >> input;

	while (!first_flag) {
		switch (input)
		{
		case create:
			cout << "Enter type of elections" << endl << "1. Regular" << endl << "2. Simple" << endl;
			cin >> type;
			// Create Regular Elections
			if (type == 1)
			{
				try {
					elec = new Elections();
				}
				catch (bad_alloc & ex) {
					cout << ex.what() << endl; exit(-1);
				}

				try {
					addDate(elec);
				}
				catch (exception & ex)
				{
					cout << ex.what() << endl;
					delete elec;
					first_menu();
					cin >> input;
					break;
				}
				input = GetInput();
			}
			// Create Simple Elections
			else if (type == 2)
			{
				try {
					elec = new SimpleElections();
				}
				catch (bad_alloc & ex) {
					cout << ex.what() << endl; exit(-1);
				}
				try {
					addDate(elec);

				}
				catch (exception & ex)
				{
					cout << ex.what() << endl;
					delete elec;
					first_menu();
					cin >> input;
					break;
				}
				cout << "Enter Number Of Electors:";
				cin >> numOfElec;
				try {
					elec->setNumOfElectors(numOfElec);
				}
				catch (exception& ex){
					cout << ex.what() << endl;
					delete elec;
					first_menu();
					cin >> input;
					break;
				}
				input = GetInput();
			}
			
			else {
				cout << "Error - wrong input, the program will end.";
				exit(-1);
			}
			first_flag = true;
			break;

		case load_elections:
			input = 12;
			first_flag = true;
			break;

		case exit_fisrt_code:
			exit(-1);
			break;

		default:
			cout << "Error - wrong input, the program will end.";
			exit(-1);
			break;
		}
	}
	
	while (!flag)
	{
		try {
			switch (input)
			{
			case add_district:
				addDistrict(elec);
				input = GetInput();
				break;
			case add_citizen:
				addCitizen(elec);
				input = GetInput();
				break;
			case add_party:
				addParty(elec);
				input = GetInput();
				break;
			case rep:
				addRep(elec);
				input = GetInput();
				break;
			case p_districts:
				elec->PrintDistricts();
				input = GetInput();
				break;
			case p_citizens:
				
				elec->PrintVotersBook();
				input = GetInput();
				break;
			case p_parties:
				elec->PrintParties();
				input = GetInput();
				break;
			case do_vote:
				vote(elec);
				input = GetInput();
				break;
			case update_result:
				updateResults(elec);
				input = GetInput();
				break;
			case exit_code:
				flag = true;
				break;
			case save_file:
				cout << "Enter the file's name you want to open ";
				cin >> name;
				outfile.open(name, ios::out, ios::binary);         // opening file to write in
				save(outfile, elec);
				input = GetInput();
				break;
			case load_file:
				cout << "Enter File's Name: ";
				cin >> name;
				infile.open(name, ios::binary);
				if (!infile) {
					throw invalid_argument("Error in opening infile.");
					break;
				}
				// Type of Elections
				bool e_type;
				delete elec;
				infile.read(rcastc(&e_type), sizeof(e_type));
				if (!e_type) // regular elections
					elec = new Elections();
				else
					elec = new SimpleElections();
				input = load(infile, elec);
				break;
			default:
				cout << "Wrong Input, try again." << endl;
				input = GetInput();
				break;
			}
		}
		catch (invalid_argument& ex)
		{
			cout << ex.what() << endl;
			input = GetInput();
		}
		catch (const char* msg)
		{
			cout << msg << endl;
			input = GetInput();
		}
	}
	delete elec;
}

void first_menu()
{
	cout << "Choose an action according to the Menu:" << endl;
	cout << "1. Create Election Round" << endl;
	cout << "2. Load Existing File" << endl;
	cout << "3. Exit" << endl;
}
void main_menu()
{
	cout << "Choose an action according to the Menu:" << endl;
	cout << "1. Add District" << endl;
	cout << "2. Add Citizen" << endl;
	cout << "3. Add Party" << endl;
	cout << "4. Add Candidate To Party" << endl;
	cout << "5. Present Districts List" << endl;
	cout << "6. Present Citizens List" << endl;
	cout << "7. Present Parties List" << endl;
	cout << "8. Vote" << endl;
	cout << "9. Show Elections Results" << endl;
	cout << "10. Exit" << endl;
	cout << "11. Save To File" << endl;
	cout << "12. Load Elections From File" << endl;
}
int GetInput() {
	int input;
	main_menu();
	cin >> input;
	system("cls");
	return input;
}

void save(ofstream& outfile, ElectionsBASE* elec)
{
	// first we save the  type of elections
	bool type = false; // regular =0, simple = 1;
	if (typeid(*elec) == typeid(Elections))
		outfile.write(rcastcc(&type), sizeof(type));
	else
	{
		type = true;
		outfile.write(rcastcc(&type), sizeof(type));
	}
	elec->save(outfile);
	outfile.close();

}
int load(ifstream& infile, ElectionsBASE* elec)
{
	
	DistrictID = PartyID = 0;
	//update the date of elections
	elec->load_date(infile);
	elec->load(infile);

	DistrictID = elec->getDistrictSize();
	PartyID = elec->getPartiesSize();

	infile.close();
	return GetInput();
}

void addDate(ElectionsBASE* elec)
{
	int day, month, year;

	cout << "Hello!" << endl;

	cout << "Enter Elections' Date: " << endl;
	cout << "Day: ";
	cin >> day;
	cout << "Month: ";
	cin >> month;
	cout << "Year: ";
	cin >> year;

	if (year <= 0) {
		throw invalid_argument("Invalid Year");
		return;
	}
	if ((month < 1) || (month > 12)) {
		throw invalid_argument("Invalid month");
		return;
	}
	if ((day < 1) || (day > 31))
	{
		throw invalid_argument("Invalid day");
		return;
	}
	if (((month % 2 == 1) && (month > 7)) || ((month % 2 == 0) && (month < 8)))
	{
		if (month == 2 && day > 28) {
			throw invalid_argument("Invalid day for this month.");
			return;
		}
		if (day == 31) {
			throw invalid_argument("Invalid day for this month.");
			return;
		}
	}
	elec->add(day, month, year);
}

bool validElections(ElectionsBASE* elec)
{
	for (int prty = 0; prty < elec->getPartiesSize(); prty++)
	{
		for (int dis = 0; dis < elec->getDistrictSize(); dis++)
		{
			if (elec->getNumOfElectorsByDis(dis) > elec->getSizeOfRep(prty, dis))
				return false;
		}
	}
	return true;
}

void addDistrict(ElectionsBASE* elec)
{
	if (typeid(*elec) == typeid(SimpleElections))
	{
		throw invalid_argument("ERROR: This is a Simple Elections. No Districts.");
		return;
	}
	else { // Regular Elections
		
		District* dis;

		string name;
		int numOfElec;
		bool type; // 0 - single, 1 - devided
		cout << "Add District:" << endl << endl << "Enter District Name:";
		cin.ignore();
		getline(cin, name);
		cout << "Enter Number of Electors:";
		cin >> numOfElec;
		if (numOfElec < 1)
		{
			throw invalid_argument("Not-Positive Number of Electors.");
			return;
		}
		
		cout << "Enter type of district (0-Single, 1-Devided):";
		cin >> type;
		if ((type != 0) && (type != 1))
		{
			throw invalid_argument("Not a type of district.");
			return;
		}
		if (type)
		{
			dis = new DevidedDistrict();
		}
		else
			dis = new District();

		dis->setName(name);
		dis->setID(DistrictID++);
		dis->setNumOfElectors(numOfElec);
		if (type == 1)
			dis->setType(true);
		for (int i = 0; i < (elec->getPartiesSize()); i++)
			dis->push_back();
		for (int i = 0; i < elec->getPartiesSize(); i++)
			elec->addDistrictLOR(i);

		elec->addDistrict(dis);
		system("cls");
	}
}

void addCitizen(ElectionsBASE* elec)
{
	string name;
	int cID, disID, YOB;

	if (typeid(*elec) == typeid(Elections))
	{
		if (elec->getDistrictSize() == 0)
		{
			throw invalid_argument("There are no districts yet.");
			return;
		}
	}

	cout << "Add Citizen:" << endl << endl << "Enter Citizen Name:";
	cin.ignore();
	getline(cin, name);
	cout << "Enter ID:";
	cin >> cID;

	if (to_string(cID).size() != 9)
	{
		throw invalid_argument("ID is not in the right length");
		return;
	}
	if (elec->checkCitizen(cID))
	{
		throw invalid_argument("The ID is of someone else.");
		return;
	}
	
	cout << "Enter Year Of Birth:";
	cin >> YOB;
	if (elec->get_year() - YOB < 18)
	{
		throw invalid_argument("This Citizen is too young to vote.");
		return;
	}
	
	cout << "District Number-(";
	for (int i = 0;(( i < elec->getDistrictSize())&&(typeid(*elec) == typeid(Elections))) ; i++)
		cout << i << "." << elec->getNameByDistrict(i) << "  ";
	cout << "):";

	cin >> disID;
	if (((disID > DistrictID) || (disID < 0)) && (typeid(*elec) == typeid(Elections)))
	{
		throw invalid_argument("Wrong District ID.");
		return;
	}
	if (typeid(*elec) == typeid(Elections)) // check if the district is exist
	{
		Citizen c1(name, cID, YOB, elec->get(disID), disID);
		elec->add(c1);
		elec->addCitizenToDistrict(disID, elec->getCitizen(cID));
	}
	else if (typeid(*elec) == typeid(SimpleElections))
	{
		Citizen c1(name, cID, YOB, elec->get(0), disID);
		elec->add(c1);
		elec->addCitizenToDistrict(0, elec->getCitizen(cID));
	}
	
	system("cls");
	
}

void addParty(ElectionsBASE* elec)
{
	string name;
	int cID;
	if (elec->getNumOfCitizen() == 0)
	{
		throw invalid_argument("There are no Citizens.");
		return;
	}
	else
	{
		cout << "Add Party:" << endl << endl;
		cout << "Party's Name: ";
		cin.ignore();
		getline(cin, name);
		cout << "Enter Head Candidate's ID:";
		cin >> cID;
		if (to_string(cID).size() != 9)
		{
			throw invalid_argument("ID is not in the right length");
			return;
		}
		if (!elec->checkCitizen(cID))
		{
			throw invalid_argument("There is no matched Citizen.");
			return;
		}
		Citizen* boss;
		Party p;

		p.setID(PartyID++);
		p.setName(name);	
		boss = elec->getCitizen(cID);
		p.setBoss(boss);
		
		if (typeid(*elec) == typeid(Elections)) {
			for (int i = 0; i < (elec->getDistrictSize()); i++)
				p.push_back();
			for (int i = 0; i < elec->getDistrictSize(); i++)
				elec->addPartyLOE(i);
		}
		else
			elec->addPartyLOE(0);

		elec->add(p);

		system("cls");

	}
}

void addRep(ElectionsBASE* elec)
{
	int prtID, cID, disID;
	if (elec->getNumOfCitizen() == 0)
	{
		throw invalid_argument("There are no Citizens.");
		return;
	}
	if (elec->getPartiesSize() == 0) // NO Patries
	{
		throw invalid_argument("There are no Parties.");
		return;
	}

	cout << "Enter Party ID:";
	cin >> prtID;
	if (prtID > PartyID) // worng party
	{
		throw invalid_argument("Wrong Party ID - this party doesn't exist.");
		return;
	}
	cout << "Enter Representative ID:";
	cin >> cID;
	if (!elec->checkCitizen(cID))
	{
		throw invalid_argument("There is no matched Citizen.");
		return;
	}
	if (elec->isBoss(cID, prtID))
	{
		throw invalid_argument("This citizen is the party's head candidate.");
		return;
	}
	cout << "Enter Distritc ID for Representative:";
	cin >> disID;
	if ((typeid(*elec) == typeid(Elections)) && (disID > DistrictID))
	{
		throw invalid_argument("The district doesn't exist.");
		return;
	}
	if (typeid(*elec) == typeid(Elections))
	{

		Citizen* c2;
		c2 = elec->getCitizen(cID);
		elec->addRepToParty(prtID, disID, c2);

		system("cls");
	}
	else
	{
		Citizen* c2;
		c2 = elec->getCitizen(cID);
		elec->addRepToParty(prtID, c2);

		system("cls");
	}
}

void vote(ElectionsBASE* elec)
{
	int prtID, cID;
	if (elec->getNumOfCitizen() == 0) 
	{
		throw invalid_argument("There are no Citizens.");
		return;
	}
	if (elec->getPartiesSize() == 0)
	{
		throw invalid_argument(" There are not Parties.");
		return;
	}
	
		
	cout << "Enter Party ID:";
	cin >> prtID;

	if (prtID > PartyID) // worng party
	{
		throw invalid_argument("Wrong Party ID - this party doesn't exist.");
		return;
	}
			
	cout << "Enter Voter's ID:";
	cin >> cID;
	if (!elec->checkCitizen(cID))
	{
		throw invalid_argument("There is no matched Citizen.");
		return;
	}
	
	if (typeid(*elec) == typeid(SimpleElections))
	{
		elec->vote(cID, prtID);
	}
	else {
		elec->vote(elec->getDistrictID(cID), cID, prtID);
	}
	system("cls");
			
}

void updateResults(ElectionsBASE* elec)
{
	
	if( (PartyID == 0)||(elec->getNumOfCitizen() == 0))
	{
		throw invalid_argument("Can't operate Elections - There are not any parties / citizens.");
		return;
	}
	if ((typeid(*elec) == typeid(Elections)) && (DistrictID == 0))
	{
		throw invalid_argument("Can't operate Elections - There are not any districts.");
		return;
	}
	if ((typeid(*elec) == typeid(Elections)) && (!validElections(elec)))
	{ // אין מספיק נציגים במפלגה מסויימת
		throw invalid_argument("Can't operate Elections - There are not enough Representatives in Party.");
		return;
	}
	
	cout << "The Result of " << elec->getDate() << "'s Elections are:" << endl << endl;

	
	elec->updateResults(); // מעדכן את התוצאות בכל מחוז

	if(typeid(*elec) == typeid(Elections))
		for (int i = 0; i < elec->getDistrictSize(); i++) // על המחוזות
			elec->updateElectors(i);
	else // simple elections;
		elec->updateElectors(0);

	for (int i = 0; i < elec->getPartiesSize(); i++)
		elec->calElectors(i);
	
	//elec.sort();
	elec->printResults();

	elec->reset();
}

