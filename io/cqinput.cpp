#include "io/cqinput.h"

#include <iostream>
#include <fstream>
#include <boost/smart_ptr.hpp>

size_t get_dim(const char* infile)
{
  FILE* fp = fopen(infile, "r");
  if(!fp) {
    std::cerr << "could not open input file " << infile << " for reading.\n";
    return 0;
  }
  int sz = 0;
  int dim = 0;
  fread(&sz, sizeof(int), 1, fp);
  fread(&dim, sizeof(int), 1, fp);
  fclose(fp);
  //printf("read sz=%d and dim=%d from %s\n", sz, dim, infile);
  return (size_t)dim;
}

CqInput::CqInput(const char* infile)
  : las_(get_dim(infile))
{
  valid_ = readData_(infile);
}

bool CqInput::readData_(const char* infile)
{
  FILE* fp = fopen(infile, "r");
  if(!fp) {
    std::cerr << "could not open input file " << infile << " for reading.\n";
    return false;
  }

  int elem_size = 0;
  int line_size = 0;
  fread(&elem_size, sizeof(int), 1, fp);
  fread(&line_size, sizeof(int), 1, fp);
  if(elem_size != sizeof(input_type)) {
    std::cerr << "input file in the wrong format: "
	      << elem_size << " should be " << sizeof(input_type)
	      << std::endl;
    return false;
  }
  size_t las_index = 0;
  boost::shared_ptr<input_type> buf(new input_type[line_size]);
  while(1) {
    const int nread = fread(buf.get(), elem_size, line_size, fp);
    if(feof(fp)) {
      break;
    }
    if(nread != line_size) {
      std::cerr << "encountered line of incorrect length: " << nread
		<< " instead of " << line_size << std::endl;
      fclose(fp);
      return false;
    }
    las_index = las_.create_and_store_vector();
    LAS::vector_type& v = las_.access_vector_ref(las_index);
    assert(v.size() == size_t(line_size));
    std::copy(buf.get(), buf.get() + line_size, v.begin());
  }
  fclose(fp);
  return true;
}
