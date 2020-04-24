#ifndef DATA_HH
#define DATA_HH

#include <vector>
#include <string>


class Data {
 public:
  Data(const std::string& filename);

  unsigned int size() const { return m_data.size(); }//all these parameter in brackets should be understood!!!
  double measurement(int i) const { return m_data[i]; }
  double binCenter(int i) const { return (m_bins[i+1]+m_bins[i]/2.); }
  double binLow(int i) const { return m_bins[i]; }
  double binHigh(int i) const { return m_bins[i+1]; }
  double error(int i) const { return m_error[i]; }
  double Chisq(double f(double, double*), double* parameter);
  int CheckCompatibility(Data* in,int N );

  void average(Data* in);

 private:
  Data() {}  // disallow empty data
  void assertSizes();
  std::vector<double> m_data;
  std::vector<double> m_bins;
  std::vector<double> m_error;
};

#endif
