/**
 * \file opm.cpp
 *   \author Marcus Ritt <mrpritt@inf.ufrgs.br> 
 *   \version $Id: emacs 5966 2015-09-22 23:11:05Z ritt $
 *   \date Time-stamp: <2015-09-23 12:27:17 ritt>
 *
 * Open pit mining instance generator and visualization tool.
 */
//#include "opm.H"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <fstream>
using namespace std;

#include <boost/program_options.hpp>
namespace po = boost::program_options;
#include <boost/multi_array.hpp>
using namespace boost;

multi_array<int,2> p; // profits
multi_array<bool,2> s; // selected cells
unsigned w,h;

void savePBM(ofstream& o) {
  o << "P3 " << w << " " << h << " 255 " << endl;
  for(unsigned i=0; i<h; i++) {
    for(unsigned j=0; j<w; j++) {
      if (s[i][j]) {
	if (p[i][j]>0)
	  o << "0 0 " << 2*p[i][j] << " ";
	else
	  o << -2*p[i][j] << " 0 0 ";
      } else {
	if (p[i][j]>0)
	  o << " 0 " << 2*p[i][j] << " 0  ";
	else
	  o << 254+2*p[i][j] << " " << 254+2*p[i][j] << " " << 254+2*p[i][j] << " ";
      }
      
    }
    o << endl;
  }
}

int main(int argc, char *argv[]) {
  srand48(time(0));
  
  // (0) process commandline options
  po::options_description desc("Options");
  desc.add_options()
    ("help", "show help")
    ("w", po::value<unsigned>(), "Width of the instance (for generation)")
    ("h", po::value<unsigned>(), "Height of the instance (for generation)")
    ("ins", po::value<string>(), "Input/Output instance file")
    ("sol", po::value<string>(), "Solution file")
    ("pbm", po::value<string>()->default_value("opm.pbm"), "Instance/Solution visualization")
    ;

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
  po::notify(vm);

  if (vm.count("help") || (!vm.count("ins") && vm.count("sol")) || (!vm.count("sol") && (!vm.count("w") || !vm.count("h")))) {
    cout << desc << endl;
    return 0;
  }
  
  if (vm.count("sol")) {
    // read instance
    ifstream ins(vm["ins"].as<string>());
    ins >> w >> h;
    p.resize(extents[h][w]);
    for(unsigned i=0; i<h; i++)
      for(unsigned j=0; j<w; j++)
	ins >> p[i][j];
    ins.close();
    // read solution
    ifstream sol(vm["sol"].as<string>());
    unsigned w_,h_;
    sol >> w_ >> h_;
    s.resize(extents[h][w]);
    if (w != w_ || h != h_)
      throw("Instance and solution don t match");
    for(unsigned i=0; i<h; i++)
      for(unsigned j=0; j<w; j++)
	sol >> s[i][j];
    sol.close();
    // save the instance and draw a bitmap
    ofstream pbm(vm["pbm"].as<string>());
    savePBM(pbm);
    pbm.close();
  } else {
    w = vm["w"].as<unsigned>();
    h = vm["h"].as<unsigned>();
    p.resize(extents[h][w]);
    s.resize(extents[h][w]);
    
    // generate an instance
    for(unsigned i=0; i<h; i++)
      for(unsigned j=0; j<w; j++) {
	p[i][j]=int(lrand48()%254)-127;
	if (p[i][j]==0) // exclude 0 profits
	  p[i][j]++;
      }
    
    // save the instance
    ofstream ins(vm["ins"].as<string>());
    ins << w << " " << h << endl;
    for(unsigned i=0; i<h; i++) {
      for(unsigned j=0; j<w; j++)
	ins << p[i][j] << " ";
      ins << endl;
    }
    ins.close();

    ofstream pbm(vm["pbm"].as<string>());
    savePBM(pbm);
    pbm.close();
  }
}
