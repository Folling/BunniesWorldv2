#pragma once

#include <string>
#include <random>
#include <iostream>
#include <fstream>
#include <chrono>
#include <cassert>
#include <map>

#define male 0
#define female 1
#define emptyColony 0x01

const int startBunnies = 5;
const int sexualMatAge = 2;

const int amGenders = 2;
const int amColours = 5;
const int amMnames = 10;
const int amFNames = 10;

const int maxRadAge = 10;
const int maxAge = 5;

const std::string genders[amGenders] = { "male", "female" };
const std::string colours[amColours] = { "White", "Brown", "Red", "Black", "Mixed" };
const std::string maleNames[amMnames] =
{
	"Jason", "James", "Norbert", "Jonny", "Brad",
	"Lucas", "Gordon", "Bill", "Thomas", "Charles"
};
const std::string femaleNames[amFNames] =
{
	"Kristina", "Lucy", "Erika", "Yvonne", "Sophia",
	"Mary", "Leticia", "Luisa", "Caroline", "Beatrice"
};

struct Bunny {
	short name;
	short age;
	short gender;
	short colour;		
	Bunny* prev;
	Bunny* next;
	bool rad;
	void printInfo() {
		std::cout << "Name: "   << (gender==0? maleNames[name] : femaleNames[name]) << '\n';
		std::cout << "Gender: " << genders[gender] << '\n';
		std::cout << "Age: "    << age << '\n';
		std::cout << "Colour: " << colours[colour] << '\n';
		std::cout << "Is " << (!rad ? "not " : "") << "a mutant\n";
		std::cout << std::endl;
	};	
};

struct Stats {
	int total;
	int radCount;
	std::map<std::string, int> genderCount = {
		{ "male", 0}, {"female", 0}
	};
	std::map<std::string, int> colourCount = {
		{"White", 0 }, {"Brown", 0 }, {"Red", 0 }, {"Black", 0 }, {"Mixed", 0 }
	};
	std::map<std::string, int> maleNameCount = {
		{"Jason"    , 0 }, {"James"  , 0 },
		{"Norbert"  , 0 }, {"Jonny"  , 0 },
		{"Brad"     , 0 }, {"Lucas"  , 0 },
		{"Gordon"   , 0 }, {"Bill"   , 0 },
		{"Thomas"   , 0 }, {"Charles", 0 }
	};
	std::map<std::string, int> femaleNameCount = {
		{ "Kristina" , 0 },{ "Lucy"    , 0 },
		{ "Erika"    , 0 },{ "Yvonne"  , 0 },
		{ "Sophia"   , 0 },{ "Mary"    , 0 },
		{ "Leticia"  , 0 },{ "Luisa"   , 0 },
		{ "Caroline" , 0 },{ "Beatrice", 0 }
	};
	void printStats() {
		std::cout << "Total bunnies: " << total << '\n';
		std::cout << "Of which " << radCount << " are mutants\n";
		std::cout << "Males : " << genderCount["male"] << '\n';
		std::cout << "Female : " << genderCount["female"] << '\n';
		std::cout << std::endl;
		for(auto& colour : colourCount) {
			if (colour.second != 0) std::cout << colour.first << " : " 
			<< colour.second << '\n';
		}
		std::cout << std::endl;
		for(auto& mName : maleNameCount) {
			if (mName.second != 0) std::cout << mName.first << " : "
			<< mName.second << '\n';
		}
		std::cout << std::endl;
		for(auto& fName : femaleNameCount) {
			if (fName.second != 0) std::cout << fName.first << " : "
			<< fName.second << '\n';
		}
	}
};

struct Births {
	int total;
	int mBirths;
	int fBirths;
	int rBirths;
	int rfBirths;
	int rmBirths;
	void printBirths() {
		std::cout
			<< "Male births:  \t" << mBirths << '\n'
			<< "Of which " << rmBirths << " are mutants\n"
			<< "Female births:\t" << fBirths << '\n'
			<< "Of which " << rfBirths << " are mutants\n"
			<< "Mutant births:\t" << rBirths << '\n'
			<< "Total births: \t" << total << std::endl;
	}

};

struct Deaths {
	int total;
	int mDeaths;
	int fDeaths;
	int rDeaths;
	int rmDeaths;
	int rfDeaths;
	void printDeaths() {
		std::cout 
		    << "Male deaths:  \t" << mDeaths << '\n'
			<< "Of which " << rmDeaths << " were mutants\n"
			<< "Female deaths:\t" << fDeaths << '\n'
			<< "Of which" << rfDeaths << " were mutants\n"
			<< "Mutant deaths:\t" << rDeaths << '\n'
			<< "Total deaths: \t" << total << std::endl;			
	}

};

struct Iteration {
	int index;
	Stats  itStats;
	Births itBirths;
	Deaths itDeaths;
	double timeNeeded;
	void printIt() {
		std::cout << "Iteration #" << index + 1 << '\n';
		std::cout << "General stats: \n";
		itStats.printStats();
		std::cout << "\n";
		std::cout << "Birth stats: \n";
		itBirths.printBirths();
		std::cout << "\n";
		std::cout << "Death stats: \n";
		itDeaths.printDeaths();
		std::cout << "\n";
		std::cout << "Iteration took ";
		if (timeNeeded < 1) std::cout << "less than a millisecond";
		else {
			std::cout << unsigned long long(timeNeeded);
			if (unsigned long long(timeNeeded) == 1) std::cout << " millisecond.";
			else std::cout << " milliseconds.";
		}
		std::cout << std::endl;
	}
};

class Colony
{
public:
	Colony();
	Colony(short flag);
	~Colony();
public:
	void newBunny( short colour = -1);
	void killBunny(Bunny* sel);
	void printList();
	void iterate();
	void age();
	void killElders();
	void giveBirth();
	void saveColony(std::string file);
	void saveReadable(std::string file);
	void loadColony(std::string file);
public:
	void adaptBirthStats(Bunny* newborn);
	void adaptDeathStats(Bunny* passed);
	void adaptStats(Bunny* individual);
private:
	Bunny* first = nullptr;
	Bunny* last = nullptr;
	Bunny* end = nullptr;
	Bunny* root = nullptr;
public:
	std::vector<Iteration> iterations;
	int count;
	int itCount = 0;
};