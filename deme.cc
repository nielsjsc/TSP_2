#include "deme.hh"




Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate) {
	mut_rate_ = mut_rate;
	for (unsigned i = 0; i < pop_size; i++) {
		pop_.push_back(new Chromosome(cities_ptr));//makes a population of random chromosomes on the heap
	}
}


Deme::~Deme() {
	for (size_t i = 0; i < pop_.size(); i++) {
		delete pop_[i];//deallocates the population that was stored on the heap
	}
}


//iterates through whole population, calculating the fitness and returning the best one
const Chromosome* Deme::get_best() const{
	Chromosome* fittest=pop_[0];
	for (size_t i = 1; i < pop_.size();i++) {
		if (pop_[i]->get_fitness() > fittest->get_fitness()) fittest = pop_[i];
	}
	return fittest;
}


Chromosome* Deme::select_parent() {
	double sum = 0;
	for (size_t i = 0; i < pop_.size(); i++) {//calculates the sum of all fitnesses for the random selection
		sum+=pop_[i]->get_fitness();
	}
	std::uniform_real_distribution<double> distribution(0.0, sum);	//sets the distribution from 0 to the sum
	double choice = distribution(generator_);//gets a random double from 0 to sum
	double find_choice = 0;
	for (size_t i = 0; i < pop_.size(); i++) {//iterates through sum, adding each fitness each time, until it reaches the point that was randomly selected
		find_choice += pop_[i]->get_fitness();
		if (find_choice > choice)return pop_[i];//if it finds the point, return the chromosome
	}

	return pop_.back();//if the point was never reached, then it was the last one
}

void Deme::compute_next_generation() {
	std::vector<Chromosome*> new_vect;
	for (size_t i = 0; i < pop_.size() / 2; i++) {//for the whole population divided by two, it makes two children every iteration
		Chromosome* p1 = select_parent();
		Chromosome* p2 = select_parent();//selects two parents
		while (p1->get_ordering() == p2->get_ordering()) {
			p2 = select_parent();//if the parents have the same ordering, change them
		}
		double choice = ((double)rand() / (double)RAND_MAX);
		if (choice < mut_rate_)p1->mutate();
		choice = ((double)rand() / (double)RAND_MAX);
		if (choice < mut_rate_)p2->mutate();//mutate if the random choice is less than the mutation rate
		std::pair<Chromosome*, Chromosome*> children = p1->recombine(p2);//turn the parents into the new children
		new_vect.push_back(children.first);
		new_vect.push_back(children.second);//add both children to the new pop_
	}
	for (size_t i = 0; i < pop_.size(); i++) {
		delete pop_[i];//deallocate the current population stored on the heap
	}
	pop_ = new_vect;//set the pop to be the new vector of children
}