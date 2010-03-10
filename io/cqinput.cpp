#include "io/cqinput.h"

#include <stdexcept>
#include <sstream>
#include <fstream>
#include <boost/smart_ptr.hpp>

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

// ============================= CqInput ==============================
size_t CqInput::get_dim_(FILE* fp)
{
  fread(&elem_sz_, sizeof(int), 1, fp);
  fread(&line_sz_, sizeof(int), 1, fp);
  //printf("get_dim_() read sz=%d and dim=%d\n", elem_sz_, line_sz_);
  if(elem_sz_ != sizeof(input_type)) {
    std::ostringstream error;
    error << "input file in the wrong format: "
	  << elem_sz_ << " should be " << sizeof(input_type);
    throw std::runtime_error(error.str());
  }
  return (size_t)line_sz_;
}

CqInput::CqInput(CqFile& cqfile)
  : elem_sz_(0)
  , line_sz_(0)
  , las_(get_dim_(cqfile.fp()))
{
  readData_(cqfile.fp());
}

void CqInput::readData_(FILE* fp)
{
  boost::shared_ptr<input_type> buf(new input_type[line_sz_]);
  while(1) {
    const int nread = fread(buf.get(), elem_sz_, line_sz_, fp);
    if(feof(fp)) {
      break;
    }
    if(nread != line_sz_) {
      std::ostringstream error;
      error << "encountered line of incorrect length: " << nread
	    << " instead of " << line_sz_;
      throw std::runtime_error(error.str());
    }
    LinAlg::Vector v(las_);
    for(int i = 0; i < line_sz_; ++i) {
      v[i] = *(buf.get() + i);
      //      v[i] -= 1;
    }
  }
}
