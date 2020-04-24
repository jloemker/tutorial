#include <iostream>
#include <vector>
#include <functional>//for exercise 2.2
#include <string>
#include <cmath>

#include "Data.hh"

// generic function comparing two values of some type T
template <class T>// A design template or template is a file that is created with an overall layout to be used with one or more documents
bool testEqual(const std::string& name, T expected, T real) {//The "&" denotes a reference instead of a pointer to an object
  if (expected != real) {
    std::cout << "(" << name << ": <" << expected << ">"
              << ", " << real << ") ";
    return false;
  }
  return true;
}

bool testReadingSize() {
  std::cout << "testReadingSize...";
  Data datA("testA");
  return testEqual<int>("size", 1, datA.size());
}

bool testReadingMeasurement() {
  std::cout << "testReadingMeasurement...";
  Data datA("testA");
  return testEqual("measurement", 10., datA.measurement(0));
}

bool testReadingBinEdges() {
  std::cout << "testReadingBinEdges...";
  Data datA("testA");
  return testEqual("bin low", 0., datA.binLow(0)) &&
         testEqual("bin high", 1., datA.binHigh(0));
}

bool testReadingErrors() {
  std::cout << "testReadingErrors...";
  Data datA("testA");
  return testEqual("error", 2., datA.error(0));
}

bool testCopyConstructor() {
  std::cout << "testCopyConstructor...";
  Data a("testA");
  Data b(a);
  Data c = a;  // equivalent to Data c(a)
  return testEqual("measurement", 10., b.measurement(0)) &&
         testEqual("measurement", 10., c.measurement(0));
}

void runTests() {
  std::cout << "running tests...\n";
  std::vector<std::function<bool()> > tests(
      {testReadingSize, testReadingMeasurement, testReadingBinEdges,
       testReadingErrors, testCopyConstructor});
  for (auto test : tests)
    std::cout << (test() ? " ok" : " FAILED!") << std::endl;
}

double f(double x, double* parameter){ //* for pointing to the element parameter
    double result = parameter[0] + parameter[1] * x + parameter[2] * std::exp(- parameter[3] * x);
    return result;
}


int main() {
  using namespace std;

  cout << "******************************************************" << endl;
  runTests();
  cout << "******************************************************" << endl;
  // create an object which holds data of experiment A
// Data datA("exp_A");

  //set the vector's values at compile time
  std::vector<std::string> dataset = { "A" , "B" , "C" , "D" };

  /*aggregate-initialization:  when a nameless temporary object is created with the initializer
  consisting of an empty pair of parentheses or braces*/
  std::vector<Data*> dat = {}; // Points on Class Data// New vector, will be filled up in the following
  // dat.reserve(datasets.size());

  /*object with dynamic storage duration is created by a
  new-expression with the initializer consisting of an empty pair of parentheses*/
  for(unsigned int i = 0; i < dataset.size(); i++){
    Data *this_dat = new Data("exp_"+dataset[i]); //creates the Objects exp_X and puts the elements in dat {}
    dat.push_back(this_dat); //push_back adds an element at the and of dat. Copying and allocating only if container is full full.
  }

  // here is the data from experiment A
  //Arrow operator used with a pointer variable pointing to a structure or union allows to access elements in structures and unions
  //Arrow operator are both used in C++ to access the members of a class
  cout << "bin 0: from " << dat[0] -> binLow(0) << " to " << dat[0] -> binHigh(1) << endl;
  cout << "measurement of experiment A in bin 27: " << dat[0] -> measurement(27)
       << endl;

    //To compare the data sets: B and D differ by 2 sigma
    for(unsigned int i = 0; i < dataset.size(); i++){
        for(unsigned int j = 0; j < dataset.size(); j++){
                if(i == j || i < j) continue;
        cout << "comparing dataset " << dataset[i] << " and " << dataset[j] << endl; // A and B f.e.
        cout << "number of not compatible bins [Within 2 sigma]: " << dat[i] ->CheckCompatibility(dat[j], 2) << endl;//Dat[i] has access to CheckCompatibilty
        cout << "number of not compatible bins [Within 3 sigma]: " << dat[i] ->CheckCompatibility(dat[j], 3) << endl;
        }
    }
    for(unsigned int i = 0; i < dataset.size(); i++){
        cout << "dataset" << dataset[i] << "bin 27:  " << dat[i] -> measurement(27) << "+-" << dat[i] -> error(27) << endl;
    }//From the data in dataset i, the specific elements stored in dat[i], f.e. exp_A  prints the value of corresponding measurement and error in bin 27

    //To combine the datasets
    Data* combined = new Data("exp_A");//creates Object
    cout << "Combine data A and B" << endl;
    combined -> average(dat[1]);//averages the created (to combine) with the previous object
    cout << "Combine data AB and C" << endl;
    combined -> average(dat[1]); //! Average points on CheckCompatibility before calculating the average !
    cout << "Combine data ABC and D" << endl;
    combined -> average(dat[1]);

    cout << "Combined datasets in Bin 27 " << combined-> measurement(27) << "+-" << combined ->error(27) << endl;

    double parameter[4] = {0.005, -0.00001, 0.08, 0.015};
    //a) What are the values of χ2/ndf when comparing the four individual datasets to the background model?
    //   Are the data compatible with the model using the above parameters? || No ! results are greater than 6.687...should be ~ 1
    for(unsigned int i = 0; i < dataset.size(); i++){
        int ndof = 52;//dat[i] -> size() - sizeof(parameter)/sizeof(*parameter);
        double chi2 = dat[i] -> Chisq(f, parameter);
        cout << dataset[i] << "Chisq/ndof : " << chi2/ndof << endl;
    }
    //b)What’s the value of χ2/ndf when comparing the background model with the combination?
    int ndof = 52;//combined -> size() - sizeof(parameter)/sizeof(*parameter);
    double chi2 = combined ->Chisq(f, parameter);
    cout << "Combined dataset Chisq/ndof : " << chi2/ndof << endl;
    //needs to be used in data.cc ..pointing?

  return 0;
}
