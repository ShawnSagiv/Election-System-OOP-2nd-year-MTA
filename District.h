#pragma once
#include "Party.h"
namespace election {

	typedef struct result {
		int total=0;
		double precent=0;
		int electors=0;
	}Result;

	class District {
	protected:
		string				name;
		int					ID;
		int					NumOfElectors;
		double				votePrecentage;
		int					winParty;
		ListOfCitizenArray	electorsByParty;
		vector<Result>		results;
		bool				type;			// 0 - single, 1 - devided
		ListOfCitizens		my_citizens;
		int					totalVotes(void);

	public:
		// **** Ctors & Dtors ****
		District() : ID(0), type(false), NumOfElectors(0), votePrecentage(0){}
		District(const District& other);
		virtual ~District() {}

		// **** Getters ****
		const string getName() const;
		int			 getID() const;
		double 		 getVotePrecentage(void);
		int			 getNumOfElectors() const;
		const int	 getwinParty() const { return winParty; }
		bool		 get_type() const { return type; }
		
		// **** Setters ****
		bool setNumOfElectors(int num);
		bool setName(const string n);
		bool setVotePrecentage(double p);
		bool setID(int id);
		bool setType(bool t) { type = t; return true; }

		// **** Add ****
		bool push_back(int party, Citizen* el);
		bool push_back(Citizen* c);
		bool push_back();

		// ****Voting Process ****
		bool		vote(int id, int prtyNum);
		bool		updateResult(); 
		int			winningPartyIndex();                    // calculate the winning party by highest voting precentage 
		int			getVotesByParty(int prtyNum);           // return how many votes a party got
		virtual int getSizeElectors(int prtyNum)const;		// returns how many representatives a party has (after elections)
		virtual int sumElectors(int prtyNum);
		bool		reset();

		// **** Prints & Operators ****
		virtual void	printElectorsByParty(); // prints the list of electors of all parties in curr district
		void			printCitizens(); // prints the list of citizen	
		virtual void	print() { cout << *this; }
		virtual void	printResults(); 
		friend ostream& operator<<(ostream& out, const District& other)
		{
			out << "Single District: ";
			out << other.name << ", District's Number: " << other.ID<< endl;
			out << "Number Of Representatives: " << other.NumOfElectors << endl;
			
			return out;
		}
		friend ostream& operator<<(ostream& out, const District* other)
		{
			cout << *other;
			return out;
		}
		District&		operator=(const District& other);

		// **** File ****
		void save(ofstream& out) const;
		void load(ifstream& in);
	};

	class DevidedDistrict : public District
	{
	public:
		// **** Ctors & Dtors ****
		DevidedDistrict(): District() {}
		~DevidedDistrict() {}

		// **** Getters ****
		int getSizeElectors(int prtyNum)const;
		
		// **** Voting Process ****
		int sumElectors(int prtyNum);

		// **** Prints & Operators ****
		void			print() { cout << *this; }
		void			print_party_result(int prtyNum);
		void			printElectorsByParty(); // prints the list of electors of all parties in curr district
		friend ostream& operator<<(ostream& out, const DevidedDistrict& other)
		{
			cout << "Devided District: ";
			cout << other.name << ", District's Number: " << other.ID << endl;
			cout << "Number Of Representatives: " << other.NumOfElectors << endl;
			return out;
		}

		// **** File ****
		void load_citizens(ListOfCitizens& votersList);

	};
	
}
