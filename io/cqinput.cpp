#include "io/cqinput.h"

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <boost/smart_ptr.hpp>

namespace Cq {

// ============================= CqFile ==============================
CqFile::CqFile(const char* filename)
{
  fp_ = fopen(filename, "r");
  if(!fp_) {
    throw std::runtime_error(std::string("could not open ") + filename + " for reading");
  }
}

CqFile::~CqFile()
{
  if(fp_) { fclose(fp_); fp_ = 0; }
}

FILE* CqFile::fp()
{
  // want something like:
  // FILE* outfp;
  // if(Zlib::def(fp_, outfp)) {
  //    return outfp;
  // }
  // could use a temp file, but that kinda defeats the purpose...
  
  return fp_;
}

// ============================= free functions ============================
LinAlg::System::ptr read_data(CqFile& cqfile)
{
  int line_sz = 0;
  int elem_sz = 0;

  cqfile.read_bytes(&elem_sz, 1);
  cqfile.read_bytes(&line_sz, 1);
  if(elem_sz != sizeof(short)) {
    throw std::runtime_error("input file header error");
  }
  LinAlg::System::ptr las(new LinAlg::System(line_sz));

  boost::shared_array<short> buf(new short[line_sz]);
  while(cqfile.read_bytes(buf.get(), line_sz)) {
    LinAlg::Vector v(*las);
    std::copy(buf.get(), buf.get() + line_sz, v.begin());
    // for(int i = 0; i < line_sz; ++i)  { v[i] -= 1; }
  }
  
  return las;
}

LinAlg::System::ptr read_data(CqTextFile& cqtextfile)
{
  const size_t dim = cqtextfile.dim();
  LinAlg::System::ptr las(new LinAlg::System(dim));
  std::vector<short> input(dim);
  while(cqtextfile.valid()) {
    if(dim == cqtextfile.readline(input)) {
      LinAlg::Vector v(*las);
      std::copy(input.begin(), input.end(), v.begin());
    }
    else {
      if(cqtextfile.valid()) {
	throw std::runtime_error("incorrect line length in input file");
      }
    }
  }
  return las;
}

} // namespace
