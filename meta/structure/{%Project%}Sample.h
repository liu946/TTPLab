//
// Created by liu on {% DateTime %}.
//

#ifndef PROJECT_{% Project %}SAMPLE_H
#define PROJECT_{% Project %}SAMPLE_H

#include "vector"
using namespace std;

class {% Project %}Sample {
  vector<int> data;
public:
  unsigned size() {
    return data.size();
  }

  static string getClassName() {
    return "{% Project %}Sample";
  }
};


#endif //PROJECT_{% Project %}SAMPLE_H
