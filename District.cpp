#include "District.h"

namespace election {

	const double FULL = 100.0;
	
	int District::getID() const {
		return ID;
	}
	
	const string District::getName() const {
		return name;
	}
	
	bool District::setName(const string n) {
		this->name = n;
		return true;
	}
	
	int District::getNumOfElectors() const {
		return NumOfElectors;
	}
	
	bool District::setNumOfElectors(int num) {
		NumOfElectors = num;
		return true;
	}
	
	void District::printElectorsByParty() {
		for (int i = 0; i < electorsByParty.size(); i++)
		{
			cout << "Party ID - " << i << ":" << endl;
			cout << "Vote precentage: " << results[i].precent << "%" << endl;
			cout << "Number of Votes: " << results[i].total << endl << endl;
			cout << "List of Chosen Electors:" << endl;
			electorsByParty.print(i);
			cout << "_________________________" << endl;
		}
	}

	bool District::push_back(int party, Citizen* el)
	{
		electorsByParty.push_back(party, el);
		return true;
	}

	bool District::push_back(Citizen* c)
	{
		my_citizens.push_back(c);
		return true;
	}
	
	bool District::setID(int id)
	{
		ID = id;
		return true;
	}

	bool District::push_back()
	{
		electorsByParty.push_back();
		return true;
	}

	int District::totalVotes(void) {
		int voters = 0;
		for (int i = 0; i < my_citizens.size(); i++)
		{
			if (my_citizens.checkVote(i))
				voters++;
		}
		return voters;
	}

	double District::getVotePrecentage(void) 
	{
		double temp;
		double x = static_cast<double>(totalVotes());
		double y = static_cast<double>(my_citizens.size());
		try {
			temp = (x / y) * 100.0;
		}
		catch (logic_error & ex) {
			cout << ex.what() << endl;
			exit(-1);
		}
		setVotePrecentage(temp);
		return temp;
	}

	bool District::setVotePrecentage(double p)
	{
		votePrecentage = p;
		return true;
	}

	bool District::vote(int id, int prtyNum)
	{
		return my_citizens.vote(id, prtyNum);
	}

	int District::winningPartyIndex() {

		double _max = results[0].precent;
		int index = 0;
		for (int i = 1; i < electorsByParty.size(); i++)
		{
			if (results[i].precent != _max)
			{
				if (results[i].precent > _max) {
					_max = results[i].precent;
					index = i;
				}
			}
		}
		return index;
	}

	bool District::updateResult()
	{
		int _size = electorsByParty.size(); //  מספר המפלגות
		int votes = totalVotes(); // sum total votes in district
		int count = 0;
		double temp, carry;

		try {
			results.resize(_size);
		}
		catch (bad_alloc & ex) {
			cout << ex.what() << endl;
			exit(-1);
		}
		for (int i = 0; i < my_citizens.size(); i++)
		{ // סופר כמה קולות הצביעו לכל מפלגה
			if (my_citizens.checkVote(i))
			{
				results[my_citizens.getVotedParty(i)].total++;
			}
		}
		for (int i = 0; i < _size; i++)
		{ // אחוז ההצבעות שמפלגה קיבלה במחוז ביחס להצבעות כל המחוז
			try {
				results[i].precent = (static_cast<double>(results[i].total) / votes) * 100.0;
			}
			catch (logic_error & ex) {
				cout << ex.what() << endl;
				exit(-1);
			}
		}
		for (int i = 0; i < _size; i++)
		{
			temp = (results[i].precent * NumOfElectors) / FULL;
			carry = temp - static_cast<int>(temp);
			if (carry == 0)
				results[i].electors = static_cast<int>(temp);
			else if (carry > 0.5)
				results[i].electors = static_cast<int>(temp) + 1;
			else
				results[i].electors = static_cast<int>(temp);
			count += results[i].electors;
		}
		this->winParty = winningPartyIndex();

		if (NumOfElectors - count > 0) {
			results[winParty].electors += (NumOfElectors - count);
		}

		return true;
	}

	int District::getSizeElectors(int prtyNum)const
	{
		return results[prtyNum].electors;
	}
	
	void District::printCitizens()
	{
		cout << my_citizens;
		
	}

	int District::getVotesByParty(int prtyNum)
	{
		return results[prtyNum].total;
	}

	District& District::operator=(const District& other)
	{
		this->electorsByParty = other.electorsByParty;
		this->my_citizens = other.my_citizens;
		this->ID = other.ID;
		this->NumOfElectors = other.NumOfElectors;
		this->votePrecentage = other.votePrecentage;
		for (int i = 0; i < other.electorsByParty.size(); i++)
			results[i] = other.results[i];
		this->type = other.type;
		name.clear();
		this->name = other.name;
		return *this;
	}
	
	District::District(const District& other)
	{

		this->electorsByParty = other.electorsByParty;
		this->my_citizens = other.my_citizens;
		this->ID = other.ID;
		this->NumOfElectors = other.NumOfElectors;
		this->votePrecentage = other.votePrecentage;
		for (int i = 0; i < other.electorsByParty.size(); i++)
			results[i] = other.results[i];
		this->type = other.type;
		name.clear();
		this->name = other.name;
	}
	
	int District::sumElectors(int prtyNum)
	{
		if (winParty == prtyNum)
			return NumOfElectors;
		return 0;
	}

	void District::printResults()
	{
		cout << "Voting Precentage in district:" << getVotePrecentage() << "%" << endl;
		cout << endl << "Head Candidate of Winnig Party:" << endl;

		this->printElectorsByParty();

	}

	bool District::reset()
	{
		for (int i = 0; i < electorsByParty.size(); i++)
		{
			results[i].electors = 0;
			results[i].precent = 0;
			results[i].total = 0;
		}
		return true;
	}

	void District::save(ofstream& out) const
	{
		int len_name = name.size();
		out.write(rcastcc(&len_name), sizeof(len_name));
		out.write(rcastcc(&name[0]), len_name);
		out.write(rcastcc(&NumOfElectors), sizeof(NumOfElectors));
		out.write(rcastcc(&ID), sizeof(ID));

		if (!out.good())
			throw "Failure in saving to file";
	}
	
	void District::load(ifstream& in) {
		
		int len_name;
		in.read(rcastc(&len_name), sizeof(len_name));
		name.resize(len_name);
		in.read(rcastc(&name[0]), len_name);
		in.read(rcastc(&NumOfElectors), sizeof(NumOfElectors));
		in.read(rcastc(&ID), sizeof(ID));
	
		if (!in.good())
			throw "Reading infile - Load District.";
	}
	
	void DevidedDistrict::printElectorsByParty() {

		for (int i = 0; i < electorsByParty.size(); i++)
		{
			if (results[i].electors != 0)
			{
				cout << "Party ID - " << i << ":" << endl;
				cout << "Number of Chosen Electors:" << endl;
				results[i].electors;
			}
		}
	}

	int DevidedDistrict::sumElectors(int prtyNum)
	{
		return results[prtyNum].electors;
	}
	
	int DevidedDistrict::getSizeElectors(int prtyNum)const
	{
		return results[prtyNum].electors;
	}

	void DevidedDistrict::print_party_result(int prtyNum)
	{
		cout << "Sum of votes:" << results[prtyNum].total
			<< ", Voting Precentage: " << results[prtyNum].precent << "%" << endl;
		
	electorsByParty.print(prtyNum);
	}

	void DevidedDistrict::load_citizens(ListOfCitizens& votersList)
	{
		for (int i = 0; i < votersList.size(); i++)
		{
			push_back(votersList.getCitizenByIndex(i));
		}
	}
}