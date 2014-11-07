#include <cstdlib>
#include <cassert>
#include <string>
#include <fstream>
#include <algorithm>
#include <numeric>

#include "common.hpp"
#include <n50_cmdline.hpp>

static std::vector<uint32_t> get_nsizes(const std::vector<uint32_t>& in_sizes, bool others) {
  std::vector<uint32_t> res;
  if(in_sizes.empty() && !others) {
    res.push_back(50);
  } else {
    for(auto x : in_sizes) {
      if(x == 0 || x >= 100)
        n50_cmdline::error() << "Invalid Nx size '" << x << "': x must be in (0, 100)";
      res.push_back(x);
    }
  }
  std::sort(res.begin(), res.end());

  return res;
}

int n50_main(int argc, char *argv[]) {
  n50_cmdline args(argc, argv);

  const std::vector<uint32_t> nsizes = get_nsizes(args.N_arg, args.Esize_flag || args.sum_flag);

  std::vector<size_t> sizes;
  std::string         line;
  size_t              total_size = 0;
  double              E = 0.0;
  size_t              contig_i = 0;
  for(auto file : args.file_arg) {
    try {
      std::ifstream is;
      is.exceptions(std::ios::failbit|std::ios::badbit);
      is.open(file);

      int c = is.peek();
      if(c == EOF) continue; // Skip file
      if(c != '>') throw std::runtime_error("Invalid file format");

      while(c != EOF) {
        skip_line(is);

        size_t size = 0;
        for(c = is.peek(); c != '>' && c != EOF; c = is.peek()) {
          std::getline(is, line);
          size += line.size();
        }
        ++contig_i;
        sizes.push_back(size);
        size_t o_total_size  = total_size;
        total_size          += size;
        E = ((double)o_total_size / (double)total_size) * E + (double)(size * size) / total_size;
      }
    } catch(std::ios::failure) {
      std::cerr << "Error with file '" << file << '\'' << std::endl;
      return EXIT_FAILURE;
    } catch(std::runtime_error e) {
      std::cerr << "Error with file '" << file << "': " << e.what() << std::endl;
    }
  }

  // Compute statistics
  std::sort(sizes.begin(), sizes.end(), [](size_t x, size_t y) { return x > y; });
  const size_t sum_size = total_size;
  if(args.size_given)
    total_size = args.size_arg;

  std::vector<size_t> sizes_for_n;
  for(auto s : nsizes) sizes_for_n.push_back(std::ceil((double)s * total_size / 100.0));
  assert(sizes_for_n.size() == nsizes.size());

  size_t csize = 0;
  size_t nsize = 0;
  for(auto s : sizes) {
    csize += s;
    for( ; nsize < nsizes.size() && csize >= sizes_for_n[nsize]; ++nsize) {
      if(!args.no_header_flag)
        std::cout << 'N' << nsizes[nsize] << ' ';
      std::cout << s << '\n';
    }
  }
  for( ; nsize < nsizes.size(); ++nsize) {
    if(!args.no_header_flag)
      std::cout << 'N' << nsizes[nsize] << ' ';
    std::cout << "-\n";
  }
  if(args.sum_flag) {
    if(!args.no_header_flag)
      std::cout << "S ";
    std::cout << sum_size << '\n';
  }
  if(args.average_flag) {
    if(!args.no_header_flag)
      std::cout << "A ";
    std::cout << ((double)sum_size / sizes.size()) << "\n";
  }
  if(args.Esize_flag) {
    if(!args.no_header_flag)
      std::cout << "E ";
    std::cout << E << '\n';
  }

  return EXIT_SUCCESS;
}
