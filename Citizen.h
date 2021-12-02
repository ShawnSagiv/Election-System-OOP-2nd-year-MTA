#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "DynamicArray.h"
#include <string>

using namespace std;
using namespace d_array;


#define rcastcc reinterpret_cast<const char*>
#define rcastc  reinterpret_cast<char*>

namespace election {
	class District;				// fw declaration
	class DistrictList;			// fw declaration
	class DevidedDistrict;		// fw declaration

	// template func
	template <class T>
	bool check(DynamicArray<T> arr, int id) 
	{
		for (int i = 0; i < arr.size(); i++)
		{
			if (id == arr[i]->getID())
				return true;
		}
		return false;
	}

	class Citizen
	{
	private:
		string		name;
		int			yob;
		int			districtID;
		District*	district_ID;
		int			id;
		bool		voted;
		int			votedParty;
		
	public:
		// ***** Ctors & Dtors ****
		Citizen() : id(0), votedParty(-1), district_ID(nullptr), yob(0), voted(false) {}
		Citizen(string name, int id, int yob, District* dis, int dis_id);
		Citizen(const Citizen& other);	
		~Citizen() {}
		
		// **** File ****
		void save(ofstream& out)const ;
		void load(ifstream& in, DistrictList& districts);
		void load(ifstream& in, DevidedDistrict* dis);

		// **** Getters ****
		int getYOB() const;
		int getID() const;
		int getDistrictID() const;
		int getVotedParty(void) const;
		const string getName() const;

		// **** Setters ****
		bool set(string name, int id, int yob, District* dis);
		bool setName(const string n);
		bool setID(int id);
		bool setYOB(int yob);
		bool setDistrictID(int districtID);
		bool setDistrict(District* dis);

		// **** Voting Process ****
		bool vote(int votedParty);				 // vote for citizen (by id) for party (votedParty), FALSE - if citizen already voted
		bool checkVote(void) { return voted; }   // return true if the citizen voted
		
		// **** Operators *****
		friend ostream& operator<<(ostream& out, const Citizen& other)
		{
			out << "Name: " << other.name
				<< ", ID: " << other.id
				<< ", Year Of Birth: " << other.yob
				<< ", District: " << other.districtID << endl;
			return out;
		}
		Citizen&		operator=(const Citizen& other);
		bool			operator!=(const Citizen& other) {
			return !(*this == other);
		}
		bool			operator==(const Citizen& other) const {
			return (id == other.id);
		}
		Citizen* operator->() {
			return this;
		}
	};

	class ListOfCitizens {
	private:
		DynamicArray<Citizen> arr;
		
	public:	
		// ***** Ctors & Dtors ****
		ListOfCitizens() {}
		~ListOfCitizens() {}

		// **** Getters ****
		int			size(void) { return arr.size(); }
		int			getDistrictID(int id)const;			// return the district id of a citizen by his personal id
		int			getVotedParty(int i) const;			// return the party id of a citizen by his personal id
		Citizen*	getCitizen(int id);					// return a pointer to a citizen by his personal id
		Citizen*	getCitizenByIndex(int i);			// return a pointer to a citizen by his index in the array of citizens

		// **** Add ****
		bool push_back(Citizen& new_rep);		// add a citizen to the array: reference
		bool push_back(Citizen* new_rep);		//                             pointer

		// **** Voting Process ****
		bool checkCitizen(int id)const {return check(arr, id);}      // return true of the citizen is exist (check by id)
		bool checkVote(int i) { return arr[i].checkVote(); }    // return true if the citizen voted (by index in the array)
		bool vote(int id, int prtyNum);							// vote for citizen (by id) for party (prtyNum), FALSE - if citizen already voted

		// **** Prints & Operators ****
		friend ostream& operator<<(ostream& out, const ListOfCitizens& other)
		{
			if (other.arr.size() == 0)
			{
				out << "No Citizens." << endl;
			}
			else
			{
				out << "List of Citizens:" << endl;
				other.arr.print();
			}
			return out;
		}
		void			operator=(const ListOfCitizens& other);
		bool			operator!=(const ListOfCitizens& other) {
			return !(*this == other);
		}
		bool			operator==(const ListOfCitizens& other) const {
			return (arr == other.arr);
		}
		
		// **** File ****
		void save(ofstream& out) const;
		void load(ifstream& in, DistrictList& districts);
		void load(ifstream& in, DevidedDistrict* dis);

	};

	class ListOfCitizenArray {
	private:
		DynamicArray<DynamicArray<Citizen>> arr;
	public:
		// ***** Ctors & Dtors ****
		ListOfCitizenArray() {}
		~ListOfCitizenArray() {}

		// **** Getters ****
		int		 size(void) const { return arr.size(); }
		int		 size(int i) const{ return arr.size(i); }
		Citizen* getRep(int disID, int index) const;

		// **** Add ****
		bool push_back(void);
		bool push_back(int i, Citizen* rep);
		
		// **** Prints & Operators ****
		void			print(int i) { arr[i].print(); }
		friend ostream& operator<<(ostream& out, const ListOfCitizenArray& other)
		{
			for (int i = 0; i < other.size(); i++)
			{
				out << "ID - " << i << ":" << endl;
				other.arr[i].print();
				out << endl;
			}
			return out;
		}
		void			operator=(const ListOfCitizenArray& other);	
		bool			operator!=(const ListOfCitizenArray& other) {
			return !(*this == other);
		}
		bool			operator==(const ListOfCitizenArray& other) const {
			return (arr == other.arr);
		}
	};
}