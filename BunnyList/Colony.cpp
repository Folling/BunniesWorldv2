#include "Colony.h"

std::random_device rd;
typedef unsigned long long ULL;

Colony::Colony()
{
	ULL s = _Query_perf_counter();

	Iteration currIt;
	currIt.itBirths = Births{};
	currIt.itDeaths = Deaths{};
	currIt.itStats = Stats{};
	currIt.index = 0;
	this->iterations.push_back(currIt);
	for(int i = 0; i < startBunnies; i++) {
		newBunny(i%5);
	}
	ULL e = _Query_perf_counter();

	this->iterations.at(itCount).timeNeeded = 
		((e - s)*1000.0 / _Query_perf_frequency());	
}

Colony::Colony(short flag) {
	ULL s = _Query_perf_counter();

	Iteration currIt;
	currIt.itBirths = Births{};
	currIt.itDeaths = Deaths{};
	currIt.itStats = Stats{};
	currIt.index = 0;
	this->iterations.push_back(currIt);
	ULL e = _Query_perf_counter();

	this->iterations.at(itCount).timeNeeded =
		((e - s)*1000.0 / _Query_perf_frequency());
}

Colony::~Colony()
{
	Bunny* i = first;
	for(; i != end;) {
		i->prev = nullptr;
		Bunny* temp = i;
		i = i->next;
		temp->next = nullptr;
		delete temp;
	}
	delete i;
	delete end;
}

void Colony::newBunny( short colour ) {
	Bunny* toAdd = new Bunny();
	toAdd->age = 0;
	toAdd->colour = colour == -1 ? rd() % amColours : colour;
	toAdd->gender = rd() % amGenders;
	toAdd->name = toAdd->gender == 0 ?
		rd() % amMnames : rd() % amFNames;
	toAdd->rad = rd() % 50 == 0;
	if (first == nullptr) {
		first = toAdd;
		last = first;
		toAdd->next = end;
		toAdd->prev = root;
	}
	else {
		last->next = toAdd;
		toAdd->prev = last;
		toAdd->next = end;
		last = toAdd;		
	}
	count++;
	adaptBirthStats(toAdd);
	adaptStats(toAdd);
}

void Colony::killBunny( Bunny* sel ) {
	adaptDeathStats(sel);
	if(sel == last) {
		sel->prev->next = end;
		last = last->prev;
		delete sel;
		return;
	}
	if(sel == first) {
		first->next->prev = nullptr;
		first = first->next;
		delete sel;
		return;
	}
	
	sel->prev->next = sel->next;
	sel->next->prev = sel->prev;
	delete sel;
}

void Colony::printList() {
	Bunny* i = first;
	for (; i != end; i = i->next) {
		i->printInfo();
	}
}

void Colony::age() {
	Bunny* i = first;
	for (; i != end; i = i->next) {
		i->age++;
	}
}

void Colony::killElders() {
	Bunny* i = first;
	for(; i != end;) {
		if(i->rad && i->age > maxRadAge || !i->rad && i->age > maxAge) {
			Bunny* temp = i;
			i = i->next;
			this->count--;
			killBunny(temp);
		}
		else i = i->next;
	}	
}

void Colony::giveBirth() {
	Bunny* i = first;
	bool validMale = false;
	for(; i != end; i = i->next) {
		if (i->age >= 2 && i->gender == male) validMale = true;
	}
	i = first;
	if (validMale) {
		for (; i != end; i = i->next) {
			if (i->age >= sexualMatAge && i->gender == female) {
				newBunny(i->colour);				
			}
		}
	}
}


void Colony::iterate() {
	Iteration nextIt = this->iterations.at(itCount);
	nextIt.itBirths = Births{};
	nextIt.itDeaths = Deaths{};

 	nextIt.index = ++itCount;
	this->iterations.push_back(nextIt);
	ULL s = _Query_perf_counter();
	age();
	killElders();
	giveBirth();
	ULL e = _Query_perf_counter();
	this->iterations.at(itCount).timeNeeded = ((e - s)*1000.0) / _Query_perf_frequency();
}

void Colony::adaptStats( Bunny* subject ) {
	auto& c = this->iterations.at(itCount).itStats;
	c.colourCount[colours[subject->colour]]++;
	if (subject->rad)c.radCount++;
	if (subject->gender == male) {
		c.genderCount["male"]++;
		c.maleNameCount[maleNames[subject->name]]++;
	}
	else {
		c.genderCount["female"]++;
		c.femaleNameCount[femaleNames[subject->name]]++;
	}
	c.total = this->count;
}

void Colony::adaptBirthStats( Bunny* newborn ) {
	auto& b = this->iterations.at(itCount).itBirths;
	if(newborn->gender == male) {
		if (newborn->rad) {
			b.rmBirths++;
			b.rBirths++;
		}
		b.mBirths++;
	} else {
		if (newborn->rad) {
			b.rfBirths++;
			b.rBirths++;
		}
		b.fBirths++;
	}
	b.total++;
}

void Colony::adaptDeathStats(Bunny* passed) {
	auto& d = this->iterations.at(itCount).itDeaths;
	auto& c = this->iterations.at(itCount).itStats;
	if (passed->gender == male) {
		if (passed->rad) {
			d.rmDeaths++;
			d.rDeaths++;
		}
		d.mDeaths++;
	}
	else {
		if (passed->rad) {
			d.rfDeaths++;
			d.rDeaths++;
		}
		d.fDeaths++;
	}
	d.total++;

	c.colourCount[colours[passed->colour]]--;
	if (passed->rad)c.radCount--;
	if (passed->gender == male) {
		c.genderCount["male"]--;
		c.maleNameCount[maleNames[passed->name]]--;
	}
	else {
		c.genderCount["female"]--;
		c.femaleNameCount[femaleNames[passed->name]]--;
	}
	c.total = this->count;
}

void Colony::saveColony(std::string file) {
	std::ofstream writer(file);
	if (!writer.is_open()) {
		std::cout << "Unable to open file\n";
		return;
	}
	for (Bunny* i = first; i != end; i = i->next) {
		writer << "{\n";
		writer << i->name << '\n';
		writer << i->age << '\n';
		writer << i->gender << '\n';
		writer << i->colour << '\n';
		writer << i->rad;
		writer << "\n}\n\n";
	}
}

void Colony::saveReadable(std::string file) {
	std::ofstream writer(file);
	if (!writer.is_open()) {
		std::cout << "Unable to open file\n";
		return;
	}
	for (Bunny* i = first; i != end; i = i->next) {
		writer << '{';
		writer << (i->gender == male ? maleNames[i->name] : femaleNames[i->name]) << '\n';
		writer << i->age << '\n';
		writer << genders[i->gender] << '\n';
		writer << colours[i->colour] << '\n';
		writer << std::boolalpha << i->rad;
		writer << '}' << "\n\n";
	}
}

void Colony::loadColony( std::string file ) {
	std::ifstream reader(file);
	if(!reader.is_open()) {
		std::cout << "Unable to open file\n";
		return;
	}
	char brace;
	while(reader >> brace) {
		Bunny* temp = new Bunny();
		assert(brace == '{');
		reader >> temp->name;
		reader >> temp->age;
		reader >> temp->gender;
		reader >> temp->colour;
		reader >> temp->rad;
		reader >> brace;
		assert(brace == '}');

		if (first == nullptr) {
			first = temp;
			last = first;
			temp->next = end;
			temp->prev = root;
		}
		else {
			last->next = temp;
			temp->prev = last;
			temp->next = end;
			last = temp;
		}
		count++;
		adaptBirthStats(temp);
		adaptStats(temp);
	}
}