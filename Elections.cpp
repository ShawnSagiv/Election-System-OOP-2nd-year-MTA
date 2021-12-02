#include "Elections.h"

namespace election {


	Date::Date(int day, int month, int year) {
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

		this->day = day;
		this->month = month;
		this->year = year;
	}

	bool Date::add(int d, int m, int y)
	{
		day = d;
		month = m;
		year = y;
		return true;
	}

	void Date::save(ofstream& out) const
	{
		out.write(rcastcc(&day), sizeof(day));
		out.write(rcastcc(&month), sizeof(month));
		out.write(rcastcc(&year), sizeof(year));
	}


	// ************************************* PartyList *************************************

	bool PartyList::push_back(Party& p)
	{
		arr.push_back(p);
		return true;
	}

	bool PartyList::push_back(int partyNum, int districtID, Citizen* rep) {
		arr[partyNum].addRepToDistrict(districtID, rep);
		return true;
	}

	bool PartyList::push_back(int i)
	{
		arr[i].push_back();
		return true;
	}

	bool PartyList::isBoss(int id, int prtID)
	{
		return arr[prtID].isBoss(id);
	}

	int PartyList::size(int prty, int dis) const {
		return arr[prty].size_of_index(dis);
	}

	void PartyList::print_boss(int prtyNum)
	{
		arr[prtyNum].print_boss();
	}

	Citizen* PartyList::get_by_index(int i, int disID, int indexofRep) {
		return arr[i].getElector(disID, indexofRep);
	}

	bool PartyList::calElectors(int prtyNum, int n)
	{
		arr[prtyNum].set_chosen_reps(n);
		return true;
	}

	const string PartyList::get_name_of_party(int id)
	{
		return arr[id].getName();
	}

	int PartyList::get_chosen_reps(int id)
	{
		for (int i = 0; i <size(); i++)
		{
			if (arr[i].getID() == id)
				return arr[i].get_chosen_reps();
		}
		return true;
	}

	void PartyList::operator=(const PartyList& other)
	{
		arr = other.arr;
	}

	void PartyList::save(ofstream& out) const
	{
		int _size = size();
		out.write(rcastcc(&_size), sizeof(_size));
		for (int i = 0; i < _size; ++i) {
			arr[i].save(out);
		}
		if (!out.good())
			throw "Failure in saving to file";
	}

	// ************************************* DistrictList *************************************

	bool DistrictList::push_back(District* d)
	{
		arr.push_back(d);
		return true;
	}

	const string DistrictList::getNameByDistrict(int i)
	{
		return arr[i]->getName();
	}

	District* DistrictList::get(int id)const
	{
		if (check_district(id))
			return arr[id];
	}

	bool DistrictList::push_back(int dis, Citizen* c)
	{
		arr[dis]->push_back(c);
		return true;
	}

	bool DistrictList::push_back(int i)
	{
		arr[i]->push_back();
		return true;
	}

	double DistrictList::getVotePrecentage(int i) {
		return arr[i]->getVotePrecentage();
	}

	bool DistrictList::vote(int disID, int id, int prtyNum) {
		return arr[disID]->vote(id, prtyNum);
	}

	bool DistrictList::push_back(int disID, int prty, Citizen* c)
	{
		arr[disID]->push_back(prty, c);
		return true;
	}

	bool DistrictList::updateResult(int i) {
		return arr[i]->updateResult();
	}

	int DistrictList::winningPartyIndex(int i) {
		return arr[i]->winningPartyIndex();
	}

	bool DistrictList::PrintSingleDistrict(int i)
	{
		this->arr[i]->print();
		return true;
	}

	void DistrictList::printElectorsByParty(int i)
	{
		arr[i]->printElectorsByParty();
	}

	int DistrictList::sumVotesByParty(int prtyNum)
	{
		int count = 0;
		for (auto it : arr)
		{
			count += it->getVotesByParty(prtyNum);
		}
		return count;
	}

	int DistrictList::sumElecByParty(int prtyNum)
	{
		int count = 0;
		for (auto it : arr)
		{
			count += it->sumElectors(prtyNum);
		}
		return count;
	}

	void DistrictList::operator=(const DistrictList& other)
	{
		arr = other.arr;
	}

	bool DistrictList::type(int i)
	{
		if (typeid(*arr[i]) == typeid(District))
			return false;
		return true;
	}

	void DistrictList::save(ofstream& out) const
	{
		bool type = false; //  0 = regular, 1 = devided
		int logSize = arr.size();
		int phySize = arr.capacity();
		out.write(rcastcc(&logSize), sizeof(logSize));
		out.write(rcastcc(&phySize), sizeof(phySize));

		for (int i = 0; i < logSize; ++i) {
			if (typeid(*arr[i]) == typeid(District))
				out.write(rcastcc(&type), sizeof(type));
			else
			{
				type = true;
				out.write(rcastcc(&type), sizeof(type));
			}
			arr[i]->save(out);
			type = false;
		}
		if (!out.good())
			throw "Failure in saving to file";
	}

	void DistrictList::load(ifstream& in)
	{
		bool type;
		int logSize;
		int phySize;

		in.read(rcastc(&logSize), sizeof(logSize));
		in.read(rcastc(&phySize), sizeof(phySize));
		
		DynamicArray<District*> temp(phySize, logSize);
		for (int i = 0; i < logSize; i++)
		{
			in.read(rcastc(&type), sizeof(type));
			try {
				if (!type) // single district
					temp[i] = new District();
				else // devided
					temp[i] = new DevidedDistrict();
			}
			catch (bad_alloc & ex) {
				cout << ex.what() << endl;
				exit(-1);
			}
			temp[i]->setType(type);
			try {
				temp[i]->load(in);
			}
			catch (const char* msg) {
				cout <<msg << endl;
				exit(-1);
			}
		}
		arr = temp;
		if (!in.good())
			throw "Reading infile - load List Of District";
	}

	void DistrictList::load_citizens(ListOfCitizens& votersList)
	{
		for (int i = 0; i < votersList.size(); i++)
		{
			Citizen* temp = votersList.getCitizenByIndex(i);
			push_back(temp->getDistrictID(), votersList.getCitizenByIndex(i));
		}
	}
	
	bool DistrictList::reset(int i)
	{
		arr[i]->reset();
		return true;
	}

	// ************************************* ElectionsBASE *************************************

	void ElectionsBASE::load_date(ifstream& in)
	{
		int day, month, year;
		in.read(rcastc(&day), sizeof(day));
		
		in.read(rcastc(&month), sizeof(month));
		
		in.read(rcastc(&year), sizeof(year));
		
		if (!in.good())
			throw invalid_argument("Reading infile - Load Date of elections");
		date.add(day, month, year);

	}

	bool ElectionsBASE::add(int d, int m, int y)
	{
		date.add(d, m, y);
		return true;
	}

	bool ElectionsBASE::add(Party& p)
	{
		parties.push_back(p);
		return true;
	}

	bool ElectionsBASE::add(Citizen& c)
	{
		votersList.push_back(c);
		return true;
	}

	Citizen* ElectionsBASE::getCitizen(int id)
	{
		return votersList.getCitizen(id);
	}

	bool ElectionsBASE::checkCitizen(int id)
	{
		return votersList.checkCitizen(id);
	}

	bool ElectionsBASE::isBoss(int id, int prtID)
	{
		return parties.isBoss(id, prtID);
	}

	void ElectionsBASE::load_parties(ifstream& in)
	{
		int size, len_name, reps, boss_id, ID, temp_id;
		string name;
		// int num of parties
		in.read(rcastc(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			in.read(rcastc(&len_name), sizeof(len_name));
			name.resize(len_name);
			in.read(rcastc(&name[0]), len_name);
			in.read(rcastc(&boss_id), sizeof(boss_id)); // boss id
			in.read(rcastc(&ID), sizeof(ID));
			in.read(rcastc(&reps), sizeof(reps));

			Party p;
			p.setName(name);
			p.setID(ID);
			p.setBoss(getCitizen(boss_id));

			for (int i = 0; i < getDistrictSize(); i++)
				p.push_back();

			this->add(p);


			for (int j = 0; j < reps; j++)
			{
				in.read(rcastc(&boss_id), sizeof(boss_id));
				in.read(rcastc(&temp_id), sizeof(temp_id));
				if (typeid(*this) == typeid(Elections))
					addRepToParty(ID, temp_id, getCitizen(boss_id));
				else
					addRepToParty(ID, getCitizen(boss_id));
			}
		}


		// update size of parties for districts
		for (int j = 0; j < getDistrictSize(); j++)
		{
			for (int i = 0; i < getPartiesSize(); i++)
				addPartyLOE(j);
		}
	}

	// ************************************* Elections *************************************

	bool Elections::addDistrict(District* d)
	{
		districts.push_back(d); return true;
	}
	
	bool Elections::addCitizenToDistrict(int dis, Citizen* c)
	{
		districts.push_back(dis, c);
		return true;
	}
	
	const string Elections::getNameByDistrict(int i)
	{
		return districts.getNameByDistrict(i);
	}
	
	bool Elections::addRepToParty(int partyNum, int districtID, Citizen* rep)
	{
		parties.push_back(partyNum, districtID, rep);
		return true;
	}
	
	bool Elections::addDistrictLOR(int i)
	{
		parties.push_back(i);
		return true;
	}

	bool Elections::addPartyLOE(int i)
	{
		districts.push_back(i);
		return true;
	}

	int Elections::getNumOfElectorsByDis(int i)const
	{
		return districts.getNumOfElectors(i);
	}

	double Elections::getVotePrecentage(int i) {
		return districts.getVotePrecentage(i);
	}


	bool Elections::vote(int disID, int id, int prtyNum) {
		//return votersList.vote(id, prtyNum);

		votersList.vote(id, prtyNum);
		return districts.vote(disID, id, prtyNum);

	}
	
	int Elections::getDistrictID(int id)const {
		return votersList.getDistrictID(id);
	}
	
	int Elections::getSizeOfRep(int prtyNum, int disID)
	{
		return parties.size(prtyNum, disID);
	}
	
	bool Elections::updateResults()
	{
		for (int i = 0; i < getDistrictSize(); i++)
		{
			districts.updateResult(i);
		}
		return true;
	}	
	
	void Elections::printWinBoss(int disID) {
		int prtyNum;

		prtyNum = districts.winningPartyIndex(disID);
		parties.print_boss(prtyNum);
	}
	
	bool Elections::PrintSingleDistrict(int i)
	{
		districts.PrintSingleDistrict(i);
		return true;
	}

	bool Elections::updateElectors(int disID)
	{
		for (int i = 0; i < getPartiesSize(); i++) // מפלגות
		{
			for (int j = 0; j < districts.getSizeElectors(disID, i); j++)
			{
				districts.push_back(disID, i, parties.get_by_index(i, disID, j));
			}
		}
		return true;
	}
	
	bool Elections::printElectorsByDis(int i) {
		districts.printElectorsByParty(i);
		return true;
	}

	int Elections::sumOfVoteByParty(int prtyNum)
	{
		return districts.sumVotesByParty(prtyNum);
	}
	
	bool Elections::calElectors(int prtyNum)
	{
		parties.calElectors(prtyNum, sumOfElecByParty(prtyNum));
		return true;
	}
	
	int Elections::sumOfElecByParty(int prtyNum) // לשכפל
	{
		return districts.sumElecByParty(prtyNum);
	}
	
	void Elections::printResults()
	{
		for (int i = 0; i < getDistrictSize(); i++)
		{
			PrintSingleDistrict(i);
			cout << "Voting Precentage in district:" << getVotePrecentage(i) << "%" << endl;

			if (!districts.type(i)) // single district
			{
				cout << endl << "Head Candidate of Winnig Party:" << endl;
				printWinBoss(i);
				printElectorsByDis(i);
			}
			else //devided district
			{
				int k, max_id = 0;
				int _max = districts.getSizeElectors(i, 0); // the number of chosen electors of the first party
				int prev_max = 1000;
				for (int j = 0; j < getPartiesSize(); j++)
				{
					for (k = 0; k < getPartiesSize(); k++)
					{
						if ((districts.getSizeElectors(i, k) > _max) && (districts.getSizeElectors(i, k) < prev_max))
						{
							max_id = k;
							_max = districts.getSizeElectors(i, k);
						}
					}
					printNameOfParty(max_id); cout << endl;
					printBoss(max_id);
					cout << "Number of Electors: " << _max << endl;
					prev_max = _max;
					_max = 0;
				}
			}
		}
		cout << "----------------" << endl << endl;

		cout << "Results by Parties:" << endl;

		int max_id = 0;
		int _max = parties.get_chosen_reps(0);// the number of chosen electors of the first party
		int prev_max = 1000;
		for (int j = 0; j < getPartiesSize(); j++)
		{
			for (int k = 0; k < getPartiesSize(); k++)
			{
				if ((parties.get_chosen_reps(k) > _max) && (parties.get_chosen_reps(k) < prev_max))
				{
					max_id = k;
					_max = parties.get_chosen_reps(k);
				}
			}
			printNameOfParty(max_id); cout;
			cout << ":" << endl;
			cout << "Head Candidate: " << endl;
			printBoss(max_id);
			cout << "Number of representatives: " << _max << endl;

			cout << "Sum of votes:" << sumOfVoteByParty(max_id) << endl
				<< "----------------" << endl;
			prev_max = _max;
			_max = 0;
		}
	}

	bool Elections::reset()
	{
		for (int i = 0; i < getDistrictSize(); i++)
		{
			districts.reset(i);
		}
		for (int i = 0; i < getPartiesSize(); i++)
		{ // this reset the chosenReps in each party to - 0;
			parties.calElectors(i, 0);
		}
		return true;
	}
	
	void Elections::save(ofstream& out) const
	{
		date.save(out);
		districts.save(out);
		votersList.save(out);
		parties.save(out);
	}

	void Elections::load(ifstream& in)
	{
		try {
			districts.load(in);
			votersList.load(in, districts);
			districts.load_citizens(votersList);
			load_parties(in);
		}
		catch (const char* msg) {
			cout << "ERROR: " << msg << endl;
			exit(-1);
		}
	}

	// ************************************* SimpleElections *************************************

	bool SimpleElections::addCitizenToDistrict(int i, Citizen* c)
	{
		dis.push_back(c);
		return true;
	}
	
	bool SimpleElections::addRepToParty(int partyNum, Citizen* rep)
	{
		parties.push_back(partyNum, 0, rep);
		return true;
	}
	
	bool SimpleElections::addPartyLOE(int i)
	{
		dis.push_back();
		return true;
	}

	bool SimpleElections::vote(int id, int prtyNum) {
		votersList.vote(id, prtyNum);
		return dis.vote(id, prtyNum);
	}
	
	bool SimpleElections::updateResults()
	{
		dis.updateResult();
		return true;
	}

	bool SimpleElections::updateElectors(int disID)
	{
		for (int i = 0; i < getPartiesSize(); i++) // מפלגות
		{
			for (int j = 0; j < dis.getSizeElectors(i); j++)
			{
				dis.push_back(i, parties.get_by_index(i, disID, j));
			}
		}
		return true;
	}
	
	void SimpleElections::print_party_result(int prtyNum)
	{
		dis.print_party_result(prtyNum);
	}

	bool SimpleElections::calElectors(int prtyNum)
	{
		int n;
		n = dis.sumElectors(prtyNum);
		parties.calElectors(prtyNum, n);
		return true;
	}

	SimpleElections::SimpleElections()
	{
		char n[8] = "default";
		dis.setName(n);
		dis.setType(1); // Devided District

	}

	void SimpleElections::printResults()
	{
		int max_id = 0;
		int _max = parties.get_chosen_reps(0);// the number of chosen electors of the first party
		int prev_max = 1000;


		cout << "Voting Precentage:" << getVotePrecentage() << "%" << endl;

		for (int j = 0; j < getPartiesSize(); j++)
		{
			for (int k = 0; k < getPartiesSize(); k++)
			{
				if ((parties.get_chosen_reps(k) > _max) && (parties.get_chosen_reps(k) < prev_max))
				{
					max_id = k;
					_max = parties.get_chosen_reps(k);
				}
			}
			printNameOfParty(max_id); cout;
			cout << ":" << endl;
			cout << "Head Candidate: " << endl;
			printBoss(max_id);
			cout << "Number of representatives: " << _max << endl;

			print_party_result(max_id);
			cout << "----------------" << endl;
			prev_max = _max;
			_max = 0;
		}
	}

	bool SimpleElections::reset()
	{
		dis.reset();
		for (int i = 0; i < getPartiesSize(); i++)
		{ // this reset the chosenReps in each party to - 0;
			parties.calElectors(i, 0);
		}
		return true;
	}

	void SimpleElections::save(ofstream& out) const
	{
		date.save(out);
		dis.save(out);
		votersList.save(out);
		parties.save(out);
	}

	void SimpleElections::load(ifstream& in)
	{
		try {
			dis.load(in);
			votersList.load(in, &dis);
			dis.load_citizens(votersList);
			load_parties(in);
		}
		catch (const char* msg) {
			cout << "Error:" << msg << endl;
			exit(-1);
		}
	}
}