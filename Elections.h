#pragma once
#include "District.h"

namespace election {

	
	class Date {
	private:
		int day;
		int month;
		int year;
	public:
		Date() : day(0), month(0), year(0) {}
		Date(int day, int month, int year);

		bool			add(int d, int m, int y);
		int				get_year()const { return year; }
		friend ostream& operator<<(ostream& out, const Date& date)
		{
			cout << date.day << "/" << date.month << "/" << date.year;
			return out;
		}

		//file
		void save(ofstream& out) const;
		
	};

	class PartyList {
	private:
		DynamicArray<Party> arr;
	public:
		// **** Ctors & Dtors ****
		PartyList(){}
		~PartyList() {}

		// **** Getters ****
		int			 size() const { return arr.size(); }
		int			 size(int prty, int dis)const;
		Citizen*	 get_by_index(int i, int disID, int indexofRep);
		const string get_name_of_party(int id);
		int			 getPartyID(int i) {  return arr[i].getID(); }
		int			 get_chosen_reps(int id);
		bool		 isBoss(int id, int prtID);

		// **** Add ****
		bool push_back(Party& p);
		bool push_back(int partyNum, int districtID, Citizen* rep);
		bool push_back(int i);

		// **** Prints & Operators ****
		void			print_boss(int prtyNum);
		bool			calElectors(int prtyNum, int n);
		friend ostream& operator<<(ostream& out, const PartyList& other)
		{
			if (other.arr.size() == 0)
			{
				out << "No Parties." << endl;
			}
			else
			{
				out << "List of Parties:" << endl;
				other.arr.print();
			}
			
			return out;
		}
		void			operator=(const PartyList& other);

		// **** File ****
		void save(ofstream& out) const;
	};

	class DistrictList{
	private:
		DynamicArray<District*> arr;
		bool check_district(int id) const{ return check(arr, id); }

	public:
		// **** Ctors & Dtors ****
		DistrictList() {}
		~DistrictList() {
			for (auto itr : arr)
				delete itr;   //deletes the pointer-district.
		}

		// **** Getters ****
		int			 size() const { return arr.size(); }
		int			 getNumOfElectors(int i)const { return arr[i]->getNumOfElectors(); }
		const string getNameByDistrict(int i);
		double		 getVotePrecentage(int i);
		int			 getSizeElectors(int disID, int prtyNum) const{ return arr[disID]->getSizeElectors(prtyNum); }
		bool		 getType(int i) { return arr[i]->get_type(); }
		bool		 type(int i);
		int			 getwinParty(int i) { return arr[i]->getwinParty(); }
		District*	 get(int id)const;
		
		
		// **** Add ****
		bool push_back(District* d);
		bool push_back(int dis, Citizen* c);
		bool push_back(int i);
		bool push_back(int disID, int prty, Citizen* c);

		// **** Voting Process ****
		bool	vote(int disID, int id, int prtyNum);
		bool	updateResult(int i);
		int		winningPartyIndex(int i);
		int		sumVotesByParty(int prtyNum);
		int		sumElecByParty(int prtyNum);
		bool	reset(int i);

		// **** Prints & Operators ****
		bool			PrintSingleDistrict(int i);
		void			printElectorsByParty(int i);
		void			print_citizens(int i) { arr[i]->printCitizens(); }
		void			printResults(int i) { arr[i]->printResults(); }
		friend ostream& operator<<(ostream& out, const DistrictList& other)
		{
			out << "List Of Districts:" << endl;
			other.arr.print();
			return out;
		}
		void			operator=(const DistrictList& other);

		// **** File ****
		void save(ofstream& out) const;
		void load(ifstream& in);
		void load_citizens(ListOfCitizens& votersList);
	};
	
	class ElectionsBASE {
	protected:
		Date date;
		ListOfCitizens votersList;
		PartyList parties;
	public:
		// **** Ctors & Dtors ****
		ElectionsBASE() : date(), votersList(), parties() {}

		// **** Getters ****
		int		 getPartiesSize(void) const { return parties.size(); }
		int		 getNumOfCitizen(void) { return votersList.size(); }
		Citizen* getCitizen(int id);
		Date	 getDate() const{ return date; }
		int		 getPartyID(int i) { return parties.getPartyID(i); }
		int		 getChosenReps(int i) {return parties.get_chosen_reps(i); }
		bool	 checkCitizen(int id);
		bool	 isBoss(int id, int prtID);
		int		 get_year() const { return date.get_year(); }
		
		// **** Add ****
		bool add(int d, int m, int y);
		bool add(Party& p);
		bool add(Citizen& c);

		// **** Prints ****
		
		bool PrintVotersBook(void) const { cout << votersList << endl; return true; }
		bool PrintParties(void) const { cout << parties << endl; return true; }
		bool PrintDate(void) const { cout << date << endl; return true; }
		void printNameOfParty(int i) { cout << parties.get_name_of_party(i); }
		void printBoss(int prtyNum) { parties.print_boss(prtyNum); }

		// **** Voting Process ****
		virtual bool updateResults() { return true; }
		virtual bool updateElectors(int disID) { return true; }
		virtual bool calElectors(int prtyNum) { return true; }
		virtual void printResults() {}
		virtual bool reset() { return true; }

		// **** File ****
		virtual void save(ofstream& out) const {}
		void		 load_date(ifstream& in);
		virtual void load(ifstream& in) {}
		void		 load_parties(ifstream& in);


		//Regular Elections
		virtual bool		 addDistrictLOR(int i){ return true; }
		virtual bool		 addDistrict(District* d){ return true; }
		virtual bool		 PrintDistricts(void) const = 0;
		virtual int			 getDistrictSize(void) const { return 0; }
		virtual const string getNameByDistrict(int i) { return nullptr; }
		virtual bool		 addCitizenToDistrict(int dis, Citizen* c){ return true; }
		virtual bool		 addPartyLOE(int i) { return true; }
		virtual bool		 addRepToParty(int partyNum, int districtID, Citizen* rep) { return true; }
		virtual bool		 vote(int disID, int id, int prtyNum) { return true; }
		virtual int			 getDistrictID(int id)const { return 0; }
		virtual int			 getSizeOfRep(int prtyNum, int disID) { return 0; }
		virtual int			 getNumOfElectorsByDis(int i)const { return 0; }
		virtual	District* get(int id) { return nullptr; }
		// Simple
		virtual void setNumOfElectors(int n) {};
		virtual bool vote(int id, int prtyNum) { return true; }
		virtual bool addRepToParty(int partyNum, Citizen* rep) { return true; }

	};

	// Regular Elections
	class Elections : public ElectionsBASE {
	private:
		
		DistrictList districts;
	public:
		// **** Ctors & Dtors ****
		Elections() : districts() {}

		// **** Getters ****
		int			 getDistrictID(int id)const;
		int			 getDistrictSize(void) const { return districts.size(); }
		const string getNameByDistrict(int i);
		int			 getSizeofElectors(int disID, int prtyNum)const { return districts.getSizeElectors(disID, prtyNum); }
		int			 getNumOfElectorsByDis(int i)const;
		int			 getSizeOfRep(int prtyNum, int disID);
		double		 getVotePrecentage(int i);
		bool		 getType(int i) { return districts.getType(i); }
		District* get(int id) { return districts.get(id); }
		// **** Add ****
		bool addDistrict(District* d);
		bool addCitizenToDistrict(int dis, Citizen* c);/////////////////////
		bool addDistrictLOR(int i);
		bool addPartyLOE(int i);
		bool addRepToParty(int partyNum, int districtID, Citizen* rep);

		// **** Prints ****
		virtual bool PrintDistricts(void) const { cout << districts << endl; return true; }
		bool PrintSingleDistrict(int i);
		bool printElectorsByDis(int i);
		void printWinBoss(int disID);
		void printResults();

		// **** Voting Process ****
		bool vote(int disID, int id, int prtyNum);
		bool updateResults();
		bool updateElectors(int disID);
		int	 sumOfVoteByParty(int prtyNum);
		int	 sumOfElecByParty(int prtyNum);
		bool calElectors(int prtyNum);
		bool reset();

		// **** File ****
		void save(ofstream& out) const;
		void load(ifstream& in);
};
	
	//simple Elections
	class SimpleElections :public ElectionsBASE {
	private:
		DevidedDistrict dis; 
	public:
		// **** Ctors & Dtors ****
		SimpleElections();

		// **** Getters ****
		const string getName() { dis.getName(); }
		int			 getSizeofElectors(int prtyNum)const { return dis.getSizeElectors(prtyNum); } // return the number of chosen electore (after elections).
		int			 getNumOfElectors()const { return dis.getNumOfElectors(); }
		int			 getSizeOfRep(int prtyNum) { return parties.size(prtyNum, 0); } // return number of representative in a party
		double		 getVotePrecentage() { return dis.getVotePrecentage(); }
		int			 getDistrictSize(void) const { return 1; }
		void		 setNumOfElectors(int n) {
			if (n < 1)
				throw invalid_argument("Not-Positivie Number of Electors");
			else
				dis.setNumOfElectors(n); }
		District*	 get(int id) { return &dis; }
	
		// **** Add ****
		bool addCitizenToDistrict(int i, Citizen* c);
		bool addPartyLOE(int i);
		bool addRepToParty(int partyNum, Citizen* rep);

		// **** Prints ****
		bool PrintDistricts(void) const { cout << dis << endl; return true; }
		void printResults();
		void print_party_result(int prtyNum);
		
		// **** Voting Process ****
		bool vote(int id, int prtyNum);
		bool reset();
		bool updateResults();
		bool updateElectors(int disID);	
		bool calElectors(int prtyNum);

		// **** File ****
		void save(ofstream& out) const;
		void load(ifstream& in);
	};
 }