#include "Party.h"

namespace election {
	int Party::getID() const {
		return ID;
	}
	
	const string Party::getName() const {
		return name;
	}

	bool Party::setName(const string n) {
		this->name = n;
		return true;
	}

	bool Party::setID(int id)
	{
		ID = id;
		return true;
	}
	
	bool Party::addRepToDistrict(int district, Citizen* rep) {
		repsByDistricts.push_back(district, rep);
		return true;
	}
	
	bool Party::push_back()
	{
		repsByDistricts.push_back();
		return true;
	}
	
	Citizen* Party::getElector(int disID, int index)
	{
		return repsByDistricts.getRep(disID, index);
	}

	Party& Party::operator=(const Party& other)
	{
		this->boss = other.boss;
		this->chosenReps = other.chosenReps;
		this->ID = other.ID;
		this->repsByDistricts = other.repsByDistricts;
		name.clear();
		this->name = other.name;
		return *this;
	}
	Party::Party(const Party& other)
	{
		this->boss = other.boss;
		this->chosenReps = other.chosenReps;
		this->ID = other.ID;
		this->repsByDistricts = other.repsByDistricts;
		name.clear();
		this->name = other.name;
	}

	void Party::save(ofstream& out) const
	{
		int len_name = name.size();
		int count = 0;
		// save name
		out.write(rcastcc(&len_name), sizeof(len_name));
		out.write(rcastcc(&name[0]), len_name);
		//save boss' id
		int boss_id = boss->getID();
		out.write(rcastcc(&boss_id), sizeof(boss_id));
		//save party id
		out.write(rcastcc(&ID), sizeof(ID));

		for (int i = 0; i < repsByDistricts.size(); i++)
		{ // number of total representative in party
			count += repsByDistricts.size(i);
		}

		out.write(rcastcc(&count), sizeof(boss_id));
		//save the array of reps by districts
		for (int i = 0; i < repsByDistricts.size(); i++)
		{
			for (int j = 0; j < repsByDistricts.size(i); j++)
			{
				int rep_id = repsByDistricts.getRep(i, j)->getID();
				out.write(rcastcc(&rep_id), sizeof(rep_id));
				out.write(rcastcc(&i), sizeof(i));
			}
		}

		if (!out.good())
			throw "Failure in saving to file";


	}
}