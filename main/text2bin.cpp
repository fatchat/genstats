#include <cstdio>
#include <cstring>
#include <string>

typedef short output_type;

int main(int argc, char* argv[])
{
  if(argc < 2) {
    printf("Usage: %s <input filename>\n", argv[0]);
    return 1;
  }
  const char *infile = argv[1];

  FILE* fpin = fopen(infile, "r");
  if(!fpin) {
    printf("Could not open %s for reading, quitting.\n", infile);
    return 1;
  }
  printf("Opened %s successfully.\n", infile);

  std::string outfile(infile);
  outfile += ".bin";
  FILE* fpout = fopen(outfile.c_str(), "w");
  if(!fpout) {
    printf("Could not open %s for writing.\n", outfile.c_str());
    return 1;
  }

  const int dim = 2000;
  {
    const int sz = sizeof(output_type);
    fwrite(&sz, sizeof(int), 1, fpout);
    fwrite(&dim, sizeof(int), 1, fpout);
    //printf("wrote sz=%d and dim=%d to %s\n", sz, dim, outfile.c_str());
  }

  const size_t line_len = 2 * dim + 1; // +1 for \n
  char buf[line_len + 1];
  bool die = false;

  while(fgets(&buf[0], line_len + 1, fpin)) { // +1 for \0
    if(strlen(buf) != line_len) {
      printf("Incorrect line length encountered: %d instead of %d in [%s]\n",
	     (int)strlen(buf), (int)line_len, buf);
      break;
    }
    output_type bin[dim];
    int bufx = 0;
    int binx = 0;
    while(buf[bufx]) {
      if (buf[bufx] == '\n') {
	if(bufx != 2*dim) {
	  printf("error: read %d characters instead of %d\n", bufx, 2*dim);
	  die = true;
	}
	break;
      }
      switch(buf[bufx]) {
      case '0':
	bin[binx] = (output_type)-1;
	break;
      case '1':
	bin[binx] = (output_type)0;
	break;
      case '2':
	bin[binx] = (output_type)1;
	break;
      default:
	printf("Encountered unexpected character: [%c]\n", buf[bufx]);
	die = true;
	break;
      }
      if(die) break;
      ++binx;
      bufx += 2;
    } // while buf[]

    if(die) break;

    const int written = fwrite((const void*)bin, sizeof(bin[0]), binx, fpout);
    if(written != binx) {
      printf("Wrote %d byes instead of %d to %s.\n",
	     written, binx, outfile.c_str());
      die = true;
      break;
    }
  } // while fgets

  fclose(fpin);
  fclose(fpout);
  if(die) {
    printf("File conversion failed.\n");
    return 1;
  }

  printf("File conversion successful, wrote %s.\n", outfile.c_str());
  return 0;
}
