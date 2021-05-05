/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>

#include "chromosome.hh"

 //sets the random seed 


//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(rand())
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
  size_t size = order_.size();
  int v1 = rand() % size;
  int v2 = rand() % size;//takes two random numbers in the range 0-size
  while (v1==v2)v2 = rand() % size;//makes sure theyre not equal, so it mutates
  int v1_int = order_[v1];
  order_[v1] = order_[v2];
  order_[v2] = v1_int;//switch the permutation
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());
  size_t size = order_.size();
  unsigned v1 = rand() % size;
  unsigned v2 = rand() % size;
  while (v1==v2)v2 = rand() % size;//make sure the two parents arent the same chromosome
  Chromosome* child_1;
  if (v1<v2) child_1 = create_crossover_child(this, other, v1, v2);//make sure begin<end so is_in_range works correctly
  if (v2<v1) child_1 = create_crossover_child(this, other, v2, v1);
  v1 = rand() % size;
  v2 = rand() % size;
  while (v1 == v2)v2 = rand() % size;
  Chromosome* child_2;
  if (v1 < v2) child_2 = create_crossover_child(other, this, v1, v2);
  if (v2 < v1) child_2 = create_crossover_child(other, this, v2, v2);
  return std::pair<Chromosome*, Chromosome*>(child_1, child_2);//return the pair of children
}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b and i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
      }
      assert(j < p2->order_.size());
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{
    double curr_dist = cities_ptr_->total_path_distance(order_);//get the distance of the chromosome
    double div = 10000;
    return div/curr_dist;//divide 10000 by the distance to make the shorter distances have better fitnesses
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
// We implement this check with a sort, which is a bit inefficient, but simple
bool
Chromosome::is_valid() const
{
    Cities::permutation_t sorted_perm = order_;//copy the current perm to a new vector
    std::sort(sorted_perm.begin(), sorted_perm.end());//sort the new vector for faster checking
    int j = 0;
    for (size_t i = sorted_perm.front(); i < sorted_perm.size(); i++) {//simply checks if the iterator is equal to the sorted permutation
        if (i != sorted_perm[j])return false;
        j++;
    }
    return true;
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
    while (begin != end) {
        if (order_[begin] == value)return true;//return true if the value has been found in the range
        begin++;
  }
    return false;
}
