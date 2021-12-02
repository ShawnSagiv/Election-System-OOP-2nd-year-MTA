#include "Citizen.h"
#include "District.h"
#include "Elections.h"

namespace election {

	int Citizen::getDistrictID() const {
		return this->district_ID->getID();
	}

	int Citizen::getID() const {
		return id;
	}

	const string Citizen::getName() const {
		return name;
	}

	int Citizen::getYOB() const {
		return yob;
	}

	bool Citizen::setDistrictID(int DistrictID) {
		this->districtID = DistrictID;
		return true;
	}

	bool Citizen::setDistrict(District* dis) {
		this->district_ID = dis;
		return true;
	}

	bool Citizen::setID(int ID) {
		this->id = ID;
		return true;
	}

	bool Citizen::setYOB(int yob) {
		this->yob = yob;
		return true;
	}

	bool Citizen::setName(const string n) {

		name = n;
		return true;
	}

	bool Citizen::set(string name, int id, int yob, District* dis) {
		return setDistrict(dis) && setID(id) && setName(name) && setYOB(yob);
	}

	Citizen::Citizen(string name, int id, int yob, District* dis, int dis_id): id(id), yob(yob), district_ID(dis) {
		if (to_string(id).size() != 9)
		{
			throw invalid_argument("ID is not in the right length");
			return;
		}
		if (yob <= 0) {
			throw invalid_argument("Wrong Date of Birth");
			return;
		}
		districtID = dis_id;
		districtID = district_ID->getID();
		this->name = name;
		votedParty = 0;
		voted = false;
	}

	bool Citizen::vote(int votedParty) {
		this->voted = true;
		this->votedParty = votedParty;
		return true;
	}

	int Citizen::getVotedParty(void) const {
		return votedParty;
	}

	Citizen& Citizen::operator=(const Citizen& other)
	{
		this->district_ID = other.district_ID;
		//this->districtID = other.districtID;
		this->yob = other.yob;
		this->id = other.id;
		this->voted = other.voted;
		this->votedParty = other.votedParty;
		name.clear();
		this->name = other.name;
		this->districtID = other.districtID;
		return *this;

	}

	Citizen::Citizen(const Citizen& other)
	{
		this->name = other.name;

		yob = other.yob;
		district_ID = other.district_ID;
		id = other.id;
		voted = other.voted;
		votedParty = other.votedParty;
		this->districtID = other.districtID;

	}

	void Citizen::save(ofstream& out) const
	{
		int len_name = name.size();
		out.write(rcastcc(&len_name), sizeof(int));
		out.write(rcastcc(&name[0]), len_name);
		int dis_id = district_ID->getID();
		out.write(rcastcc(&dis_id), sizeof(dis_id));
		out.write(rcastcc(&id), sizeof(int));
		out.write(rcastcc(&yob), sizeof(yob));
		out.write(rcastcc(&voted), sizeof(voted));
		out.write(rcastcc(&votedParty), sizeof(votedParty));

		if (!out.good())
			throw "Failure in saving to file";
	}

	void Citizen::load(ifstream& in,DistrictList& districts)
	{
		int len_name;
		int dis_id;

		in.read(rcastc(&len_name), sizeof(len_name));
		name.resize(len_name);
		in.read(rcastc(&name[0]), len_name);
		in.read(rcastc(&dis_id), sizeof(dis_id));
		in.read(rcastc(&id), sizeof(id));
		
		in.read(rcastc(&yob), sizeof(yob));
		
		in.read(rcastc(&voted), sizeof(voted));
		in.read(rcastc(&votedParty), sizeof(votedParty));
		districtID = dis_id;
		district_ID = districts.get(dis_id);
		if(!in.good())
			throw "Reading infile - load Citizen";
		if (to_string(id).size() != 9)
			throw this;
		if (yob < 0)
			throw this;
	}
	
	void Citizen::load(ifstream& in, DevidedDistrict* dis)
	{
		int len_name;
		in.read(rcastc(&len_name), sizeof(len_name));
		name.resize(len_name);
		in.read(rcastc(&name[0]), len_name);
		
		in.read(rcastc(&districtID), sizeof(districtID));
		in.read(rcastc(&id), sizeof(id));
		in.read(rcastc(&yob), sizeof(yob));
		in.read(rcastc(&voted), sizeof(voted));
		in.read(rcastc(&votedParty), sizeof(votedParty));

		district_ID = dis;
		if (!in.good())
			throw "Reading infile - load Citizen";
		if (to_string(id).size() != 9)
			throw this;
		if (yob < 0)
			throw this;
	}
	
	// *********************** ListOfCitizens ***********************

	void ListOfCitizens::save(ofstream& out) const {

		int logSize, phySize;
		logSize = arr.size();
		phySize = arr.capacity();
		out.write(rcastcc(&logSize), sizeof(logSize));
		out.write(rcastcc(&phySize), sizeof(phySize));
		for (int i = 0; i < arr.size(); i++)
		{
			arr[i].save(out);
		}
		if (!out.good())
			throw "Failure in saving to file";
	}

	void ListOfCitizens::load(ifstream& in, DistrictList& districts)
	{
		int logSize, phySize;
		in.read(rcastc(&logSize), sizeof(logSize));  // how many citizens there are
		in.read(rcastc(&phySize), sizeof(phySize));

		DynamicArray<Citizen> temp(phySize, logSize);
		try {
			for (int i = 0; i < logSize; i++) {
				temp[i].load(in, districts);
			}
		}
		catch (const char* msg){
			cout << "Error:" << msg << endl;
			exit(-1);
		}
		catch (Citizen* c)
		{
			cout << "Invalid Data from file"<<endl;
			c->~Citizen();
			exit(-1);

		}
		arr = temp;
		if (!in.good())
			throw "Reading infile - Load List of Citizens.";
	}
	
	void ListOfCitizens::load(ifstream& in, DevidedDistrict* dis)
	{
		int logSize, phySize;
		in.read(rcastc(&logSize), sizeof(logSize));  // how many citizens there are
		in.read(rcastc(&phySize), sizeof(phySize));

		DynamicArray<Citizen> temp(phySize, logSize);
		try {
			for (int i = 0; i < logSize; i++) {
				temp[i].load(in, dis);
			}
		}
		catch (const char* msg) {
			cout << "Error:" << msg << endl;
			exit(-1);
		}
		catch (Citizen * c)
		{
			cout << "Invalid Data from file" << endl;
			c->~Citizen();
			exit(-1);

		}
		arr = temp;
		if (!in.good())
			throw "Reading infile - Load List of Citizens.";
	}
	
	void ListOfCitizens::operator=(const ListOfCitizens& other)
	{
		arr = other.arr;
	}

	bool ListOfCitizens::push_back(Citizen* new_rep) {
		arr.push_back(new_rep);
		return true;
	}

	bool ListOfCitizens::push_back(Citizen& new_rep) {
		arr.push_back(new_rep);
		return true;
	}

	Citizen* ListOfCitizens::getCitizen(int id)
	{
		return arr.get_by_id(id);
	}

	Citizen* ListOfCitizens::getCitizenByIndex(int i)
	{
		return arr.get_by_index(i);
	}

	bool ListOfCitizens::vote(int id, int prtyNum)
	{
		for (int i = 0; i < arr.size(); i++)
		{
			if (arr[i].getID() == id)
			{
				if (arr[i].checkVote())
					throw "Citizen Already voted.";
				else
					arr[i].vote(prtyNum);
			}
		}
		return true;
	}

	int ListOfCitizens::getDistrictID(int id)const {
		for (int i = 0; i < arr.size(); i++)
		{
			if (id == arr[i].getID())
				return arr[i].getDistrictID();
		}
	}

	int ListOfCitizens::getVotedParty(int i) const {
		return arr[i].getVotedParty();
	}

	// *********************** ListOfCitizensArray ***********************

	bool ListOfCitizenArray::push_back(void)
	{
		arr.push_back();
		return true;
	}

	bool ListOfCitizenArray::push_back(int i, Citizen* rep)
	{
		arr.get_by_index(i)->push_back(rep);
		if (arr.size() == 0)
			arr.set_size(1);
		return true;
	}

	Citizen* ListOfCitizenArray::getRep(int disID, int index)const {
		
		return arr.get_by_index(disID)->get_by_index(index);
		
	}

	void ListOfCitizenArray::operator=(const ListOfCitizenArray& other)
	{
		arr = other.arr;
	}

}