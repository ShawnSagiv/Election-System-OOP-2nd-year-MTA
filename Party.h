#pragma once
#include "Citizen.h"


namespace election {

	class Party {
	private:	
		int ID;
		string name;
		Citizen* boss;
		ListOfCitizenArray repsByDistricts;
		int chosenReps;

	public:
		// **** Ctors & Dtors ****
		Party() : boss(), ID(0), repsByDistricts(), chosenReps(0) {}
		Party(const Party& other);
		~Party() {}

		// **** Getters ****
		const string getName() const;
		int getID() const;
		Citizen* getElector(int disID, int index);
		int size_of_index(int dis) const{ return repsByDistricts.size(dis); }
		int get_chosen_reps()const { return chosenReps; }  // return the number of chosen reps, after elections

		// **** Setters ****
		bool setID(int id);
		bool setName(const string n);
		bool setBoss(Citizen *boss) { this->boss = boss; return true; }

		// **** Add ****
		bool addRepToDistrict(int district, Citizen* rep); // add a representative to the party, by his district
		bool push_back();							   // add a cell to the array of repsByDistricts

		// **** General ****
		bool isBoss(int id) { return (id ==boss->getID()); }
		bool set_chosen_reps(int n) { chosenReps = n; return true; }

		// **** Prints & Operators ****
		void print_boss() { cout << *boss; }  // Print the head candidate of the party
		friend ostream& operator<<(ostream& out, const Party& other)
		{
			out << "Party's Name: " << other.name << ", Party's Number:" << other.ID << endl;
			out << "Head Candidate: " << endl;
			out << *other.boss << endl << endl;
			out << other.repsByDistricts;
			return out;
		}
		Party& operator=(const Party& other);

		// **** File ****
		void save(ofstream& out) const;
	};
}


