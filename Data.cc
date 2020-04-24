//extend this class with experimental uncertainty
#include "Data.hh"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

using namespace std;

Data::Data(const std::string& filename) {
  // read data from file
  // return 1 if successful, 0 if otherwise

  // open the file
  ifstream file(filename);

  // check if file opened correctly
  if (!file.is_open()) {
    throw "Error could not open file with name: " + filename;
  }

  int size = -1;
  file >> size;

  // read in data from file: m_siz+1 bin edges
  for (int i = 0; i < size + 1; i++) {
    double edge;
    file >> edge;
    m_bins.push_back(edge);
  }
  // read in data from file: m_siz bin contents
  for (int i = 0; i < size; i++) {
    double entries;
    file >> entries;
    m_data.push_back(entries);
  }
  //extension1
  for (int i = 0; i < size; i++){
    double uncertainty;
    file >> uncertainty;
    m_error.push_back(uncertainty);
  }

  // done! close the file
  file.close();
};

void Data::assertSizes() { assert(m_data.size() + 1 == m_bins.size()); }

  //The returned integer should give the number of data points that diﬀer by more than N standard deviations
  // ‘this’ pointer is passed as a hidden argument to all nonstatic member function calls
  // X* is pointer for classes- A pointer is a variable that stores a memory address: essential for dynamic memory allocation
int Data::CheckCompatibility(Data* in,int N ){
  int n_points_deviation = 0;
  for(unsigned int i = 0; i < this -> size(); i++){// this-> points on Class Data. Used to address the size of the previous Object.
    double difference = (this -> measurement(i) - in-> measurement(i)); // in -> points on inserted/New date and gives access to the corresponding measurement
    if(difference > N * (this-> error(i) + in-> error(i) )) n_points_deviation++;// * for multiplication in this case! ++ for increment
  }
  return n_points_deviation;
};

void Data::average(Data* in){
   // bool compatible;
    bool compatible = this -> CheckCompatibility(in, 3);// this on previous data to check compatibility with inserted data
    //bool compatible;// True = 1, false = 0
    cout<< "Datasets are " << (compatible ? " compatible " : " incompatible ") << endl;
    for(unsigned int i = 0; i < this -> size(); i++){
        double dat1 = this -> measurement(i);
        double dat2 = in -> measurement(i);
        double e_1 = 1./(this -> error(i) * this -> error(i));
        double e_2 = 1./(in -> error(i) * in -> error(i));
        double dat = (dat1*e_1 + dat2 * e_2)/(e_1 + e_2);
        double sigma = std::sqrt(1./(e_1 +e_2));
        m_data[i] = dat; //Vector in data.hh -> dynamic coupling to store values for further operations
        m_error[i] = sigma;
    }
};

double Data::Chisq(double f(double x, double* parameter), double* parameter){ //this pointer is an implicit parameter to all member functions
    double sum = 0;
    for(unsigned int i = 0; i < this -> size(); i++){
    double numerator = this -> measurement(i) - f(this -> binCenter(i), parameter);
    numerator *= numerator; // *= is (in this case) equal to sq
    double denumerator = this ->error(i) * this ->error(i);
    double value = numerator/denumerator;
    sum += value;
    }
    return sum;
}
